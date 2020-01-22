/**
 * @file    compra.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief
 * @version 1
 * @date 2020-01-15
 *
 * @copyright Copyright (c) 2020
 */
//TODO: Update brief

#ifndef PARIDNUM_H
#define PARIDNUM_H

#include <stdint.h>
#include <stdio.h>
#include "utilities.h"

// https://www.infarmed.pt/documents/15786/17838/Normas_Prescri%C3%A7%C3%A3o/bcd0b378-3b00-4ee0-9104-28d0db0b7872
typedef struct {
    int64_t IDartigo;    //< ID do artigo
    int64_t qtd;         //< Quantidade de artigos encomendados
    char     receita[19]; //< Receita do artigo
} compra;

int    save_compra(FILE* const f, const compra* const data);
int    load_compra(FILE* const f, compra* const data);
compra new_compra();

#endif