/*H*
 * 
 * FILENAME: 2sat.h
 * DESCRIPTION: 2-satisfiability algorithm
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 24.11.2018
 * 
 *H*/

#include "2sat.h"



/**
  * 
  * This function creates an implication graph of $nbvar variables,
  * returning a pointer to a newly initialized SAT2_Graph struct.
  * 
  **/
SAT2_Graph *sat2_graph_alloc(int nbvar) {
	int i;
	int nodes = nbvar * 2;
	SAT2_Graph *G = malloc(sizeof(SAT2_Graph));
	G->adjacents = malloc(nodes * sizeof(SAT2_Node));
	G->nbvar = nbvar;
	for(i = 0; i <= nodes; i++)
		G->adjacents[i] = NULL;
	return G;
}

/**
  * 
  * This function frees a previously allocated graph $G.
  * The graph nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the graph nodes will not be deallocated.
  * 
  **/
void sat2_graph_free(SAT2_Graph *G) {
	int i;
	SAT2_Node *node, *next;
	for(i = 0; i <= G->size; i++) {
		node = G->adjacents[i];
		while(node != NULL) {
			next = node->next;
			free(node);
			node = next;
		}
	}
	free(G->adjacents);
	free(G);
}

/**
 * 
 * This function adds a new adjacent ($atom_to, $lit_to) to the
 * element ($atom_from, $lit_from) of the graph $G.
 *  
 **/
void sat2_graph_add_adjacent(SAT2_Graph *G, Atom atom_from, Literal lit_from, Atom atom_to, Literal lit_to) {
	int index_from = lit_from == NEGATIVE ? G->nbvar + atom_from : atom_from;
	int index_to = lit_to == NEGATIVE ? G->nbvar + atom_to : atom_to;
	SAT2_Node *node = malloc(sizeof(SAT2_Node));
	node->node = index_to;
	node->next = G->adjacents[index_from];
	G->adjacents[index_from] = node;
}
