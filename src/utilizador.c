/**
 * @file    utilizador.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Capaz de gravar todas as informações sobre um utilizador.
 * @version 1
 * @date 2020-01-04
 *
 * @copyright Copyright (c) 2020
 */

#include "utilizador.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief    Responsável por verificar se um número de cartão de cidadão é
 *           válido.
 * @param cc Número de cartão de cidadão para ser validado.
 * @returns  0 se o número de cartão de cidadão é inválido.
 * @returns  1 se o número de cartão de cidadão é válido.
 */
int utilizador_eCCValido(const uint8_t* const cc) {
    int i = 0;

    // Assegurar 9 digitos
    for (; i < 9; ++i)
        if (!isdigit(cc[i])) return 0;

    // Assegurar 2 letras
    for (; i < 11; ++i)
        if (!isalpha(cc[i])) return 0;

    // Asegurar ultimo digito
    if (!isdigit(cc[i])) return 0;

    return 1;
}

/**
 * @brief     Responsável por verificar se um NIF é válido.
 * @param NIF NIF para ser validado.
 * @returns   0 se o NIF é inválido.
 * @returns   1 se o NIF é válido.
 */
int utilizador_eNIFValido(const uint8_t* const NIF) {
    int i = 0;
    // Assegurar 9 digitos
    for (; i < 9; ++i)
        if (!isdigit(NIF[i])) return 0;

    return 1;
}

/**
 * @brief   Responsavél por criar um novo utilizador.
 * @returns Um novo utilizador válido.
 */
utilizador newUtilizador() {
    return (utilizador) {
        .nome = strdup("Nome do cliente"),                                   //
        .NIF  = {'0', '0', '0', '0', '0', '0', '0', '0', '0'},               //
        .CC   = {'0', '0', '0', '0', '0', '0', '0', '0', '0', 'X', 'Y', '0'} //
    };
}

/**
 * @brief   Responsavél por libertar a memória utilizada por um utilizador.
 * @param u O utilizador por libertar.
 */
void freeUtilizador(utilizador* const u) { freeN(u->nome); }

/**
 * @brief       Responsável por salvar um utilizador num ficheiro.
 * @param f     Ficheiro onde salvar o utilizador.
 * @param data  Utilizador para salvar no ficheiro 'f'.
 * @returns     0 se falhou ao salvar o utilizador.
 * @returns     1 se slvou o utilizador com sucesso.
 */
int save_utilizador(FILE* const f, const utilizador* const data) {
    int written = 0;
    written += save_str(f, data->nome);
    if (!data->nome) { menu_printInfo("ao gravar utilizador - nome inválido"); }
    written += fwrite(&data->NIF, sizeof(uint8_t), 9, f);
    written += fwrite(&data->CC, sizeof(uint8_t), 12, f);
    return written == (1 + 9 + 12);
}

/**
 * @brief       Responsavél por carregar um utilizador de um ficheiro.
 * @param f     Ficheiro de onde careggar o utilizador.
 * @param data  Utilizador para salvar os dados carregados do ficheiro 'f'.
 * @returns     0 se falhou ao carregar o utilizador.
 * @returns     1 se carregou o utilizador com sucesso.
 */
int load_utilizador(FILE* const f, utilizador* const data) {
    int written = 0;
    written += load_str(f, &data->nome);
    if (!data->nome) { menu_printInfo("ao carregar utilizador - nome inválido"); }
    written += fread(&data->NIF, sizeof(uint8_t), 9, f);
    written += fread(&data->CC, sizeof(uint8_t), 12, f);
    return written == (1 + 9 + 12);
}