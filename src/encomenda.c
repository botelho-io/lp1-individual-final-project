/**
 * @file    encomenda.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma encomenda é uma coleção de compras, compradas por um cliene numa
 *          certa data.
 * @version 0.1
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 */

#include "encomenda.h"

#include "menu.h"
#include "utilities.h"

/**
 * @brief           Inicializador para encomendas.
 * @returns         Uma encomenda, válida mas sem artigos.
 */
encomenda newEncomenda() {
    return (encomenda) {
        .compras    = compracol_new(), //
        .ID_cliente = 0,               //
        .tempo      = time(NULL)       //
    };
}

/**
 * @brief           Responsavél por libertar a memória da encomenda.
 * @param e         Encomenda para ser libertado.
 */
void freeEncomenda(encomenda* const e) { compracol_free(&e->compras); }

/**
 * @brief           Calcula o preço de uma encomenda, em cêntimos.
 * @param e         Encomenda cujo preço será calculado.
 * @returns         O preço da encomenda em cêntimos.
 * @returns         0 se a encomenda tiver um código postal inválido.
 * @warning         O tipoEstado da encomenda tem que estar correto antes de
 *                  chamar esta função.
 */
int64_t encomenda_CalcPreco(const encomenda* const e, const artigocol* const av) {
    int64_t precoFinal = 0;
    int64_t precoPreTax;
    artigo* artAtual;
    for (int64_t i = 0; i < e->compras.size; i++) {
        artAtual    = &(av->data[e->compras.data[i].IDartigo]);
        precoPreTax = artAtual->preco_cent;
        switch (artAtual->meta & ARTIGO_IVA) {
            case ARTIGO_IVA_NORMAL: precoPreTax *= ARTIGO_IVA_NORMAL_VAL; break;
            case ARTIGO_IVA_INTERMEDIO: precoPreTax *= ARTIGO_IVA_INTERMEDIO_VAL; break;
            case ARTIGO_IVA_REDUZIDO: precoPreTax *= ARTIGO_IVA_REDUZIDO_VAL; break;
        }
        precoFinal += precoPreTax * (e->compras.data[i].qtd);
    }
    return precoFinal;
}

/**
 * @brief           Responsável por salvar uma encomenda num ficheiro.
 * @param f         Ficheiro onde salvar a encomenda.
 * @param data      Encomenda para salvar no ficheiro 'f'.
 * @returns         0 se falhou ao salvar a encomenda.
 * @returns         1 se slvou a encomenda com sucesso.
 */
int save_encomenda(FILE* const f, const encomenda* const data) {
    // Gravar artigos
    if (!compracol_write(&(data->compras), f)) {
        menu_printError("ao gravar encomenda - compracol_write falhou");
        return 0;
    }
    return fwrite(&data->ID_cliente, sizeof(colSize_t), 1, f);
}

/**
 * @brief           Responsavél por carregar uma encomenda de um ficheiro.
 * @param f         Ficheiro de onde careggar a encomenda.
 * @param data      Encomenda para salvar os dados carregados do ficheiro 'f'.
 * @returns         0 se falhou ao carregar a encomenda.
 * @returns         1 se carregou a encomenda com sucesso.
 */
int load_encomenda(FILE* const f, encomenda* const data) {
    // Carregar artigos
    data->compras = compracol_new();
    if (!compracol_read(&(data->compras), f)) {
        menu_printError("ao carregar encomenda - compracol_read falhou");
        return 0;
    }
    return fread(&data->ID_cliente, sizeof(colSize_t), 1, f);
}
