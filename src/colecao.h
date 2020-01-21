/**
 * @file    colecao.h
 * @author  André Botelho (keyoted@gmail.com)
 * @brief   Protótipo para uma coleção, um objeto que contém um numero variável
 *          de objetos e que é capaz de alocar memoria dinâmicamente e de
 *          dealocar memória quando necessáro. Este ficheiro contém apenas um
 *          'protótipo' que é compilado para um objeto real quando os macros
 *          COL_TIPO; COL_NOME e COL_DEALOC(X) são definidos antes de incluir o
 *          ficheiro. COL_IMPLEMENTACAO e COL_DECLARACAO terão também que ser
 *          definidos (mesmo que sem nenhum valor) de modo a ativar a
 *          implementação e a declaração das funções que dizem respeito à
 *          coleção que está a ser criado. Por omição da defenição de ambos os
 *          macros, COL_DECLARACAO é definido automaticamente. COL_WRITE(X) e
 *          COL_READ(X) podem também ser definidos de modo a ativar as
 *          funcionalidades de gravar e carregar dados.
 * @version 1
 * @date    2019-12-04
 *
 * @copyright Copyright (c) 2019
 * @section example_sec Exemplo
 * @code{c}
 * #ifndef  strcol_H
 * #define  strcol_H
 * #define  COL_TIPO             char*
 * #define  COL_NOME             strcol
 * #define  COL_DEALOC(X)        free(X)
 * #include "./colecao.h"
 * #endif
 *
 * #ifndef  intcol_H
 * #define  intcol_H
 * #define  COL_TIPO             int
 * #define  COL_NOME             intcol
 * #define  COL_WRITE(X, F)      fwrite(X, sizeof(int), 1, F)
 * #define  COL_READ(X, F)       fread (X, sizeof(int), 1, F)
 * #include "./colecao.h"
 * #endif
 * @endcode
 */

/**
 * @def COL_TIPO
 *                  O tipo de dados guardado na coleção.
 * @def COL_NOME
 *                  Nome da nova struct, todas as funções têm este nome como
 *                  prefixo.
 * @def COL_DEALOC(X)
 *                  Maneira de dealocar COL_TIPO, se necessário
 *                  (pode não ser definido). X é do tipo 'COL_TIPO*'.
 * @def PASTER(X, Y)
 *                  Macro auxiliar para juntar COL_NOME e Y.
 * @def EVAL(X, Y)
 *                  Macro auxiliar para avaliar COL_NOME de modo a poder
 *                  juntá-lo com o nome da função.
 * @def COL_FUN(X)
 *                  Macro responsavél por adicionar COL_NOME antes do nome de
 *                  cada função como prefixo.
 * @def COL_IMPLEMENTACAO
 *                  Se este macro estiver definido as implementações das funções
 *                  da coleção são ativadas. COL_DECLARACAO é definida por
 *                  omição de COL_IMPLEMENTACAO e COL_DECLARACAO.
 * @def COL_DECLARACAO
 *                  Se este macro estiver definido as declarações das funções do
 *                  coleção são ativadas. COL_DECLARACAO é definida por omição
 *                  de COL_IMPLEMENTACAO e COL_DECLARACAO.
 * @def INVAL_INDEX
 *                  O valor máximo que o tamanho da coleção pode ter.
 * @def COL_WRITE(X, F)
 *                  Função para escrever o tipo 'COL_TIPO' num ficheiro. Onde X
 *                  é do tipo 'COL_TIPO*' e F é do tipo FILE* e corresponde ao
 *                  ficheiro onde gravar o objeto. Caso não esteja definido, a
 *                  função _write não vai ser gerada.
 * @def COL_READ(X, F)
 *                  Função para carregar o tipo 'COL_TIPO' num ficheiro. Onde X
 *                  é do tipo 'COL_TIPO*' e F é do tipo FILE* e corresponde ao
 *                  ficheiro onde gravar o objeto. Caso não esteja definido, a
 *                  função _read não vai ser gerada.
 */

#include <inttypes.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef COL_TIPO
#    define COL_TIPO int
#endif

#ifndef COL_NOME
#    define COL_NOME coleção
#endif

#define PASTER(X, Y) X##Y
#define EVAL(X, Y) PASTER(X, Y)
#define COL_FUN(X) EVAL(COL_NOME, X)

