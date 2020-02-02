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
#include <unistd.h>

#include "menu.h"
#include "utilities.h"

// De listagens_fuzzySearch
// *********************************************************************************************************************
colSize_t getSize_art() { return artigos.size; }

/**
 * @brief   Retorna o nome do artigo na posição 'i'.
 * @param i Posição do artigo.
 * @returns O nome do artigo na posição 'i'.
 */
char* getStr_art(colSize_t i) { return artigos.data[i].nome; }

/**
 * @brief   Imprime o artigo na posição 'i'.
 * @param i Posição do artigo a imprimir.
 */
void print_art(colSize_t i) {
    menu_printArtigo(&artigos.data[i]);
    printf("\n");
}

colSize_t getSize_uti() { return clientes.size; }

/**
 * @brief   Retorna o nome do cliente na posição 'i'.
 * @param i Posição do utilizador.
 * @returns O nome do cliente na posição 'i'.
 */
char* getStr_uti(colSize_t i) { return clientes.data[i].nome; }

/**
 * @brief   Imprime o cliente na posição 'i'.
 * @param i Posição do utilizador a imprimir.
 */
void print_uti(colSize_t i) {
    menu_printUtilizador(clientes.data[i]);
    printf("\n");
}




// De listagem_procura
// *********************************************************************************************************************
/**
 * @brief         Calcula a distância de levenshtein de duas palavras.
 * @param a       Primeira palavra.
 * @param b       Segunda palavra.
 * @param aLength Tamanho da primeira palavra.
 * @param bLength Tamanho da segunda palavra.
 * @return        A distância de levenshtein das duas palavras.
 * @note          Baseado em "https://github.com/wooorm/levenshtein.c/blob/master/levenshtein.c".
 */
size_t listagens_levenshtein(wchar_t const* const a, wchar_t const* const b, const size_t aLength,
                             const size_t bLength) {
    if (aLength == 0) { return bLength; }
    if (bLength == 0) { return aLength; }
    size_t* cache = malloc(aLength * sizeof(size_t));
    size_t  aIndex;
    size_t  distance;
    size_t  bDistance;
    size_t  result;
    wchar_t code;
    for (aIndex = 0; aIndex < aLength; aIndex++) { cache[aIndex] = aIndex + 1; }
    for (size_t bIndex = 0; bIndex < bLength; bIndex++) {
        code   = b[bIndex];
        result = distance = bIndex;
        for (aIndex = 0; aIndex < aLength; aIndex++) {
            bDistance     = (code == a[aIndex]) ? distance : distance + 1;
            distance      = cache[aIndex];
            cache[aIndex] = result = (distance > result) ? (bDistance > result) ? result + 1 : bDistance
                                                         : (bDistance > distance) ? distance + 1 : bDistance;
        }
    }
    free(cache);
    return result;
}

/**
 * @brief       Retorna uma coleção das palavras da frase.
 * @param frase Frase contítuida por uma ou mais palavras.
 * @returns     Uma coleção constituída pelas palavras que constituem a 'frase'.
 */
wstrcol listagens_partirEspacos(wchar_t const* frase) {
    wstrcol        col = wstrcol_new();
    wchar_t const* beg = frase;
    wchar_t const* end = frase;
    // Posicionar beg no inicio da proxima palavra
    for (beg; *beg && iswspace(*beg); beg++)
        ;
    if (!(*beg)) return col; // Ultima palavra
    // Posicionar end no mesmo local que beg
    end = beg;
    // Ciclo principal
    for (end; *end; end++) {
        // Espaço encontrado
        if (iswspace(*end)) {
            // Criar novo elemnto na colção
            protectFcnCall(wstrcol_push(&col, malloc(sizeof(wchar_t) * (end - beg + 1))), "wstrcol_push falhou");
            // Copiar elemtos de [beg, end[ para o novo elemnto
            wchar_t* cur;
            for (cur = col.data[col.size - 1]; beg != end; beg++) {
                (*cur) = towupper(*beg);
                cur++;
            }
            *cur = 0;

            // Posicionar beg no inicio da proxima palavra
            for (beg; *beg && iswspace(*beg); beg++)
                ;
            if (!(*beg)) return col; // Ultima palavra
            // Posicionar end no mesmo local que beg
            end = beg;
        }
    }
    if (end != beg) {
        // Criar novo elemnto na colção
        protectFcnCall(wstrcol_push(&col, malloc(sizeof(wchar_t) * (end - beg + 1))), "wstrcol_push falhou");
        // Copiar elemtos de [beg, end[ para o novo elemnto
        wchar_t* cur;
        for (cur = col.data[col.size - 1]; beg != end; beg++) {
            (*cur) = towupper(*beg);
            cur++;
        }
        *cur = 0;
    }
    return col;
}

