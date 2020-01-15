/**
 * @file    parIdReceita.h
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

typedef struct {
    uint64_t IDartigo; //< ID do artigo
    char*    receita;  //< Receita do artigo
} parIdReceita;

int          save_parIdReceita(FILE* const f, const parIdReceita* const data);
int          load_parIdReceita(FILE* const f, parIdReceita* const data);
void         free_parIdReceita(parIdReceita* const data);
parIdReceita new_parIdReceita();

#endif