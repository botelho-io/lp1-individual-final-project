/**
 * @file    encomenda.c
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Uma encomenda é um conjunto de artigos transportados de um certo
 *          adereço de origem para um adereço de destino.
 *          A encomenda percorre uma certa distância e tem diversos estados
 *          como ENCOMENDA_ESTADO_EM_ENTREGA ou tipos como
 *          ENCOMENDA_TIPO_URGENTE.
 *          Adicionalmente a encomenda armazena a tabela de preços ativa quando
 *          esta foi criada, a data de criação e o ID do cliente que criou a
 *          encomenda.
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
        .artigos    = parircol_new(), //
        .ID_cliente = 0,              //
        .tempo      = time(NULL)      //
    };
}

/**
 * @brief           Responsavél por libertar a memória da encomenda.
 * @param e         Encomenda para ser libertado.
 */
void freeEncomenda(encomenda* const e) { parircol_free(&e->artigos); }

/**
 * @brief           Calcula o preço de uma encomenda, em cêntimos.
 * @param e         Encomenda cujo preço será calculado.
 * @returns         O preço da encomenda em cêntimos.
 * @returns         0 se a encomenda tiver um código postal inválido.
 * @warning         O tipoEstado da encomenda tem que estar correto antes de
 *                  chamar esta função.
 */
uint64_t encomenda_CalcPreco(const encomenda* const e, const artigocol* const av) {
    uint64_t precoFinal;
    uint64_t precoPreTax;
    artigo*  artAtual;
    for (uint64_t i = 0; i < e->artigos.size; i++) {
        artAtual    = &(av->data[e->artigos.data[i].IDartigo]);
        precoPreTax = artAtual->preco_cent;
        switch (artAtual->meta & ARTIGO_IVA) {
            case ARTIGO_IVA_NORMAL: precoPreTax *= ARTIGO_IVA_NORMAL_VAL; break;
            case ARTIGO_IVA_INTERMEDIO: precoPreTax *= ARTIGO_IVA_INTERMEDIO_VAL; break;
            case ARTIGO_IVA_REDUZIDO: precoPreTax *= ARTIGO_IVA_REDUZIDO_VAL; break;
        }
        precoFinal += precoPreTax;
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
    if (!parircol_write(&(data->artigos), f)) {
        menu_printError("ao gravar encomenda - parircol_write falhou.");
        return 0;
    }
    return fwrite(&data->ID_cliente, sizeof(uint64_t), 1, f);
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
    data->artigos = parircol_new();
    if (!parircol_read(&(data->artigos), f)) {
        menu_printError("ao carregar encomenda - parircol_read falhou.");
        return 0;
    }
    return fread(&data->ID_cliente, sizeof(uint64_t), 1, f);
}
