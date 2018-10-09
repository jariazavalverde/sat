/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Operations with structures for SAT problem
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 09.10.2018
 * 
 *H*/

#include "structures.h"



/** Initialiaze a new interpretation */
void init_interpretation(Interpretation *I, int length) {
    int i;
    I->length = length;
    I->bindings = malloc(length*sizeof(Bool));
    for(i = 0; i < length; i++)
        I->bindings[i] = UNKNOWN;
}

/** Initializa a new action */
void init_action(Action *actions) {
	actions->first = NULL;
	actions->length = 0;
}

/** Initialize a new implication graph */
void init_graph(Graph *G, int size) {
	G->nodes = malloc((size+1)*sizeof(GraphNode));
	G->size = size;
	for(; size >= 0; size--)
		G->nodes[size] = NULL;
}

/** Add a new node into a graph */
int add_graph_node(Graph *G, Atom atom, Bool value, Decision decision, Clause *clause) {
	int index = decision == CONFLICTIVE ? G->size : atom;
	int i, j = 0, degree = clause == NULL ? 0 : clause->size-1;
	if(G->nodes[index] != NULL)
		return 0;
	GraphNode *node = malloc(sizeof(GraphNode));
	G->nodes[index] = node;
	node->atom = atom;
	node->value = value;
	node->decision = decision;
	node->degree = degree;
	node->antecedents = malloc(degree*sizeof(int));
	if(clause != NULL)
		for(i = 0; i <= degree; i++) {
			if(clause->literals[i] != atom) {
				node->antecedents[j] = clause->literals[i];
				j++;
			}
		}
	return 1;
}

/** Set the value of a graph node */
int set_graph_node(Graph *G, Atom atom, Bool value) {
	if(G->nodes[atom] != NULL)
		return 0;
	G->nodes[atom]->value = value;
	return 1;
}
