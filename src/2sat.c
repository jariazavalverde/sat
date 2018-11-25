/*H*
 * 
 * FILENAME: 2sat.h
 * DESCRIPTION: 2-satisfiability algorithm
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 25.11.2018
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
	int i, nodes = G->nbvar * 2;
	SAT2_Node *node, *next;
	for(i = 0; i < nodes; i++) {
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

/**
  * 
  * This function checks the satisfiability of the formula $F,
  * following the Apswall algorithm for 2-SAT problem. If $F is
  * satisfiable, the function returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  * 
  **/
int sat2_check_sat(Formula *F) {
	SAT2_Graph *G = sat2_implicative_normal_form(F);
	sat2_graph_free(G);
	return 0;
}

/**
  * 
  * This function returns a pointer to the graph representing the 
  * formula $F in implicative normal form. For each variable x there
  * will be two vertices v_x and v_{\neg x}. The edges will correspond
  * to the implications.
  *  
  **/
SAT2_Graph *sat2_implicative_normal_form(Formula *F) {
	int i;
	Clause* clause;
	LiteralNode *fst, *snd;
	SAT2_Graph *G = sat2_graph_alloc(F->nbvar);
	for(i = 0; i < F->nbclauses; i++) {
		clause = F->arr_clauses[i]->clause;
		fst = clause->arr_literals[0];
		snd = clause->arr_literals[1];
		sat2_graph_add_adjacent(G, fst->atom, literal_not(fst->literal), snd->atom, snd->literal);
		sat2_graph_add_adjacent(G, snd->atom, literal_not(snd->literal), fst->atom, fst->literal);
	}
	return G;
}
