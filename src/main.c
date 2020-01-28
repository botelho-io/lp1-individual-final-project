/**
 * @file    main.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Ficheiro principal onde consta maior parte da lógica do programa.
 * @version 1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 */

#ifdef DEBUG_BUILD
#    include <sys/stat.h>
#endif

#include <errno.h>
#include <stdint.h>

// Estado do programa
// *********************************************************************************************************************
#define COL_IMPLEMENTACAO
#include "artigo.h"
#include "encomenda.h"
#include "utilizador.h"
#include "menu.h"

#ifndef artigocol_H
#    define artigocol_H
#    define COL_TIPO artigo
#    define COL_NOME artigocol
#    define COL_DEALOC(X) freeArtigo(X)
#    define COL_WRITE(X, F) save_artigo(F, X)
#    define COL_READ(X, F) load_artigo(F, X)
#    include "./colecao.h"
#endif

#ifndef encomendacol_H
#    define encomendacol_H
#    define COL_TIPO encomenda
#    define COL_NOME encomendacol
#    define COL_DEALOC(X) freeEncomenda(X)
#    define COL_WRITE(X, F) save_encomenda(F, X)
#    define COL_READ(X, F) load_encomenda(F, X)
#    include "./colecao.h"
#endif

#ifndef utilizadorcol_H
#    define utilizadorcol_H
#    define COL_TIPO utilizador
#    define COL_NOME utilizadorcol
#    define COL_DEALOC(X) freeUtilizador(X)
#    define COL_WRITE(X, F) save_utilizador(F, X)
#    define COL_READ(X, F) load_utilizador(F, X)
#    include "./colecao.h"
#endif

artigocol     artigos;      ///< Artigos da seção atual
encomendacol  encomendas;   ///< Encomendas
utilizadorcol utilizadores; ///< Utilizadores existentes no registo

#include "outrasListagens.h"

/* Descrição do problema ***********************************************************************************************
Login -
    Apenas requer “F” ou “DC” (Funcionário ou Diretor Clínico)

Funcionário –
    Editar/ Criar novos clientes
    Introduzir a compra do cliente no sistema
        No final de cada compra o stock deverá ser atualizado automaticamente
        Não sendo permitida a venda de artigos que se encontrem semstock

Diretor  clínico –
    Editar/ criar clientes, artigos, compras, entre outras
    Gerar/reimprimir a fatura para um cliente
        Gerada no ecrã e/ou escrita para ficheiro
        Conter a listagem de todosos artigos vendidos

Artigo -
    É  armazenado  ou  vendido  pela  Farmácia
    Pode pertencer  ao  grupo  “Humano”  ou  “Animal”
    De  venda  livre  ou  necessitar  de receita
    Deve  ser  solicitado  o  número  da receita para que este fique no sistema
    O preço final de um artigo contem  o valor base e o respetivo valor do IVA

Persistência  de  dados –
    A  aplicação  deverá  permitir  guardar/carregar  dados  em / de  ficheiro

IVA –
    Imposto sobre o valor acrescentado associada a um tipo de produto
    Este pode  variar  de  acordo  com  3  categorias:
        Taxa normal  (23%),
        Taxa intermédia  (13%)
        Taxa reduzida (6%)

Compra –
    Conjunto  de  artigos  vendidos  a  um  cliente

Cliente –
    Representa um cliente que faz uma compra
    Regista  o  nome,  NIF,  número  de  CC
*/




// De interface_cliente
// *********************************************************************************************************************
/**
 * @brief   Pode ser utilizado como um iterador, imprime um utilizador.
 * @param u Cliente a ser impresso.
 * @param i Deve ser inicializado como 0, no final irá conter o número de
 *          utilizadores impressos.
 * @returns 0
 */
int printUtiVP(utilizador const* const u, int64_t const* i) {
    printf("   %8lu   |   ", *i++);
    menu_printUtilizador(*u);
    printf("\n");
    return 0;
}

int form_editar_cliente(utilizador* const u, int isNew) {
    // TODO: implementar
}




// De interface_artigo
// *********************************************************************************************************************
/**
 * @brief   Pode ser utilizado como um iterador, imprime um artigo.
 * @param a Artigo a ser impresso.
 * @param i Deve ser inicializado como 0, no final irá conter o número de
 *          artigos impressos.
 * @returns 0
 */
int printArtVP(artigo const* const a, int64_t const* i) {
    printf("   %8lu   |   ", *i++);
    menu_printArtigo(a);
    printf("\n");
    return 0;
}

int form_editar_artigo(artigo* const a, int isNew) {
    // TODO: implementar
}




