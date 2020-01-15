/**
 * @file    artigo.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Um artigo é um objeto com um nome, peso e volume que pode ou não
 *          requerer tratamento especial.
 * @version 1
 * @date    2019-12-31
 * @copyright Copyright (c) 2019
 */

#include "artigo.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief       Inicializador para artigos.
 * @returns     Um novo artigo válido.
 */
artigo newArtigo() {
    return (artigo) {
        .nome       = strdup("Nome do artigo"), //
        .meta       = ARTIGO_IVA_NORMAL,        //
        .preco_cent = 0,                        //
        .stock      = 0                         //
    };
}

/**
 * @brief       Responsavél por libertar a memória do artigo.
 * @param a     Artigo para ser libertado.
 */
void freeArtigo(artigo* const a) { freeN(a->nome); }

/**
 * @brief       Responsável por salvar um artigo num ficheiro.
 * @param f     Ficheiro onde salvar o artigo.
 * @param data  Artigo para salvar no ficheiro 'f'.
 * @returns     0 se falhou ao salvar o artigo.
 * @returns     1 se slvou o artigo com sucesso.
 */
int save_artigo(FILE* const f, const artigo* const data) {
    int written = 0;
    written += save_str(f, data->nome);
    if (!data->nome) { menu_printInfo("ao gravar artigo - nome inválido."); }
    written += fwrite(&data->meta, sizeof(uint8_t), 1, f);
    written += fwrite(&data->preco_cent, sizeof(uint64_t), 1, f);
    written += fwrite(&data->stock, sizeof(uint64_t), 1, f);
    return written == 4;
}

/**
 * @brief       Responsavél por carregar um artigo de um ficheiro.
 * @param f     Ficheiro de onde careggar o artigo.
 * @param data  Artigo para salvar os dados carregados do ficheiro 'f'.
 * @returns     0 se falhou ao carregar o artigo.
 * @returns     1 se carregou o artigo com sucesso.
 */
int load_artigo(FILE* const f, artigo* const data) {
    int written = 0;
    written += load_str(f, &data->nome);
    if (!data->nome) {
        menu_printInfo("ao carregar artigo - nome inválido.");
        data->nome = strdup("Nome");
    }
    written += fread(&data->meta, sizeof(uint8_t), 1, f);
    written += fread(&data->preco_cent, sizeof(uint64_t), 1, f);
    written += fread(&data->stock, sizeof(uint64_t), 1, f);
    return written == 4;
}
