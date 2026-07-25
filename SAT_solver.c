#include "DIMACS.h"
#include "SAT_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 *Inicializa uma interpretação parcial, alocando memória para o vetor de atribuições
 *e definindo todos os valores iniciais como UNDEFINED.
 * * @param formula Ponteiro para a fórmula CNF de referência.
 * @return Estrutura de interpretação parcial inicializada.
 */
partial_interpretation inicio_partial_interpretation(CNF *formula){
    partial_interpretation new_options;
    
    new_options.atributions = malloc(sizeof(int) *(formula->total_literals + 1));

    for (int i = 0; i < formula->total_literals + 1; i++){
        new_options.atributions[i] = UNDEFINED;
    }

    return new_options;
}

/**
 *Verifica se a fórmula CNF é completamente satisfeita (SAT) pela interpretação atual.
 *Uma fórmula é satisfeita se todas as suas cláusulas possuírem pelo menos um literal verdadeiro.
 * * @param formula Ponteiro para a fórmula CNF.
 * @param options Ponteiro para as atribuições de variáveis atuais.
 * @return true se a fórmula for totalmente satisfeita, false caso contrário.
 */
bool eh_sat(CNF *formula, partial_interpretation *options){
    clause * clauses_now = formula->clauses; 

    while(clauses_now != NULL){ 
        bool satisfeita = false; 
        literal_node * literal_now = clauses_now->literals; 

        while(literal_now != NULL){ 
            int valores = options->atributions[abs(literal_now->value)]; 

            if(valores !=UNDEFINED){
                if((literal_now->value > 0 && valores == 1) || (literal_now->value < 0 && valores == 0)){ 
                    satisfeita = true; 
                    break; 
                }
            }
            literal_now = literal_now->next; 
        } 
            if(!satisfeita) 
            {
                return false;
            }
        
        clauses_now = clauses_now->next; 
    }   
    
    return true; 
}

/**
 *Verifica se a fórmula CNF é insatisfatível (UNSAT) sob a interpretação atual.
 *Uma fórmula é considerada UNSAT se contiver pelo menos uma cláusula cujos literais 
 *avaliados sejam todos falsos e não possua mais nenhum literal indefinido.
 * * @param formula Ponteiro para a fórmula CNF.
 * @param options Ponteiro para as atribuições atuais.
 * @return true se a fórmula for comprovadamente insatisfatível, false caso contrário.
 */
bool eh_unsat(CNF *formula, partial_interpretation *options){
clause *clauses_now = formula->clauses;

while(clauses_now != NULL){
    bool clause_still_valid = false;
    literal_node *literal_now = clauses_now->literals;

    while (literal_now != NULL){
        int valores = options->atributions[abs(literal_now->value)];
    
    if (valores == UNDEFINED){
        clause_still_valid = true;
        break;
    }

    if((literal_now->value > 0 && valores == 1) || (literal_now->value < 0 && valores == 0)){
        clause_still_valid = true;
        break;
    }
        literal_now = literal_now->next;
}
    if(!clause_still_valid){
        return true;
    }
    clauses_now = clauses_now->next; 
}
return false; 
}

/**
 *Cria uma cópia da interpretação parcial atual e aplica uma ramificação de valor
 *lógico (0 ou 1) em uma variável específica.
 * * @param inicio Ponteiro para a interpretação parcial base.
 * @param total_literals Quantidade total de literais na fórmula.
 * @param value Valor booleano a ser atribuído (0 ou 1).
 * @param xi Índice da variável alvo da ramificação.
 * @return Nova estrutura de interpretação parcial gerada.
 */
partial_interpretation unir(partial_interpretation *inicio, int total_literals, int value, int xi){
    partial_interpretation new_atributions;
    
    new_atributions.atributions = malloc(sizeof(int) *(total_literals + 1));

    for (int i = 0; i < total_literals + 1; i++){
        new_atributions.atributions[i] = inicio->atributions[i];
    }
    new_atributions.atributions[xi] = value;
    return new_atributions;

}

/**
 *Executa o resolvedor algoritmo SAT clássico por meio da construção de uma árvore de decisão.
 *Utiliza busca em profundidade com ramificação e backtracking baseado nos estados de SAT e UNSAT.
 * * @param formula Ponteiro para a fórmula CNF.
 * @param now Estado atual das atribuições das variáveis booleanas.
 * @return Nó raiz da árvore binária de decisão mapeada.
 */
tree *resposta_sat(CNF *formula, partial_interpretation now){
    tree *node = (tree*)malloc(sizeof(tree));
    node->left = NULL;
    node->right = NULL;


    if(eh_sat (formula, &now)){
        node->value = 1; 
        return node;
    }

    if(eh_unsat(formula, &now)){
    node->value = 0;
    return node;
    }

    int xi = -1; 

    for (int i = 1; i <= formula->total_literals; i++){
        if (now.atributions[i] == UNDEFINED){
            xi = i; 
            break;
        }
    }
        if(xi == - 1){
            node->value = 0;
            return node;
        }

        node->variable = xi;

    partial_interpretation L_true = unir(&now, formula->total_literals, 1, xi);
    node->left = resposta_sat(formula,L_true);
    free(L_true.atributions);

    partial_interpretation R_false = unir(&now, formula->total_literals, 0,xi);
    node->right = resposta_sat(formula, R_false);
    free(R_false.atributions);

    node->value = node->left->value || node->right->value;
    
    return node;
}

/**
 *Libera recursivamente toda a memória dinâmica alocada para os nós da árvore de decisão.
 * * @param node Ponteiro para o nó inicial/atual da árvore a ser desalocado.
 */
void free_tree (tree *node){
        if (node == NULL){
            return;
        }

        free_tree(node->left);
        free_tree(node->right);
        free (node);
    }

/**
 *Desaloca toda a memória alocada para a estrutura CNF, varrendo as listas 
 *encadeadas de cláusulas e de seus respectivos literais.
 * * @param formula Ponteiro para a estrutura CNF que será limpa.
 */
void free_cnf(CNF *formula){
    clause *current_clause = formula->clauses;
    while(current_clause != NULL){
        literal_node *literal = current_clause->literals;

        while(literal != NULL){
        literal_node *aux = literal;
        literal = literal->next;
        free(aux);
        }
        clause *aux_clause = current_clause;
        current_clause = current_clause->next;
        free(aux_clause);
    }
    free(formula);
}

/**
*Percorre recursivamente a árvore de decisão procurando caminhos válidos (SAT).
*Exibe no terminal a valoração final de cada variável que levou ao sucesso da fórmula.
* * @param node Ponteiro para o nó atual da busca na árvore.
* @param total_literals Quantidade de variáveis registradas no problema.
* @return true se encontrou um caminho folha satisfatível com sucesso, false caso contrário.
*/
bool imprimir(tree *node, int total_literals) 
{
    if(node == NULL) 
    {
        return false;
    }

    if(node->value == 1 && node->left == NULL && node->right == NULL)  
    {
        printf("Configuracao encontrada:\n");
        return true; 
    }

    if(node->left != NULL && node->left->value == 1){
        if (imprimir(node->left, total_literals)){
            if (node->variable != -1){
                printf("%d = 1\n", node->variable); 
            } 
            return true;
        }
    }
    
    if(node->right != NULL && node->right->value == 1){
        if (imprimir(node->right, total_literals)){
            if (node->variable != -1){
                printf("%d = 0\n", node->variable);
            }
            return true;
        }
    }
    
    return false;
}