// De form_editar_encomenda
// *********************************************************************************************************************
/**
 * @brief   Pode ser utilizado como um iterador, imprime uma compra.
 * @param c Compra a ser impressa.
 * @param i Deve ser inicializado como 0, no final irá conter o número de
 *          compras impressas.
 * @returns 0
 */
int printComVP(compra const* const c, int64_t const* i) {
    printf("   %8lu   |   ", *i++);
    menu_printCompra(c, &artigos);
    printf("\n");
    return 0;
}

/**
 * @brief       Função responsável por editar todos os parametros de uma compra.
 * @param c     Compra que será editada.
 * @param isNew Deve ser 1 se o artigo é um novo artigo e 0 se é um artigo
 *              válido que será editado.
 * @returns     1 Se o artigo foi editado.
 * @returns     0 Se a compra não foi editada/ foi eleminada; a compra c terá
 *              que ser eleminada pois é inválida.
 */
int form_editar_compra(compra* const c, int isNew) {
    menu_printDiv();
    if(isNew) menu_printHeader("Criar Nova Compra");
    else menu_printHeader("Editar Compra");

    artigo* art;
    if(!isNew) {
        art = &artigos.data[c->IDartigo];
        // Fazer reset do stock
        art->stock += c->qtd;
        // Eleminar compra
        printf("Eleminar compra (S / N)");
        int YN = 2;
        while (YN == 2) {
            YN = menu_YN('S', 'N');
            switch(YN) {
                case 0: break;
                case 1: return 0;
                default: menu_printError("resposta inválida, devia de ser S ou N");
            }
        }
    } else {
        // Ler tipo de artigo
        menu_printInfo("escolher artigo");
        int64_t id = -2;
        int64_t max;
        while (id == -2) {
            printf("      ID      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            max = 0;
            artigocol_iterateFW(&artigos, (artigocol_pred_t) &printComVP, &max);
            menu_printInfo("Insira o ID do artigo que será vendido na compra");
            id = menu_readInt64_tMinMax(-2, max - 1);
        }
        c->IDartigo = id;
        art = &artigos.data[id];

        // Ler receita
        if(art->meta & ARTIGO_NECESSITA_RECEITA) {
            char* tmp = NULL;
            menu_printInfo("artigo necessita de receita para ser vendido");
            while (1) {
                printf("Insira os 12 characteres da receita do artigo");
                freeN(tmp);
                tmp = menu_readNotNulStr();
                if(strlen(tmp) != 12) {
                    menu_printError("receitas médicas necessitam de ter 12 dígitos");
                } else {
                    size_t i;
                    for (i = 0; i < 12; i++) {
                        if(!isdigit(tmp[i])) {
                            menu_printError("receitas médicas são compostas apenas de dígitos");
                            break;
                        }
                    }
                    if(i == 12) {
                        memcpy(&c->receita, tmp, 12);
                        break;
                    }
                }
            }
        }
    }

    // Ler quantidade
    if(art->stock == 0) {
        menu_printError("não existe stock do artigo atua");
        return 0;
    }
    if(art->meta & ARTIGO_DESATIVADO) {
        menu_printError("o artigo selecionado não se encontra disponivél para venda");
        return 0;
    }
    printf("Insira a quantidade de artigos para vender nesta compra");
    if(!isNew) printf(" (%ld)", c->qtd);
    c->qtd = menu_readInt64_tMinMax(1, art->stock);
    art->stock -= c->qtd;
    return 1;
}




#define GENERIC_EDIT(nome, colect, col, col_pred, editfnc, nomenew)                                                    \
    menu_printDiv();                                                                                                   \
    menu_printHeader("Selecione " nome);                                                                               \
    int64_t id = -2;                                                                                                   \
    int64_t max;                                                                                                       \
    while (id == -2) {                                                                                                 \
        printf("      ID      |   Item\n");                                                                            \
        printf("         -2   |   Reimprimir\n");                                                                      \
        printf("         -1   |   Sair\n");                                                                            \
        max = 0;                                                                                                       \
        COL_EVAL(colect, _iterateFW)(&col, (COL_EVAL(colect, _pred_t)) &col_pred, &max);                               \
        printf("   %8lu   |   Criar Novo " nome "\n", max++);                                                          \
        menu_printInfo("Insira o ID do " nome " para editar");                                                         \
        id = menu_readInt64_tMinMax(-2, max - 1);                                                                      \
    }                                                                                                                  \
    if (id != -1) {                                                                                                    \
        if (id == max - 1) {                                                                                           \
            /* Novo, adicionar ao vetor*/                                                                              \
            protectFcnCall(COL_EVAL(colect, _push)(&col, nomenew()), #colect "_push falhou");                          \
        }                                                                                                              \
                                                                                                                       \
        /* id é o ID do cliente a editar*/                                                                             \
        if(!editfnc(&col.data[id], id == max - 1)) {                                                                   \
            COL_EVAL(colect, _DEALOC)(&col.data[id]);                                                                  \
            COL_EVAL(colect, _moveBelow) (&col, id);                                                                   \
            menu_printInfo(nome " removido.");                                                                         \
        }                                                                                                              \
    }                                                                                                                  \

// De interface_encomenda
// *********************************************************************************************************************
/**
 * @brief   Pode ser utilizado como um iterador, imprime uma encomenda.
 * @param e Encomenda a ser impressa.
 * @param i Deve ser inicializado como 0, no final irá conter o número de
 *          encomendas impressas.
 * @returns 0
 */
int printEncVP(encomenda const* const e, int64_t const* i) {
    printf("   %8lu   |   ", *i++);
    menu_printEncomendaBrief(e, &utilizadores, &artigos);
    printf("\n");
    return 0;
}

int form_editar_encomenda(encomenda* const  e, int isNew) {
    GENERIC_EDIT("Compra", compracol, e->compras, printComVP, form_editar_compra, new_compra);

    printf("Deseja alterar o id do cliente? (S / N)");
    if ( menu_YN('S', 'N') == 1 ) {
        menu_printHeader("Selecione Cliente");
        id = -2;
        max;
        while (id == -2) {
            printf("      ID      |   Item\n");
            printf("         -2   |   Reimprimir\n");
            printf("         -1   |   Sair\n");
            max = 0;
            utilizadorcol_iterateFW(&utilizadores, (utilizadorcol_pred_t) &printUtiVP, &max);
            menu_printInfo("Insira o ID do Cliente");
            id = menu_readInt64_tMinMax(-2, max - 1);
        }
        if (id != -1) {
            e->ID_cliente = id;
        }
    }
    e->tempo = time(NULL);
}




// De inteface_diretor
// *********************************************************************************************************************
/**
 * @brief Permite selecionar um utilizador que será editado/criado
 */
void interface_editar_cliente() {
    GENERIC_EDIT("Cliente", utilizadorcol, utilizadores, printUtiVP, form_editar_cliente, newUtilizador);
}

void interface_editar_artigo() {
    GENERIC_EDIT("Artigo", artigocol, artigos, printArtVP, form_editar_artigo, newArtigo);
}

void interface_editar_encomenda() {
    GENERIC_EDIT("Encomenda", encomendacol, encomendas, printEncVP, form_editar_encomenda, newEncomenda);
}

void interface_imprimir_recibo() {
    // TODO: Implementar
}

void interface_outras_listagens() {
    // TODO: Implementar
}




// De interface_funcionario
// *********************************************************************************************************************
void interface_criar_compra() {
    // TODO: implementar
}




// De interface_inicio
// *********************************************************************************************************************
/**
 * @brief As opções que remetem ao diretor
 */
void interface_diretor() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Menu de Diretor Clínico");
        switch (menu_selection(&(strcol) {.size = 5,
                                          .data = (char*[]) {
                                              "Editar/ criar cliente",   //
                                              "Editar/ criar artigo",    //
                                              "Editar/ criar encomenda", //
                                              "Imprimir recibo",         //
                                              "Outras Listagens",        //
                                          }})) {
            case -1: return;
            case 0: interface_editar_cliente(); break;
            case 1: interface_editar_artigo(); break;
            case 2: interface_editar_encomenda(); break;
            case 3: interface_imprimir_recibo(); break;
            case 4: interface_outras_listagens(); break;
        }
    }
}

