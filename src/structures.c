/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Operations with structures for SAT problem
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
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
void init_action(Action *actions, int size) {
	int i;
	actions->first = NULL;
	actions->decisions = malloc(size*sizeof(ActionNode*));
	actions->length = 0;
	for(i = 0; i < size; i++)
		actions->decisions[i] = NULL;
}

/** Initialize a new implication graph */
void init_graph(Graph *G, int size) {
	G->nodes = malloc((size+1)*sizeof(GraphNode));
	G->size = size;
	G->max_level = 0;
	for(; size >= 0; size--)
		G->nodes[size] = NULL;
}

/** Add a new node into a graph */
int add_graph_node(Graph *G, Atom atom, Bool value, int level, Decision decision, Clause *clause) {
	int index = decision == CONFLICTIVE ? G->size : atom;
	int i, degree = clause == NULL || decision == ARBITRARY ? 0 : clause->size;
	if(G->nodes[index] != NULL)
		return 0;
	GraphNode *node = malloc(sizeof(GraphNode));
	G->nodes[index] = node;
	node->atom = atom;
	node->value = value;
	node->decision = decision;
	node->degree = degree;
	node->level = level;
	node->antecedents = clause;
	return 1;
}
