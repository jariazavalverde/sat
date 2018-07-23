/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * 
 *H*/

#include "sat.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I) {
	Action *actions;
	// 1-literal rule
	unit_propagation(F, I, actions);
	// positive-negative rule
	positive_negative(F, I, actions);
	// split
    return 0;
}

/** Remove a clause from a formula */
void remove_clause(Formula *F, Clause *clause, Action *actions) {
	
}

/** Remove a literal from a clause */
void remove_literal(Formula *F, Clause *clause, Literal literal, Action *actions) {
	
}

/** 1-Literal rule (unit propagation) */
void unit_propagation(Formula *F, Interpretation *I, Action *actions) {
	Atom atom;
	Literal literal;
	Clause *clause;
	LiteralNode *literal_node;
	ClauseNode *unit_node, *occurs_node;
	unit_node = F->unitaries;
	// Iterate unitary clauses
	while(unit_node) {
		clause = unit_node->clause;
		literal_node = clause->lst_literals;
		atom = literal_node->atom;
		literal = literal_node->literal;
		occurs_node = F->occurrences[atom];
		// Iterate clauses containing the literal
		while(occurs_node) {
			clause = occurs_node->clause;
			// If same sign of literal, remove clause
			if(clause->arr_literals[atom] == literal) {
				remove_clause(F, clause, actions);
			// else, remove the literal from the clause
			} else {
				remove_literal(F, clause, atom, actions);
			}
			// Update node
			occurs_node = occurs_node->next;
		}
		// Update unitary nodes
		unit_node = unit_node->next;
		F->unitaries = unit_node;
	}
}

/** Positive-Negative rule */
void positive_negative(Formula *F, Interpretation *I, Action *actions) {
	
}

