/**
 * @file    artigo.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Um artigo é um objeto com um nome, preço, stock e informação
 *          relativa ao tipo de medicamento (taxa de IVA e grupo).
 * @version 1
 * @date    2019-12-31
 * @copyright Copyright (c) 2019
 */

#ifndef ARTIGO_H
#define ARTIGO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


// META ??????XX (XX é a taxa do iva a ser aplicada)
// META ?????X?? (X é 1 quando o artigo pretence ao grupo ANIMAL)
// META ????X??? (X é 1 quando o artigo necessita de receita)
// META ???X???? (X é 1 quando o artigo está desativado)
#define ARTIGO_IVA ((uint8_t) 3)
#define ARTIGO_IVA_NORMAL ((uint8_t) 0)
#define ARTIGO_IVA_INTERMEDIO ((uint8_t) 1)
#define ARTIGO_IVA_REDUZIDO ((uint8_t) 2)
#define ARTIGO_IVA_NORMAL_VAL 1.23
#define ARTIGO_IVA_INTERMEDIO_VAL 1.13
#define ARTIGO_IVA_REDUZIDO_VAL 1.06
#define ARTIGO_GRUPO_ANIMAL ((uint8_t) 4)
#define ARTIGO_NECESSITA_RECEITA ((uint8_t) 8)
#define ARTIGO_DESATIVADO ((uint8_t) 16)

/**
 * @brief   Um artigo é um objeto com um nome, peso, e informação relativa ao
 *          tipo de medicamento (taxa de IVA e grupo).
 */
typedef struct {
    char*   nome;       ///< Nome do artigo
    uint8_t meta;       ///< Info sobre o artigo
    int64_t preco_cent; ///< Preçco base do artigo em cêntimos
    int64_t stock;      ///< Stock do artigo

} artigo;

artigo newArtigo();
void   freeArtigo(artigo* const a);
int    save_artigo(FILE* const f, const artigo* const data);
int    load_artigo(FILE* const f, artigo* data);

#endif