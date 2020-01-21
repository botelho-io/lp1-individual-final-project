/**
 * @file    encomenda.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma encomenda é um conjunto de artigos transportados de um certo
 *          adereço de origem para um adereço de destino.
 *          A encomenda percorre uma certa distância e tem diversos estados
 *          como ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como
 *          ENCOMENDA_TIPO_URGENTE.
 *          Adicionalmente a encomenda armazena a tabela de preços ativa quando
 *          esta foi criada, a data de criação e o ID do cliente que criou a
 *          encomenda.
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
#include "parIdReceita.h"

#ifndef artigocol_H
#    define artigocol_H
#    define COL_TIPO artigo
#    define COL_NOME artigocol
#    define COL_DEALOC(X) freeArtigo(X)
#    define COL_WRITE(X, F) save_artigo(F, X)
#    define COL_READ(X, F) load_artigo(F, X)
#    include "./colecao.h"
#endif

#ifndef parcol_H
#    define parcol_H
#    define COL_TIPO parIdReceita
#    define COL_NOME parircol
#    define COL_DEALOC(X) free_parIdReceita(X)
#    define COL_WRITE(X, F) save_parIdReceita(F, X)
#    define COL_READ(X, F) load_parIdReceita(F, X)
#    include "./colecao.h"
#endif

/**
 * A encomenda percorre uma certa distância e tem diversos estados como
 * ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como ENCOMENDA_TIPO_URGENTE.
 * Adicionalmente a encomenda armazena a tabela de preços ativa quando esta foi
 * criada, a data de criação e o ID do cliente que criou a encomenda.
 */

typedef struct {
    parircol artigos;    ///< Artigos que fazem parte da encomenda.
    uint64_t ID_cliente; ///< ID do cliente que formalizou a encomenda.
    time_t   tempo;
} encomenda;

encomenda newEncomenda();
void      freeEncomenda(encomenda* const e);
int       save_encomenda(FILE* const f, const encomenda* const data);
int       load_encomenda(FILE* const f, encomenda* const data);
uint64_t  encomenda_CalcPreco(const encomenda* const e, const artigocol* const av);

#endif