/**
 * @brief As opções que remetem a um funcionario
 */
void interface_funcionario() {
    while (1) {
        menu_printDiv();
        menu_printHeader("Menu de Funcionário");
        switch (menu_selection(&(strcol) {.size = 2,
                                          .data = (char*[]) {
                                              "Editar/ criar cliente", //
                                              "Criar compra",          //
                                          }})) {
            case -1: return;
            case 0: interface_editar_cliente(); break;
            case 1: interface_criar_compra(); break;
        }
    }
}

/**
 * @brief Responsavél por gravar os dados em ficheiro
 */
void funcional_save() {
    menu_printDiv();
    menu_printInfo("a escrever em ficheiro");

    // Abrir ficheiro
    FILE* dataFile;
    protectVarFcnCall(dataFile, fopen("saved_data.bin", "wb"), "ficheiro não pode ser aberto");

    // Escrever artigos
    protectFcnCall(artigocol_write(&artigos, dataFile), "impossível escrever artigos no ficheiro");

    // Escrever encomendas
    protectFcnCall(encomendacol_write(&encomendas, dataFile), "impossível escrever encomendas no ficheiro");

    // Escrever utilizadores
    protectFcnCall(utilizadorcol_write(&utilizadores, dataFile), "impossível escrever utilizadores no ficheiro");

    fclose(dataFile);
    menu_printInfo("ficheiro gravado");
}

