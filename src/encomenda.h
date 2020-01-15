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

/**
 * @def ENCOMENDA_TIPO_URGENTE
 *          Define uma encomenda que tem que ser entregada urgentemente.
 * @def ENCOMENDA_TIPO_FRAGIL
 *          Define uma encomenda que é frágil.
 * @def ENCOMENDA_TIPO_PESADO
 *          Define uma encomenda que tem que é pesada.
 * @def ENCOMENDA_TIPO_VOLUMOSO
 *          Define uma encomenda que é volumosa.
 * @def ENCOMENDA_ESTADO_EM_ENTREGA
 *          Define uma encomenda que esta a ser processada mas ainda não foi
 *          expedida.
 * @def ENCOMENDA_ESTADO_EXPEDIDA
 *          Define uma encomenda que está a ser transportada até ao seu destino.
 * @def ENCOMENDA_ESTADO_ENTREGUE
 *          Define uma encomenda que foi entregue ao seu destino.
 * @def ENCOMENDA_ESTADO_CANCELADA
 *          Define uma encomenda que foi cancelada e não será expedida.
 */

#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "artigo.h"
#include "morada.h"
#include "precos_cent.h"
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

#define ENCOMENDA_TIPO_URGENTE ((uint8_t) 1)
#define ENCOMENDA_TIPO_FRAGIL ((uint8_t) 2)
#define ENCOMENDA_TIPO_PESADO ((uint8_t) 4)
#define ENCOMENDA_TIPO_VOLUMOSO ((uint8_t) 8)
#define ENCOMENDA_ESTADO_EM_ENTREGA ((uint8_t) 16)
#define ENCOMENDA_ESTADO_EXPEDIDA ((uint8_t) 32)
#define ENCOMENDA_ESTADO_ENTREGUE ((uint8_t) 64)
#define ENCOMENDA_ESTADO_CANCELADA ((uint8_t) 128)

/**
 * A encomenda percorre uma certa distância e tem diversos estados como
 * ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como ENCOMENDA_TIPO_URGENTE.
 * Adicionalmente a encomenda armazena a tabela de preços ativa quando esta foi
 * criada, a data de criação e o ID do cliente que criou a encomenda.
 */
// https://www.infarmed.pt/documents/15786/17838/Normas_Prescri%C3%A7%C3%A3o/bcd0b378-3b00-4ee0-9104-28d0db0b7872
typedef struct {
    parircol artigos;    ///< Artigos que fazem parte da encomenda.
    uint64_t ID_cliente; ///< ID do cliente que formalizou a encomenda.
} encomenda;

encomenda newEncomenda();
void      freeEncomenda(encomenda* const e);
int       save_encomenda(FILE* const f, const encomenda* const data);
int       load_encomenda(FILE* const f, encomenda* const data);
uint64_t  encomenda_CalcPreco(const encomenda* const e, const artigocol* const a);

#endif