#if !(defined(COL_IMPLEMENTACAO) || defined(COL_DECLARACAO))
#    define COL_DECLARACAO
#endif

#define INVAL_INDEX ~((uint64_t) 0)

/**
 * @struct          COL_NOME
 * @brief           Struct com o nome COL_NOME que contém o tipo de dados
 *                  COL_TIPO e informação sobre o numero de objetos guardados,
 *                  pode guardar [0, INVAL_INDEX[ elemntos.
 */
typedef struct {
    uint64_t  alocated; ///< Tamanho alocado de objetos.
    uint64_t  size;     ///< Tamanho de objetos que foi populado.
    COL_TIPO* data;     ///< Começa em data[0] e acaba em data[size-1].
} COL_NOME;
typedef int (*COL_FUN(_pred_t))(COL_TIPO*, void*);

#ifdef COL_IMPLEMENTACAO
/**
 * @brief           Aumenta v, se necessário e possivél, para que se possa
 *                  adicionar mais objetos no final.
 * @param v         coleção a que queremos adicionar espaço.
 * @returns         0 se tentou mas não conseguiu alocar memória.
 * @returns         1 se alocou memória.
 * @returns         2 se não necessitou de alocar memoria.
 */
int COL_FUN(_addCell)(COL_NOME* const v) {
    if (v->alocated == 0) {
        // coleção vazia
        v->data = malloc(sizeof(COL_TIPO) * 8);
        if (v->data == NULL) return 0;
        v->alocated = 8;
        return 1;
    } else if (v->size == v->alocated) {
        // A coleção está cheio e necessita de ser redimensionado
        void* newData = realloc(v->data, sizeof(COL_TIPO) * (v->alocated * 2));
        if (newData == NULL) {
            // Foi impossivel realocar memoria sufuciente
            // Tentar realocar apenas mais um espaço
            newData = realloc(v->data, sizeof(COL_TIPO) * (v->alocated + 1));
            if (newData == NULL) {
                // Impossivel alocar mais memoria
                return 0;
            } else {
                // Memoria alocada, fazer o update da coleção
                v->alocated += 1;
                v->data = newData;
                return 1;
            }
        } else {
            // Memoria alocada, fazer o update da coleção
            v->alocated *= 2;
            v->data = newData;
            return 1;
        }
    } else {
        // coleção ainda tem espaço livre
        return 2;
    }
}

/**
 * @brief           Constrotor da coleção.
 * @details         Constroi uma coleção vazia.
 * @return          Uma coleção vazia.
 */
COL_NOME COL_FUN(_new)() { return (COL_NOME) {.size = 0, .alocated = 0, .data = NULL}; }

/**
 * @brief           Adiciona um objeto no final da coleção.
 * @param v         Ponteiro para a coleção sob o qual operar.
 * @param newObj    Objeto para adicionar à coleção.
 * @returns         0 se não conseguiu inserir o objeto.
 * @returns         1 se conseguiu inserir o objeto.
 */
int COL_FUN(_push)(COL_NOME* const v, COL_TIPO const newObj) {
    if (!COL_FUN(_addCell)(v)) return 0;
    // Aqui está garantido que existe um espaço alocado e livre na coleção
    v->data[v->size] = newObj;
    ++(v->size);
    return 1;
}

/**
 * @brief           Move todos os elementos acima de 'i' um espaço para baixo,
 *                  escrevendo sobre 'i'.
 * @details         Todos os elementos com indexes maiores que 'i' são movidos
 *                  um index para baixo, escrevendo por cima do elemento no
 *                  index 'i' e diminuindo o tamanho de 'v' por uma unidade.
 * @param v         Ponteiro da coleção sob o qual operar.
 * @param i         Index do objeto que será escrito por cima.
 * @warning         O objecto no index 'i' não é dealocado por esta função
 */
void COL_FUN(_moveBelow)(COL_NOME* const v, const uint64_t i) {
    memmove(&v->data[i], &v->data[i + 1], (v->size - i - 1) * sizeof(COL_TIPO));
    --(v->size);
}

