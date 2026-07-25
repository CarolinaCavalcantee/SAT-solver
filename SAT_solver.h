#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

#include "DIMACS.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define UNDEFINED 2

/**
 * @struct partial_interpretation
 * @brief Estado atual das atribuições booleanas no solucionador SAT.
 */
typedef struct partial_interpretation{
    int *atributions; 
}partial_interpretation;

/**
 * @struct tree
 * @brief Estrutura de nó para a árvore binária de decisão do algoritmo SAT.
 */
typedef struct tree 
{ 
    int variable; 
    int value; 
    struct tree* left; 
    struct tree* right; 
} tree;

/* ========================================================================== */
/* Funções do SAT_solver.c                                                    */
/* ========================================================================== */

/**
 * Inicializa uma interpretação parcial definindo todos os literais como UNDEFINED.
 * @param formula Ponteiro para a fórmula CNF de referência.
 * @return Estrutura de interpretação parcial alocada e inicializada.
 */
partial_interpretation inicio_partial_interpretation(CNF *formula);

/**
 * Verifica se a fórmula CNF é satisfeita pelas atribuições atuais.
 * @param formula Ponteiro para a fórmula CNF.
 * @param options Ponteiro para o estado atual das atribuições.
 * @return true se todas as cláusulas forem satisfeitas, false caso contrário.
 */
bool eh_sat(CNF *formula, partial_interpretation *options);

/**
 * Verifica se a fórmula CNF tornou-se insatisfatível sob as atribuições atuais.
 * @param formula Ponteiro para a fórmula CNF.
 * @param options Ponteiro para as atribuições atuais.
 * @return true se a fórmula for comprovadamente UNSAT, false caso contrário.
 */
bool eh_unsat(CNF *formula, partial_interpretation *options);

/**
 * Ramifica o espaço de busca criando uma nova interpretação com um valor definido.
 * @param inicio Interpretação parcial base.
 * @param total_literals Total de literais do problema.
 * @param value Valor lógico (0 ou 1) a ser atribuído.
 * @param xi Índice da variável que receberá a valoração.
 * @return Nova interpretação parcial modificada.
 */
partial_interpretation unir(partial_interpretation *inicio, int total_literals, int value, int xi);

/**
 * Executa o resolvedor SAT gerando a árvore de decisão por busca com backtracking.
 * @param formula Ponteiro para a fórmula CNF.
 * @param now Estado atual da interpretação das variáveis.
 * @return Nó raiz da árvore de decisão mapeada.
 */
tree *resposta_sat(CNF *formula, partial_interpretation now);

/**
 * Libera recursivamente toda a memória alocada para a árvore de decisão.
 * @param node Ponteiro para o nó inicial da desalocação.
 */
void free_tree(tree *node);

/**
 * Desaloca as estruturas e literais internos de uma fórmula CNF.
 * @param formula Estrutura CNF alvo da limpeza de memória.
 */
void free_cnf(CNF *formula);

/**
 * Varre a árvore de decisão procurando e imprimindo a valoração de um nó SAT.
 * @param node Nó inicial/atual da busca recursiva.
 * @param total_literals Quantidade total de variáveis a exibir.
 * @return true se encontrou e exibiu uma solução, false caso contrário.
 */
bool imprimir(tree *node, int total_literals);

/* ========================================================================== */
/* Função do readfilecnf.c                                                    */
/* ========================================================================== */

/**
 * Lê e processa um arquivo de entrada no formato DIMACS para construir a estrutura CNF.
 * @param filename Caminho do arquivo CNF.
 * @param problem Ponteiro para o destino onde o problema estruturado será guardado.
 */
void readcnffile(const char* filename, CNF* problem);

#endif
