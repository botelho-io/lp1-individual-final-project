/**
 * @file    encomenda.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma encomenda é uma coleção de compras, compradas por um cliene numa
 *          certa data.
 * @version 1
 * @date 2019-12-31
 *
 * @copyright Copyright (c) 2019
 */

#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "artigo.h"
#include "compra.h"

#ifndef artigocol_H
#    define artigocol_H
#    define COL_TIPO artigo
#    define COL_NOME artigocol
#    define COL_DEALOC(X) freeArtigo(X)
#    define COL_WRITE(X, F) save_artigo(F, X)
#    define COL_READ(X, F) load_artigo(F, X)
#    include "colecao.h"
#endif

#ifndef compracol_H
#    define compracol_H
#    define COL_TIPO compra
#    define COL_NOME compracol
#    define COL_WRITE(X, F) save_compra(F, X)
#    define COL_READ(X, F) load_compra(F, X)
#    include "colecao.h"
#endif

/**
 * @brief   Uma encomenda é uma coleção de compras, compradas por um cliene numa
 *          certa data.
 */
typedef struct {
    compracol compras;    ///< Compras que fazem parte da encomenda.
    colSize_t ID_cliente; ///< ID do cliente que formalizou a encomenda.
    time_t    tempo;      ///< Data de criação da encomenda
} encomenda;

encomenda newEncomenda();
void      freeEncomenda(encomenda* const e);
int       save_encomenda(FILE* const f, const encomenda* const data);
int       load_encomenda(FILE* const f, encomenda* const data);
uint64_t  encomenda_CalcPreco(const encomenda* const e, const artigocol* const av);

#endif