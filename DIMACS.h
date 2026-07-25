#ifndef DIMACS_H
#define DIMACS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct literal_node
 * @brief Elemento de uma lista encadeada que representa um literal booleano.
 *
 * Armazena o identificador numérico da variável lógica. Se o valor for negativo,
 * representa a negação daquela variável.
 */
typedef struct literal_node 
{
    int value;
    struct literal_node* next;
} literal_node;

/**
 * @struct clause
 * @brief Representa uma única cláusula na fórmula CNF.
 *
 * Consiste em uma lista encadeada de literais conectados por uma disjunção lógica (OR).
 */
typedef struct clause 
{
    literal_node* literals; 
    int size; 
    struct clause* next; 

} clause;

/**
 * @struct CNF
 * @brief Estrutura principal que encapsula a fórmula lógica em Forma Normal Conjuntiva.
 *
 * Gerencia a lista global de cláusulas e armazena os metadados quantitativos do problema.
 */
typedef struct CNF
{
    clause* clauses; 
    int total_clauses; 
    int total_literals; 
} CNF;

#endif