/**
 * @brief           Move todos os elementos acima de i e i para cima, criando um
 *                  espaço livre no index i.
 * @details         Todos os elementos com index igual ou superior a 'i' serão
 *                  movidos um index acima, aumentando o tamanho de 'v' por uma
 *                  unidade e deixando um espaço livre no index 'i'. A coleção
 *                  pode ter que ser realocado durante este processo.
 * @param v         Ponteiro da coleção sob o qual operar.
 * @param i         Index do espaço livre após chamar a função.
 * @returns         0 se não conseguiu realizar a operação.
 * @returns         1 se conseguiu realizar a operação.
 * @warning         Após chamar esta função, o objeto no index i pode não ser
 *                  válido.
 */
int COL_FUN(_moveAbove)(COL_NOME* const v, const uint64_t i) {
    if (!COL_FUN(_addCell)(v)) return 0;
    memmove(&v->data[i + 1], &v->data[i], (v->size - i) * sizeof(COL_TIPO));
    ++(v->size);
    return 1;
}

// Adiciona newElement na posição position da coleção
// Retorna 1 em sucesso, 0 de outro modo
/**
 * @brief          Adiciona um objeto na coleção.
 * @param v        Ponteiro para a coleção sob o qual operar.
 * @param newObj   Objeto para adicionar à coleção.
 * @param position Index da coleção onde adicionar o objeto.
 * @note           'position' terá que ser menor ou igual ao tamanho da coleção.
 * @returns        0 se não conseguiu inserir o objeto.
 * @returns        1 se conseguiu inserir o objeto.
 */
int COL_FUN(_pushAt)(COL_NOME* const v, COL_TIPO const newObj, const uint64_t position) {
    if (!COL_FUN(_moveAbove)(v, position)) return 0;
    v->data[position] = newObj;
    return 1;
}

/**
 * @brief           Retorna e remove o último objeto da coleção.
 * @details         Retorna o o último objeto da coleção, removendo-o, mas sem o
 *                  dealocar.
 * @param v         coleção sob o qual operar.
 * @returns         O o último objeto da coleção.
 * @warning         O o último objeto da coleção é removido e não é dealocado,
 *                  terá que ser dealocado posteriormente.
 */
COL_TIPO COL_FUN(_pop)(COL_NOME* const v) {
    COL_TIPO toReturn = v->data[v->size - 1];
    --(v->size);
    return toReturn;
}

/**
 * @brief           Retorna e remove um objeto da coleção.
 * @details         Retorna o objeto com o index 'position' da coleção,
 *                  removendo-o, mas sem o dealocar.
 * @param v         coleção sob o qual operar.
 * @param position  Index do objeto que será removido da coleção.
 * @returns         O objeto com o index 'position'
 * @warning         O objeto com o index 'position' é removido da coleção e não
 *                  é dealocado, terá que ser dealocado posteriormente.
 */
COL_TIPO COL_FUN(_popAt)(COL_NOME* const v, const uint64_t position) {
    COL_TIPO toReturn = v->data[position];
    COL_FUN(_moveBelow)(v, position);
    return toReturn;
}

/**
 * @brief           Apaga e dealoca a coleção.
 * @details         Dealoca todos os elementos da coleção utilizando o macto
 *                  'COL_DEALOC', e dealoca a coleção em si, apagando-o.
 * @param v         Pointeiro para a coleção sob o qual operar.
 * @note            'v' em si não é dealocado.
 */
void COL_FUN(_free)(COL_NOME* const v) {
#    ifdef COL_DEALOC
    for (uint64_t i = 0; i < v->size; i++) { COL_DEALOC(&(v->data[i])); }
#    endif
    if (v->alocated != 0) free(v->data);
    v->data     = NULL;
    v->size     = 0;
    v->alocated = 0;
}

/**
 * @brief           Remove um objeto da coleção.
 * @details         Remove, e dealoca o objeto na posição 'position' da coleção,
 *                  diminuindo o seu tamanho por uma unidade.
 * @param v         Pointeiro para a coleção sob o qual operar.
 * @param position  Index do elemento a remover.
 */
void COL_FUN(_removeAt)(COL_NOME* const v, uint64_t position) {
#    ifdef COL_DEALOC
    COL_DEALOC(&(v->data[position]));
#    endif
    COL_FUN(_moveBelow)(v, position);
}

