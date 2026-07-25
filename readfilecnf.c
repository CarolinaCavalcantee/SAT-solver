#include "DIMACS.h"
#include "SAT_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Lê e processa um arquivo de entrada no formato DIMACS para o problema SAT.
 * Realiza o parse do cabeçalho ignorando comentários, aloca a estrutura de 
 * cláusulas e constrói a lista encadeada de literais até o fim do arquivo.
 *
 * @param filename Caminho ou nome do arquivo CNF a ser lido.
 * @param problem Ponteiro para a estrutura CNF onde os dados do problema serão armazenados.
 */
void readcnffile (const char* filename, CNF* problem)
{
    FILE* file = fopen(filename, "r");

    if(file == NULL) 
    {
        printf("error acessing cnf file\n PLEASE TRY AGAIN");
        exit(1);
    }

    char lines[256];
    problem->clauses = NULL; 

    while (fscanf(file, "%s", lines) == 1) 
    {
        if (strcmp(lines, "c") == 0) 
        {
            while (fgetc(file) != '\n' && !feof(file)); 
        } 

        else if (strcmp(lines, "p") == 0) 
        {
            fscanf(file, " cnf %d %d", &problem->total_literals, &problem->total_clauses);
            break; 
        }
    }

    clause* current_clause = NULL; 

    for (int i = 0; i < problem->total_clauses; i++) 
    {
        clause* new_clause = (clause*)malloc(sizeof(clause));
        new_clause->size = 0; 
        new_clause->literals = NULL;
        new_clause->next = NULL;

        if (problem->clauses == NULL) 
        {
            problem->clauses = new_clause;
        } 
        
        else 
        {
            current_clause->next = new_clause;
        }

        current_clause = new_clause;

        int literal;
        literal_node* current_literal = NULL; 
        
        while (fscanf(file, "%d", &literal) == 1 && literal != 0) 
        {
            literal_node* new_literal = (literal_node*)malloc(sizeof(literal_node));
            new_literal->value = literal;
            new_literal->next = NULL;
            
            if (current_clause->literals == NULL) 
            {
                current_clause->literals = new_literal;
            } 
            
            else 
            {
                current_literal->next = new_literal;
            }
            current_literal = new_literal;

            current_clause->size++;
        }
    }

    fclose(file);
}