/**
 * @brief            Pesquisa difusa, tenta pesquisar uma coleção por um nome.
 * @param find       Palavras a pesquisar na coleção.
 * @param maxDist    Distância máxia para imprimir um objeto.
 * @param step       Caso ainda não tenham sido impressos objetos suficientes a
 *                   'maxDist' vai ser aumentada por esta quantidade até que o
 *                   numero de objetos pretendido seja impresso.
 * @param minToPrint Número minimo de objetos a imprimir.
 * @param getSize    Ponteiro para uma função que retorna o tamanho da coleção
 *                   de objetos a pesquisar.
 * @param getStr     Ponteiro para uma função que retorna o nome do objeto a ser
 *                   pesquisado.
 * @param getPrint   Ponteiro para uma função que imprime o objeto a ser
 *                   pesquisado.
 * @returns          Objetos impessos.
 */
size_t listagens_fuzzySearch(char* find, size_t maxDist, size_t minToPrint, size_t const step,
                             colSize_t (*const getSize)(), char* (*const getStr)(colSize_t),
                             void (*const getPrint)(colSize_t)) {
    menu_printDiv();
    menu_printHeader("Resultados da Pesquisa");
    // wfind é find em letra grande e wchar_t
    size_t   size  = strlen(find) + 1;
    wchar_t* wfind = malloc(sizeof(wchar_t) * size);
    mbstowcs(wfind, find, size);
    wstrcol querry = listagens_partirEspacos(wfind);
    free(wfind);
    if (minToPrint > getSize()) minToPrint = getSize();

    char*  wasPrinted;
    size_t printed_n = 0;
    protectVarFcnCall(wasPrinted, calloc(getSize(), sizeof(char)), "calloc falhou");

    // Main Loop de pesquisa
    do {
        for (size_t i = 0; i < getSize(); i++) {
            // Obter palavras da pesquisa
            size  = strlen(getStr(i));
            wfind = malloc(sizeof(wchar_t) * size);
            mbstowcs(wfind, getStr(i), size);
            wstrcol dataPool = listagens_partirEspacos(wfind);
            free(wfind);
            // Fazer pesquisa
            for (size_t iquerry = 0; iquerry < querry.size; iquerry++) {
                wchar_t const* const curQuerryStr = querry.data[iquerry];
                size_t const         curQuerryLen = wcslen(curQuerryStr);
                for (size_t idata = 0; idata < dataPool.size; idata++) {
                    size_t const dist = listagens_levenshtein(curQuerryStr, dataPool.data[idata], curQuerryLen,
                                                              wcslen(dataPool.data[idata]));
                    if (dist <= maxDist && !wasPrinted[i]) {
                        getPrint(i);
                        wasPrinted[i] = 1;
                        printed_n++;
                        break;
                    }
                }
            }
            // Libertar memória
            wstrcol_free(&dataPool);
        }
        maxDist += step;
    } while (printed_n < minToPrint);

    wstrcol_free(&querry);
    free(wasPrinted);
    menu_printDiv();
    return printed_n;
}




