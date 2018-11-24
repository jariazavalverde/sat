/*H*
 * 
 * FILENAME: 2sat.h
 * DESCRIPTION: 2-satisfiability algorithm
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 24.11.2018
 * 
 *H*/

#include "structures.h"



#ifndef SAT_SAT2_H
#define SAT_SAT2_H



/** Data structures for implication graphs in 2-SAT */
typedef struct SAT2_Node {
	int node;                       // Node
	struct SAT2_Node next;          // Next node
} SAT2_Node;

typedef struct SAT2_Graph {
	SAT2_Node **adjacents;	        // Adjacency lists
	int size;					    // Maximum number of nodes
} SAT2_Graph;



#endif
