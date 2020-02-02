/**
 * @file    menu.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   API simples para mostrar informação e capturar input do utilizador.
 * @version 1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "menu.h"

#include "utilities.h"

/**
 * @brief Função auxiliar para limpar o buffer de input.
 */
void cleanInputBuffer() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

/**
 * @brief     Remove os espaços extra no início e final da string de input.
 * @param str String para ser reduzida.
 * @returns   Um ponteiro para a string reduzida.
 * @returns   NULL caso a string seja apenas constituida por espaços.
 * @warning   A string de retorno é uma substring da string de input, caso esta
 *            seja dealocada, a substring também será dealocada.
 */
char* subStringTrimWhiteSpace(char* str) {
    if (!str) return NULL;

    // At the beguining
    while (isspace(*str)) str++;
    if (*str == '\0') // All spaces?
        return NULL;

    // At the end
    char* end = &str[strlen(str) - 1];
    while (end > str && isspace(*end)) end--;
    end[1] = '\0';

    return str;
}

/**
 * @brief   Lê uma string do standard input.
 * @returns O ponteiro para uma string sem espaços extra no início ou no fim.
 * @returns NULL caso só tenham sido introduzidos espaços.
 * @warning Pode retornar NULL.
 */
char* menu_readString() {
    const int STEP     = 16;
    size_t    alocated = STEP;
    size_t    size     = 0;
    char*     str;
    protectVarFcnCall(str, malloc(STEP), "alocação de memória recusada");

    int ch;
    printf(" $ ");
    while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
        str[size++] = ch;
        if (size == alocated) {
            char* tmp = realloc(str, alocated += STEP);
            if (!tmp) {
                --size;
                menu_printError("menu_readString - realocação de memória recusada");
                break;
            }
            str = tmp;
        }
    }
    str[size++] = '\0';

    char* trimed = strdup(subStringTrimWhiteSpace(str));
    free(str);
    return trimed;
}

/**
 * @brief   Lê uma string vâlida (não nula e não só espaços), guardando-a em X.
 * @returns Ponteiro para a string lida
 */
char* menu_readNotNulStr() {
    static char* value;
    value = NULL;
    while (!value) { value = menu_readString(); }
    return value;
}

/**
 * @brief   Lê uma float do standard input.
 * @returns Valor lido.
 */
float menu_readFloat() {
    static float value;
    while (1) {
        printf(" $ ");
        if (scanf("%f", &value) != 1) {
            menu_printError("Não foi inserido um número válido");
            cleanInputBuffer();
            continue;
        }
        cleanInputBuffer();
        return value;
    }
}

/**
 * @brief   Lê uma int do standard input.
 * @returns Valor lido.
 */
int menu_readInt() {
    static int value = 1;
    while (1) {
        printf(" $ ");
        if (scanf("%d", &value) != 1) {
            menu_printError("Não foi inserido um número válido");
            cleanInputBuffer();
            continue;
        }
        cleanInputBuffer();
        return value;
    }
}

/**
 * @brief   Lê uma int64_t do standard input.
 * @returns Valor lido.
 */
int64_t menu_readInt64_t() {
    static int64_t value = 1;
    while (1) {
        printf(" $ ");
        if (scanf("%ld", &value) != 1) {
            menu_printError("Não foi inserido um número válido");
            cleanInputBuffer();
            continue;
        }
        cleanInputBuffer();
        return value;
    }
}

/**
 * @brief     Lê uma int do standard input entre [min, max]
 * @param min Valor minimo da int para ser lida.
 * @param max Valor maximo da int para ser lida.
 * @returns   Valor lido
 */
int menu_readIntMinMax(const int min, const int max) {
    static int value;
    printf("Insira um numero entre [%d e %d]", min, max);
    while (1) {
        value = menu_readInt();
        if (value >= min) {
            if (value <= max) {
                return value;
            } else
                menu_printError("[%d] é maior que [%d]", value, max);
        } else
            menu_printError("[%d] é menor que [%d]", value, min);
    }
}

/**
 * @brief     Lê uma int64_t do standard input entre [min, max]
 * @param min Valor minimo da int para ser lida.
 * @param max Valor maximo da int para ser lida.
 * @returns   Valor lido
 */
int64_t menu_readInt64_tMinMax(const int64_t min, const int64_t max) {
    static int64_t value;
    printf("Insira um numero entre [%ld e %ld]", min, max);
    while (1) {
        value = menu_readInt64_t();
        if (value >= min) {
            if (value <= max) {
                return value;
            } else
                menu_printError("[%ld] é maior que [%ld]", value, max);
        } else
            menu_printError("[%ld] é menor que [%ld]", value, min);
    }
}

