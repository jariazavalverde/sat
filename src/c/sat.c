/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 22.07.2018
 * 
 *H*/

#include "sat.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I) {
	// 1-literal rule
	unit_propagation(F, I);
	// positive-negative rule
	positive_negative(F, I);
	// split
    return 0;
}

/** 1-Literal rule (unit propagation) */
void unit_propagation(Formula *F, Interpretation *I) {
	int i;
	Atom atom;
	Literal literal;
	Clause *clause;
	ClauseNode* node = F->unitaries->clause;
	while(node) {
		clause = node->clause;
		atom = 
	}
}

/** Positive-Negative rule */
void positive_negative(Formula *F, Interpretation *I) {
	
}

