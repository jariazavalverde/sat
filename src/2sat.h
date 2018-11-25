/*H*
 * 
 * FILENAME: 2sat.h
 * DESCRIPTION: 2-satisfiability algorithm
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 25.11.2018
 * 
 *H*/

#include "structures.h"



#ifndef SAT_SAT2_H
#define SAT_SAT2_H



/** Data structures for implication graphs in 2-SAT */
typedef struct SAT2_Node {
	int node;                       // Adjacent to
	struct SAT2_Node next;          // Next node
} SAT2_Node;

typedef struct SAT2_Graph {
	SAT2_Node **adjacents;	        // Adjacency lists
	int nbvar;					    // Number of variables
} SAT2_Graph;



#endif



/**
  * 
  * This function creates an implication graph of $nbvar variables,
  * returning a pointer to a newly initialized SAT2_Graph struct.
  * 
  **/
SAT2_Graph *sat2_graph_alloc(int nbvar);

/**
  * 
  * This function frees a previously allocated graph $G.
  * The graph nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the graph nodes will not be deallocated.
  * 
  **/
void sat2_graph_free(SAT2_Graph *G);

/**
  * 
  * This function adds a new adjacent ($atom_to, $lit_to) to the
  * element ($atom_from, $lit_from) of the graph $G.
  *  
  **/
void sat2_graph_add_adjacent(SAT2_Graph *G, Atom atom_from, Literal lit_from, Atom atom_to, Literal lit_to);

/**
  * 
  * This function checks the satisfiability of the formula $F,
  * following the Apswall algorithm for 2-SAT problem. If $F is
  * satisfiable, the function returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  * 
  **/
int sat2_check_sat(Formula *F);

/**
  * 
  * This function returns a pointer to the graph representing the 
  * formula $F in implicative normal form. For each variable x there
  * will be two vertices v_x and v_{\neg x}. The edges will correspond
  * to the implications.
  *  
  **/
SAT2_Graph *sat2_implicative_normal_form(Formula *F);