// De listagem_imprimir_recibo
// *********************************************************************************************************************
int pred_printUti(utilizador const* const u, int64_t* const i);

/**
 * @brief      Pode ser utilizado como um iterador, imprime encomenda num recibo.
 * @param e    Encomenda a ser impressa
 * @param data Ano e mês do recibo como também alguns dados remetentes ao total
 *             do preço do recibo e artigos compras e encomendas vendidas.
 * @returns    0
 */
int listagens_pred_printencRec(encomenda const* const e, struct {
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
        data->encomendas += 1;
    }
    return 0;
}




// De interface_outras_listagens
// *********************************************************************************************************************
/**
 * @brief Premite imprimir um recibo para um certo utilizador.
 */
void listagem_imprimir_recibo() {
    menu_printDiv();
    menu_printHeader("Recibo de Cliente");
    printf("Inserir ano");
    int64_t ano = menu_readInt64_t();
    menu_printInfo("Inserir mês");
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
        utilizadorcol_iterateFW(&clientes, (utilizadorcol_pred_t) &pred_printUti, &max);
        menu_printInfo("Insira o ID do Cliente para editar");
        id = menu_readInt64_tMinMax(-2, max - 1);
    }
    if (id == -1)
        return;
    else
        ID_cliente = id;
    utilizador const* const cliente = &clientes.data[ID_cliente];

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
    printf("\n*** Mês do recibo: %lu/%lu\n", ano, mes);
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
    encomendacol_iterateFW(&encomendas, (encomendacol_pred_t) &listagens_pred_printencRec, &data);
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

/**
 * @brief Premite ao utilizador pesquisar pelo nome de um artigo/ utilizador.
 */
void listagem_procura() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Pesquisa");
        char* tmp = NULL;

        switch (menu_selection(&(strcol) {.size = 2,
                                          .data = (char*[]) {
                                              "Pesquisar artigos por nome",      // 0
                                              "Pesquisar utilizadores por nome", // 1
                                          }})) {
            case -1: return;
            case 0:
                printf("Inserir nome para pesquisar");
                tmp = menu_readNotNulStr();
                listagens_fuzzySearch(tmp, 0, 5, 3, &getSize_art, &getStr_art, &print_art);
                freeN(tmp);
                break;
            case 1:
                printf("Inserir nome para pesquisar");
                tmp = menu_readNotNulStr();
                listagens_fuzzySearch(tmp, 0, 5, 3, &getSize_uti, &getStr_uti, &print_uti);
                freeN(tmp);
                break;
        }
    }
}

/**
 * @brief Imprime os utilizadores que mais gastaram num certo mês.
 */
void listagem_utiMaisGasto() {
    menu_printDiv();
    menu_printHeader("Clientes Que Mais Gastaram");
    printf("Inserir ano");
    int64_t ano = menu_readInt64_t() - 1900;
    menu_printInfo("Inserir mês");
    int64_t mes = menu_readInt64_tMinMax(1, 12) - 1;

    uint64_t* gastoUti = calloc(clientes.size, sizeof(uint64_t));
    for(colSize_t i = 0; i < encomendas.size; i++) {
        encomenda const * const enc = &encomendas.data[i];
        struct tm * time = localtime(&enc->tempo);
        if(time->tm_mon == mes || time->tm_year == ano)
            gastoUti[enc->ID_cliente] += encomenda_CalcPreco(enc, &artigos);
    }

    uint64_t max = 1;
    colSize_t maxid;
    menu_printHeader("Utilizadores Ordenados");
    while (max != 0) {
        max = 0;
        for (colSize_t i = 0; i < clientes.size; i++) {
            if(gastoUti[i] > max) {
                maxid = i;
                max = gastoUti[i];
            }
        }
        if(max > 0) {
            menu_printUtilizador(clientes.data[maxid]);
            printf("   TOTAL GASTO: %luc\n", max);
            gastoUti[maxid] = 0;
        }
    }
    free(gastoUti);
}