/**
 * @brief           Ajusta a coleção para que tenha apenas o tamanho necessario
 *                  para guardar o seu conteudo.
 * @details         Ajusta a alocação da coleção para que esta seja igual ao seu
 *                  tamanho.
 * @param v         Pointeiro para a coleção sob o qual operar.
 * @returns         0 se falhou a redimensionar.
 * @returns         1 se redimensionou.
 * @returns         2 se não tiver que redimensionar.
 */
int COL_FUN(_adjust)(COL_NOME* const v) {
    if (v->size == v->alocated) return 2;
    COL_TIPO* newData = realloc(v->data, v->size * sizeof(COL_TIPO));
    if (newData == NULL) return 0;
    v->data     = newData;
    v->alocated = v->size;
    return 1;
}

/**
 * @brief           Aplica a função 'predicate' a todos os elementos da coleção
 *                  'v', do menor ao maior index.
 * @details         A coleção 'v' é iterado, do primeiro ao último elemeto,
 *                  aplicando 'predicate' a todos os elementos até que a coleção
 *                  acabe ou até que 'predicate' retorne verdadeiro.
 * @param v         Pointeiro para a coleção sob o qual operar.
 * @param predicate Função que é chamada por cada elemento da coleção, se esta
 *                  função retornar verdadeiro a iteração pára. Os argumentos
 *                  desta função são: (1) o elemento que está a ser iterado;
 *                  (2) um ponteiro do tipo 'void*' que é passado ao chamar
 *                  _iterateFW.
 * @param userData  Dados passados pelo utilizador à função 'predicate'.
 * @returns         O index do objeto cuja função predicate primeiro returnou
 *                  verdade.
 * @returns         INVAL_INDEX caso a todos os elementos foram iterados sem que
 *                  'predicate' tenha retornado 0.
 */
uint64_t COL_FUN(_iterateFW)(COL_NOME* const v, COL_FUN(_pred_t) predicate, void* userData) {
    for (uint64_t i = 0; i < v->size; i++) {
        if (predicate(&(v->data[i]), userData)) return i;
    }
    return INVAL_INDEX;
}

/**
 * @brief           Aplica a função 'predicate' a todos os elementos da coleção
 *                  'v', do maior ao menor index.
 * @details         A coleção 'v' é iterado, do último ao primeiro elemeto,
 *                  aplicando 'predicate' a todos os elementos até que a coleção
 *                  acabe ou até que 'predicate' retorne verdadeiro.
 * @param v         Pointeiro para a coleção sob o qual operar.
 * @param predicate Função que é chamada por cada elemento da coleção, se esta
 *                  função retornar verdadeiro a iteração pára. Os argumentos
 *                  desta função são: (1) o elemento que está a ser iterado;
 *                  (2) um ponteiro do tipo 'void*' que é passado ao chamar
 *                  _iterateBW.
 * @param userData  Dados passados pelo utilizador à função 'predicate'.
 * @returns         O index do objeto cuja função predicate primeiro returnou
 *                  verdade.
 * @returns         INVAL_INDEX caso a todos os elementos foram iterados sem que
 *                  'predicate' tenha retornado 0.
 */
uint64_t COL_FUN(_iterateBW)(COL_NOME* const v, COL_FUN(_pred_t) predicate, void* userData) {
    for (uint64_t i = v->size - 1; i != INVAL_INDEX; i--) {
        if (predicate(&(v->data[i]), userData)) return i;
    }
    return INVAL_INDEX;
}

/**
 * @brief           Reserva espaço na coleção 'v'.
 * @details         Garante que a coleção 'v' tem no mínimo 'space' celulas de
 *                  memoria alocadas. Esta função é util quando queremos
 *                  adicionar um certo numero de objetos à coleção sem ter que o
 *                  realocar múltiplas vezes.
 * @param v         Ponteiro para a coleção sob o qual queremos operar.
 * @param space     Numero de células de memória que queremos alocar.
 * @returns         0 se tentou mas não conseguiu alocar memória.
 * @returns         1 se alocou memoria.
 * @returns         2 se não o necessiou de alocar memória.
 * @warning         No final de chamar esta função o valor de células de memória
 *                  alocadas pode ser maior do que 'space'.
 */
int COL_FUN(_reserve)(COL_NOME* const v, uint64_t space) {
    if (v->alocated >= space) return 2;
    void* newData = realloc(v->data, sizeof(COL_TIPO) * (space));
    if (newData == NULL) return 0;
    // Memoria alocada, fazer o update da coleção
    v->alocated = space;
    v->data     = newData;
    return 1;
}

