#include "DIMACS.h"
#include "SAT_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Ponto de entrada principal do solucionador SAT.
 *
 * Coordena o fluxo de execução do programa: aloca as estruturas necessárias,
 * realiza a leitura do arquivo de entrada no formato DIMACS, inicializa as
 * interpretações das variáveis, executa o algoritmo de decisão e exibe o 
 * veredito final (SAT ou UNSAT) acompanhado da sua respectiva valoração.
 *
 * @return 0 se a execução for concluída com sucesso.
 */
int main(){
    CNF *formula = (CNF*)malloc(sizeof(CNF)); 
    readcnffile("arquivo.cnf", formula);     
    
    partial_interpretation options = inicio_partial_interpretation(formula); 
    
    tree *resposta = resposta_sat(formula, options); 

    if(resposta != NULL && resposta->value == 1)  
    {
        printf("SAT\n");
        imprimir(resposta, formula->total_literals); 
    }
    else
    {
        printf("UNSAT\n"); 
    }

    free_tree(resposta);
    free(options.atributions);
    free_cnf(formula); 
    
    return 0;
}