/**
 * @brief Responsavél por carregar o estado de ficheiro
 */
void funcional_load() {
    menu_printDiv();
    menu_printInfo("a carregar de ficheiro");

    // Eliminar dados
    artigocol_free(&artigos);
    encomendacol_free(&encomendas);
    utilizadorcol_free(&utilizadores);

    // Abrir ficheiro
    FILE* dataFile;
    protectVarFcnCall(dataFile, fopen("saved_data.bin", "rb"), "ficheiro não pode ser aberto");

    // Carregar artigos
    protectFcnCall(artigocol_read(&artigos, dataFile), "impossível carregar artigos de ficheiro");

    // Carregar encomendas
    protectFcnCall(encomendacol_read(&encomendas, dataFile), "impossível carregar encomendas de ficheiro");

    // Carregar utilizadores
    protectFcnCall(utilizadorcol_read(&utilizadores, dataFile), "impossível carregar utilizadores de ficheiro");

    fclose(dataFile);
    menu_printInfo("dados carregados");
}




// Entry point
// *********************************************************************************************************************
/**
 * @brief Menu Inicial
 */
void interface_inicio() {
    char* login = NULL;
    while (1) {
        switch (menu_selection(&(strcol) {.size = 3,
                                          .data = (char*[]) {
                                              "Log in",         // 0
                                              "Salvar Dados",   // 1
                                              "Carregar Dados", // 2
                                          }})) {
            case -1: return;
            case 0:
                printf("(F / DC)");
                login = menu_readNotNulStr();
                if (strcmp(login, "F") == 0) {
                    interface_funcionario();
                } else if (strcmp(login, "DC") == 0) {
                    interface_diretor();
                } else {
                    menu_printError("Log in %s é inválido", login);
                    printf("Inserir \"F\"  para permissões de funcionário\n"
                           "Inserir \"DC\" para permissões de diretor clínico\n");
                }
                freeN(login);
                break;
            case 1: funcional_save(); break;
            case 2: funcional_load(); break;
        }
    }
}

/**
 * @brief   Ponto de entrada do programa, inicia as variáveis globais, chama
 *          interface_inicio e desaloca as globais no final.
 * @returns 0
 */
int main() {
#ifdef DEBUG_BUILD
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("DEBUG BULD\n");
    printf("COMPILADO EM: " __DATE__ " - " __TIME__ "\n");
    printf("COMPILADOR: "
#    if defined(_MSC_VER)
           "VISUAL STUDIO"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(_MSC_VER)
#    elif defined(__clang__)
           "CLANG"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(__clang_major__) "." MACRO_QUOTE(__clang_minor__) "." MACRO_QUOTE(
               __clang_patchlevel__)
#    elif defined(__MINGW32__)
           "MINGW"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(__MINGW32_MAJOR_VERSION) "." MACRO_QUOTE(__MINGW32_MINOR_VERSION)
#    elif defined(__TINYC__)
           "TINY C"
#    elif defined(__llvm__)
           "DESCONHECIDO - LLVM BACKEND"
#    elif defined(__GNUC__) && !defined(__INTEL_COMPILER)
           "GNU C COMPILER"
           " \nVERÇÂO DO COMPILADOR: " MACRO_QUOTE(__GNUC__) "." MACRO_QUOTE(__GNUC_MINOR__)
#    else
           "DESCONHECIDO"
#    endif
               "\n");
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    // TODO: REMOVE, WINDOWS ONLY
    stdin  = (&_iob[STDIN_FILENO]);
    stdout = (&_iob[STDOUT_FILENO]);
    stderr = (&_iob[STDERR_FILENO]);
#endif

    menu_printDiv();
    menu_printHeader("A Iniciar");
    artigos      = artigocol_new();
    encomendas   = encomendacol_new();
    utilizadores = utilizadorcol_new();

    interface_inicio();
    menu_printHeader("A Terminar");

    artigocol_free(&artigos);
    encomendacol_free(&encomendas);
    utilizadorcol_free(&utilizadores);
    menu_printDiv();

    return 0;
}