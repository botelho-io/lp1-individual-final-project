/**
 * @file    outrasListagens.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   As 5 listagens extra propostas pelos alunos.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#include "outrasListagens.h"

#include <fcntl.h>

#include "menu.h"
#include "utilities.h"




// De listagem_imprimir_recibo
// *********************************************************************************************************************
int printUtiVP(utilizador const* const u, int64_t* const i);

/**
 * @brief      Pode ser utilizado como um iterador, imprime encomenda num recibo.
 * @param e    Encomenda a ser impressa
 * @param data Ano e mês do recibo como também alguns dados remetentes ao total
 *             do preço do recibo e artigos compras e encomendas vendidas.
 * @returns    0
 */
int listagens_printencRecVP(encomenda const* const e, struct {
    int       ano;
    int       mes;
    colSize_t ID_cliente;
    uint64_t  total;
    uint64_t  art;
    uint64_t  compras;
    uint64_t  encomendas;
} * data) {
    struct tm const* const t = localtime(&e->tempo);
    if (t->tm_mon == data->mes && t->tm_year == data->ano && e->ID_cliente == data->ID_cliente) {
        printf("* Dia %d/%d/%d\n", 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday);
        printf("    * ARTIGOS COMPRADOS:\n");
        int64_t   preco_art;
        colSize_t i;
        for (i = 0; i < e->compras.size; i++) {
            // inicio
            compra const* const c = &e->compras.data[i];
            artigo const* const a = &artigos.data[c->IDartigo];
            preco_art             = a->preco_cent;
            printf("        * ");
            // consumo
            printf("CONSUMO %s", (a->meta & ARTIGO_GRUPO_ANIMAL) ? "ANIMAL" : "HUMANO");
            // preço
            printf("\t- PREÇO %luc\t- IVA", a->preco_cent);
            // iva
            switch (a->meta & ARTIGO_IVA) {
                case ARTIGO_IVA_INTERMEDIO:
                    printf(" %d%%", (int) ((ARTIGO_IVA_INTERMEDIO_VAL - 1) * 100));
                    preco_art *= ARTIGO_IVA_INTERMEDIO_VAL;
                    break;
                case ARTIGO_IVA_NORMAL:
                    printf(" %d%%", (int) ((ARTIGO_IVA_NORMAL_VAL - 1) * 100));
                    preco_art *= ARTIGO_IVA_NORMAL_VAL;
                    break;
                case ARTIGO_IVA_REDUZIDO:
                    printf(" %d%%", (int) ((ARTIGO_IVA_REDUZIDO_VAL - 1) * 100));
                    preco_art *= ARTIGO_IVA_REDUZIDO_VAL;
                    break;
            }
            // total
            printf("\t- TOTAL: %ldc", preco_art * c->qtd);
            // quantidade
            printf("\t- QUANTIDADE: %ld", c->qtd);
            // receita
            if (a->meta & ARTIGO_NECESSITA_RECEITA) {
                printf("\t- RECEITA (%12.12s)", c->receita);
            } else
                printf("\t- ARTIGO DE VENDA LIVRE ");
            // nome
            printf("\t\"%s\"", protectStr(a->nome));
            // fim
            data->art += c->qtd;
            printf("\n");
            fflush(stdout);
        }
        const uint64_t tot = encomenda_CalcPreco(e, &artigos);
        printf("    * TOTAL %ld\n", tot);
        fflush(stdout);
        data->compras += i;
        data->total += tot;
    }
    data->encomendas += 1;
    return 0;
}




// De interface_outras_listagens
// *********************************************************************************************************************
/**
 * @brief Premite imprimir um recibo para um certo utilizador.
 */
void listagem_imprimir_recibo() {
    printf("Inserir ano");
    int64_t ano = menu_readInt64_t();
    printf("Inserir mês");
    int64_t mes = menu_readInt64_tMinMax(1, 12);

    // Questionar utilizador
    colSize_t ID_cliente = 0;
    int64_t   id         = -2;
    int64_t   max;
    id = -2;
    menu_printHeader("Selecione Cliente");
    while (id == -2) {
        printf("      ID      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        max = 0;
        utilizadorcol_iterateFW(&utilizadores, (utilizadorcol_pred_t) &printUtiVP, &max);
        menu_printInfo("Insira o ID do Cliente para editar");
        id = menu_readInt64_tMinMax(-2, max - 1);
    }
    if (id == -1)
        return;
    else
        ID_cliente = id;
    utilizador const* const cliente = &utilizadores.data[ID_cliente];

    // Imprimir recibo
    int bak = dup(1);
    protectFcnCall((bak != -1), "dup falhou");
    int new               = 0;
    int printBoth         = 0;
    int needsToRestoreOut = 0;
    switch (menu_selection(&(strcol) {.size = 3,
                                      .data = (char*[]) {
                                          "Imprimir no ecrã",     // 0
                                          "Imprimir em ficheiro", // 1
                                          "Imprimir em ambos",    // 2
                                      }})) {
        case -1: return;
        case 0: break;
        case 2: printBoth = 1;
        case 1:
            needsToRestoreOut = 1;
            printf("Introduza nome de ficheiro");
            char* f = menu_readNotNulStr();
            new     = open(f, O_WRONLY | O_CREAT);
            protectFcnCall((new != -1), "open falhou");
            protectFcnCall((dup2(new, 1) != -1), "dup2 falhou");
            close(new);
            freeN(f);
            break;
    }

PRINT_BEGUIN:
    menu_printDiv();
    menu_printHeader("Recibo Mensal");
    printf("\n*** Mês do recibo: %lu/%lu", ano, mes);
    printf("*** NOME %s\n", protectStr(cliente->nome));
    printf("*** NIF  %9.9s\n", cliente->NIF);
    printf("*** CC   %12.12s\n", cliente->CC);
    struct {
        int       ano;
        int       mes;
        colSize_t ID_cliente;
        uint64_t  total;
        uint64_t  art;
        uint64_t  compras;
        uint64_t  encomendas;
    } data;
    data.ano        = ((int) ano) - 1900;
    data.mes        = ((int) mes) - 1;
    data.ID_cliente = ID_cliente;
    data.total      = 0;
    data.art        = 0;
    data.compras    = 0;
    data.encomendas = 0;
    encomendacol_iterateFW(&encomendas, (encomendacol_pred_t) &listagens_printencRecVP, &data);
    printf("*** Artigos vendidos neste mês: %ld\n", data.art);
    printf("*** Compras vendidas neste mês: %ld\n", data.compras);
    printf("*** Encomendas vendidas neste mês: %ld\n", data.encomendas);
    printf("*** Total mensal: %ld c\n", data.total);
    menu_printHeader("Final do Recibo");
    menu_printDiv();

    if (needsToRestoreOut) {
        needsToRestoreOut = 0;
        fflush(stdout);
        dup2(bak, 1);
        close(bak);
        if (printBoth) { goto PRINT_BEGUIN; }
    }
}