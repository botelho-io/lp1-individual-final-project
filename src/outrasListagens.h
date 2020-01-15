/**
 * @file    outrasListagens.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   As 5 listagens extra propostas pelos alunos.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef OUTRASLISTAGENS_H
#define OUTRASLISTAGENS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "artigo.h"
#include "encomenda.h"
#include "utilizador.h"

#ifndef artigocol_H
#    define artigocol_H
#    define COL_TIPO artigo
#    define COL_NOME artigocol
#    define COL_DEALOC(X) freeArtigo(X)
#    define COL_WRITE(X, F) save_artigo(F, X)
#    define COL_READ(X, F) load_artigo(F, X)
#    include "./colecao.h"
#endif

#ifndef encomendacol_H
#    define encomendacol_H
#    define COL_TIPO encomenda
#    define COL_NOME encomendacol
#    define COL_DEALOC(X) freeEncomenda(X)
#    define COL_WRITE(X, F) save_encomenda(F, X)
#    define COL_READ(X, F) load_encomenda(F, X)
#    include "./colecao.h"
#endif

#ifndef utilizadorcol_H
#    define utilizadorcol_H
#    define COL_TIPO utilizador
#    define COL_NOME utilizadorcol
#    define COL_DEALOC(X) freeUtilizador(X)
#    define COL_WRITE(X, F) save_utilizador(F, X)
#    define COL_READ(X, F) load_utilizador(F, X)
#    include "./colecao.h"
#endif

artigocol     artigos;
encomendacol  encomendas;
utilizadorcol utilizadores;
uint8_t       utilizadorAtual;
void          funcional_consultar_estados_encomendas();

#endif