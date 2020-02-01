/**
 * @file    compra.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma compra representa um conjunto do mesmo artigo a ser vendido.
 * @version 1
 * @date 2020-01-15
 *
 * @copyright Copyright (c) 2020
 */

#include "compra.h"

/**
 * @brief   Inicializador para compras.
 * @returns Uma compra válida.
 */
compra new_compra() { return (compra) {.IDartigo = 0}; }

/**
 * @brief      Salva compra em ficheiro.
 * @param f    Ficheiro onde guardar.
 * @param data Dados a guardar.
 * @returns    1 se guardado com sucesso.
 * @returns    0 caso tenha falhado a gravar em ficheiro.
 */
int save_compra(FILE* const f, const compra* const data) {
    int written = 0;
    written += fwrite(&data->IDartigo, sizeof(colSize_t), 1, f);
    written += fwrite(&data->qtd, sizeof(int64_t), 1, f);
    written += fwrite(data->receita, 19, 1, f);
    return written == 3;
}

/**
 * @brief      Carrega compra em ficheiro.
 * @param f    Ficheiro onde carregar.
 * @param data Dados a carregar.
 * @returns    1 se carregado com sucesso.
 * @returns    0 caso tenha falhado a carregar de ficheiro.
 */
int load_compra(FILE* const f, compra* const data) {
    int written = 0;
    written += fread(&data->IDartigo, sizeof(colSize_t), 1, f);
    written += fread(&data->qtd, sizeof(int64_t), 1, f);
    written += fread(&data->receita, 19, 1, f);
    return written == 3;
}