/**
 * @brief       Imprime um menu deixando o utilizador selecionar uma das opções.
 * @param itens Coleção se strings que constituem as opções que o utilizador
 *              pode selecionar.
 * @returns     Um valor entre [-1, itens.size[ correspondente à opção
 *              selecionada ou "Sair" para -1
 */
int64_t menu_selection(const strcol* const itens) {
    int64_t op = -2;
    int64_t max;
    menu_printDiv();
    while (op == -2) {
        printf("   Opção      |   Item\n");
        printf("         -2   |   Reimprimir\n");
        printf("         -1   |   Sair\n");
        max = 0;
        strcol_iterateFW((strcol*) itens, (strcol_pred_t) &printItemVP, &max);
        op = menu_readInt64_tMinMax(-2, max - 1);
    }
    return op;
}

/**
 * @brief   Premite ao utilizador introduzir uma resmosta de sim ou não.
 * @details Premite ao utilizador introduzir um character e compara-o com os
 *          caracteres correspondentes às resmostas de sim e não, sem tomar em
 *          conta a capitalização.
 * @param Y Caracter correspondente ao sim
 * @param N Caracter correspendente ao nao
 * @returns 0 se o utilizador introduziu 'N'
 * @returns 1 se o utilizador introduziu 'S'
 */
int menu_YN(const char Y, const char N) {
    int i;
    while (1) {
        printf(" $ ");
        i = toupper(getchar());
        cleanInputBuffer();
        if (i == toupper(Y)) {
            return 1;
        } else if (i == toupper(N)) {
            return 0;
        } else
            menu_printError("tem que inserir [%c] ou [%c]", Y, N);
    }
}

/**
 * @brief Imprime uma divisória.
 */
void menu_printDiv() { printf("--------------------------------------------------------------------------------\n"); }

/**
 * @brief     Utilizado para imprimir erros.
 * @param err O formato do erro.
 * @param ... Argumentos extra.
 */
void menu_printError(const char* const err, ...) {
    va_list args;
    printf("*** ERRO: ");
    va_start(args, err);
    vprintf(err, args);
    va_end(args);
    printf("\n");
}

/**
 * @brief      Utilizado para imprimir informação.
 * @param info O formato da informação.
 * @param ...  Argumentos extra.
 */
void menu_printInfo(const char* const info, ...) {
    va_list args;
    printf("*** INFO: ");
    va_start(args, info);
    vprintf(info, args);
    va_end(args);
    printf("\n");
}

/**
 * @brief        Utilizado para imprimir um título.
 * @param header String para o header.
 */
void menu_printHeader(const char* header) {
    if (!header) header = "";
    int spacesize = (80 - strlen(header)) - 8;
    if (spacesize < 3) spacesize = 3;
    for (int i = 0; i < spacesize / 2; i++) printf(" ");
    printf("*** ");
    printf("%.66s", header);
    printf(" ***\n");
};

/**
 * @brief    Imprime informação breve sobre a encomenda.
 * @param e  Encomenda a ser impressa.
 * @param uv Coleção de clientes ao qual o ID de utilizador da encomenda faz
 *           referência.
 * @param av Coleção de artigos ao qual o ID dos artigos na encomenda faz
 *           referência.
 */
void menu_printEncomendaBrief(const encomenda* const e, const utilizadorcol* const uv, const artigocol* const av) {

    struct tm* lt = localtime(&e->tempo);
    printf("Cliente: %s NIF:(%.9s) Data: %d/%d/%d %d:%d  -  TOTAL: %ldc",
           protectStr(uv->data[e->ID_cliente].nome), //
           uv->data[e->ID_cliente].NIF,              //
           1900 + lt->tm_year,                       //
           1 + lt->tm_mon,                           //
           lt->tm_mday,                              //
           lt->tm_hour,                              //
           lt->tm_min,                               //
           encomenda_CalcPreco(e, av)                //
    );
}

/**
 * @brief   Imprime informação sobre o utilizador.
 * @param u Utilizador para ser impresso.
 */
void menu_printUtilizador(const utilizador u) {
    printf("NIF: %.9s CC: %12.12s Nome: %s", u.NIF, u.CC, protectStr(u.nome));
}

/**
 * @brief   Imprime informação sobre o artigo.
 * @param a Artigo para ser impresso.
 */
