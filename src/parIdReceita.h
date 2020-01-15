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