/**
 * @brief           Wrapper para que o macro 'COL_DEALOC' possa ser chamado após
 *                  a inclusão do ficheiro colecao.h.
 * @param X         O parametro X do macro 'COL_DEALOC'.
 */
#    ifdef COL_DEALOC
void COL_FUN(_DEALOC)(COL_TIPO* const X) { COL_DEALOC(X); }
#    endif

#    ifdef COL_WRITE
/**
 * @brief           Escreve num ficheiro utilizando o macro 'COL_WRITE'.
 * @details         Escreve um número de 64 bits a indicar o tamanho da coleção
 *                  e de seguida escreve, um a um, utilizando o macro
 *                  'COL_WRITE' os objetos da coleção.
 * @param v         Ponteiro para a coleção sob o qual queremos operar.
 * @param f         Ficheiro onde escrever os conteudos da coleção.
 * @returns         1 se escreveu a coleção com sucesso.
 * @returns         0 caso contrário.
 */
int COL_FUN(_write)(const COL_NOME* const v, FILE* f) {
    // Gravar tamanho da coleção
    if (!fwrite(&v->size, sizeof(uint64_t), 1, f)) return 0;
    // Guardar objetos
    for (uint64_t i = 0; i < v->size; i++) {
        if (!COL_WRITE(&(v->data[i]), f)) return 0;
    }
    return 1;
}
#    endif

#    ifdef COL_READ
/**
 * @brief           Lê de num ficheiro utilizando o macro 'COL_READ'.
 * @details         Lê um número de 64 bits a indicar o tamanho da coleção e
 *                  de seguida lê, um a um, utilizando o macro 'COL_READ' os
 *                  objetos da coleção. Aloca espaço, se necessário.
 * @param v         Ponteiro para a coleção sob o qual queremos operar.
 * @param f         Ficheiro onde escrever os conteudos da coleção.
 * @returns         1 se leu a coleção com sucesso.
 * @returns         0 caso contrário.
 */
int COL_FUN(_read)(COL_NOME* const v, FILE* f) {
    // Ler tamanho de coleção em ficheiro
    uint64_t size = 0;
    if (!fread(&size, sizeof(uint64_t), 1, f)) return 0;
    // Reservar espaço para coleção
    if (!COL_FUN(_reserve)(v, size)) return 0;
    // Ler objetos do ficheiro
    for (uint64_t i = 0; i < size; i++) {
        if (!COL_READ(&(v->data[i]), f)) return 0;
        v->size++;
    }
    return 1;
}
#    endif
#endif

#ifdef COL_DECLARACAO
COL_NOME COL_FUN(_new)();
COL_TIPO COL_FUN(_pop)(COL_NOME* const v);
void     COL_FUN(_free)(COL_NOME* const v);
int      COL_FUN(_adjust)(COL_NOME* const v);
int      COL_FUN(_addCell)(COL_NOME* const v);
int      COL_FUN(_reserve)(COL_NOME* const v, uint64_t space);
int      COL_FUN(_push)(COL_NOME* const v, COL_TIPO const newObj);
void     COL_FUN(_moveBelow)(COL_NOME* const v, const uint64_t i);
void     COL_FUN(_removeAt)(COL_NOME* const v, uint64_t position);
int      COL_FUN(_moveAbove)(COL_NOME* const v, const uint64_t i);
COL_TIPO COL_FUN(_popAt)(COL_NOME* const v, const uint64_t position);
uint64_t COL_FUN(_iterateFW)(COL_NOME* const v, COL_FUN(_pred_t) predicate, void* userData);
uint64_t COL_FUN(_iterateBW)(COL_NOME* const v, COL_FUN(_pred_t) predicate, void* userData);
#    ifdef COL_DEALOC
void COL_FUN(_DEALOC)(COL_TIPO* const X);
#    endif
#    ifdef COL_WRITE
int COL_FUN(_write)(const COL_NOME* const v, FILE* f);
#    endif
#    ifdef COL_READ
int COL_FUN(_read)(COL_NOME* const v, FILE* f);
#    endif
#endif

#undef COL_TIPO
#undef COL_NOME
#undef COL_DEALOC
#undef COL_READ
#undef COL_WRITE