void menu_printArtigo(const artigo* const a) {
    const char* iva;
    switch (a->meta & ARTIGO_IVA) {
        case ARTIGO_IVA_NORMAL: iva = "normal"; break;
        case ARTIGO_IVA_REDUZIDO: iva = "reduzido"; break;
        default: iva = "intermédio"; break;
    }

    printf("%s%s -  Preço: %ldc + (IVA %s)  -  Grupo %s %s",
           (a->meta & ARTIGO_DESATIVADO) ? "[ DESATIVADO ]" : "",                      //
           protectStr(a->nome),                                                        //
           a->preco_cent,                                                              //
           iva,                                                                        //
           (a->meta & ARTIGO_GRUPO_ANIMAL) ? "animal" : "humano",                      //
           (a->meta & ARTIGO_NECESSITA_RECEITA) ? "receita necessária" : "venda livre" //
    );
}

/**
 * @brief   Imprime informação sobre uma compra.
 * @param c Compra a ser impressa.
 */
void menu_printCompra(const compra* const c, const artigocol* const av) {
    printf("QTD: %lu  |  ", c->qtd);
    menu_printArtigo(&(av->data[c->IDartigo]));
}

/**
 * @brief   Imprime informação sobre o artigo e o stcok.
 * @param a Artigo para ser impresso.
 */
void menu_printArtigoStock(const artigo* const a) {
    const char* iva;
    switch (a->meta & ARTIGO_IVA) {
        case ARTIGO_IVA_NORMAL: iva = "normal"; break;
        case ARTIGO_IVA_REDUZIDO: iva = "reduzido"; break;
        default: iva = "intermédio"; break;
    }

    printf("%s (%ld em stock) -  Preço: %ldc + (IVA %s)  -  Grupo %s %s %s",
           protectStr(a->nome),                                                         //
           a->stock,                                                                    //
           a->preco_cent,                                                               //
           iva,                                                                         //
           (a->meta & ARTIGO_GRUPO_ANIMAL) ? "animal" : "humano",                       //
           (a->meta & ARTIGO_NECESSITA_RECEITA) ? "venda livre" : "receita necessária", //
           (a->meta & ARTIGO_DESATIVADO) ? "DESATIVADO" : ""                            //
    );
}

/**
 * @brief    Imprime informação detalhada sobre a encomenda.
 * @param e  Encomenda a ser impressa.
 * @param uv Coleção de clientes ao qual o ID de utilizador da encomenda faz
 *           referência.
 * @param av Coleção de artigos ao qual o ID dos artigos na encomenda faz
 *           referência.
 */
void menu_printEncomendaDetail(const encomenda* const e, const utilizadorcol* const uv, const artigocol* const av) {
    menu_printDiv();
    menu_printDiv();
    menu_printHeader("Recibo de Encomenda");
    printf("*** NIF do Cliente: %.9s\n", uv->data[e->ID_cliente].NIF);
    printf("*** Número de CC: %.12s\n", uv->data[e->ID_cliente].CC);

    struct tm* lt = localtime(&e->tempo);
    printf("Data: %d/%d/%d %d:%d\n\n", 1900 + lt->tm_year, 1 + lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min);

    menu_printHeader("Artigos");
    printf("*       ID | NOME                                | Preço    | + IVA    | Receita\n");
    int64_t precoSemIva = 0;
    int64_t precoComIva = 0;
    for (size_t i = 0; i < e->compras.size; ++i) {
        const artigo* const a = &av->data[e->compras.data[i].IDartigo];
        double              iva;
        switch (a->meta & ARTIGO_IVA) {
            case ARTIGO_IVA_NORMAL: iva = ARTIGO_IVA_NORMAL_VAL; break;
            case ARTIGO_IVA_REDUZIDO: iva = ARTIGO_IVA_REDUZIDO_VAL; break;
            default: iva = ARTIGO_IVA_INTERMEDIO_VAL; break;
        }
        precoSemIva += a->preco_cent * e->compras.data[i].qtd;
        precoComIva += precoSemIva * iva;

        printf("* %8lu | QTD: %lu | %30.30s | %8lu | %8lu | ",
               i,                             //
               e->compras.data[i].qtd,        //
               protectStr(a->nome),           //
               a->preco_cent,                 //
               (int64_t)(a->preco_cent * iva) //
        );

        if (a->meta & ARTIGO_NECESSITA_RECEITA) { printf("Receita: %19.19s\n", e->compras.data[i].receita); }
    }

    printf("\n*** Preço líquido: %ldc\n", precoSemIva);
    printf("\n*** Preço com IVA: %ldc\n", precoComIva);
    menu_printDiv();
}