/**
 * @file    utilizador.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Capaz de gravar todas as informações sobre um utilizador.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#ifndef UTILIZADOR_H
#define UTILIZADOR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief   Capaz de gravar todas as informações sobre um utilizador.
 */
typedef struct {
    char* nome;   ///< Nome do cliente.
    char  NIF[9]; ///< NIF do cliente.
    char  CC[12]; ///< Número de cartão de cidadão do cliente.
} utilizador;

int        utilizador_eCCValido(const char* const CC);
utilizador newUtilizador();
void       freeUtilizador(utilizador* const u);
int        save_utilizador(FILE* const f, const utilizador* const data);
int        load_utilizador(FILE* const f, utilizador* const data);

#endif
