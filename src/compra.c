/**
 * @file    compra.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief
 * @version 1
 * @date 2020-01-15
 *
 * @copyright Copyright (c) 2020
 */
//TODO: Update brief

#include "compra.h"

compra new_compra() {
    return (compra) { .IDartigo = 0 };
}

int save_compra(FILE* const f, const compra* const data) {
    int written = 0;
    written += fwrite(&data->IDartigo, sizeof(uint64_t), 1, f);
    written += fwrite(&data->qtd, sizeof(uint64_t), 1, f);
    written += fwrite(data->receita, 1, 19, f);
    return written == 21;
}

int load_compra(FILE* const f, compra* const data) {
    int written = 0;
    written += fread(&data->IDartigo, sizeof(uint64_t), 1, f);
    written += fread(&data->qtd, sizeof(uint64_t), 1, f);
    written += fread(&data->receita, 1, 12, f);
    return written == 21;
}
