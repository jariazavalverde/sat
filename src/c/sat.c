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
    Action actions = {NULL, 0};
    // 1-literal rule
    unit_propagation(F, I, &actions);
    // positive-negative rule
    positive_negative(F, I, &actions);
    // split
    return 0;
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
    while(unit_node != NULL) {
        clause = unit_node->clause;
        literal_node = clause->lst_literals;
        atom = literal_node->atom;
        literal = literal_node->literal;
        occurs_node = F->occurrences[atom];
        // Iterate clauses containing the literal
        while(occurs_node != NULL) {
            clause = occurs_node->clause;
            // If same sign of literal, remove clause
            if(clause->arr_literals[atom] == literal) {
                remove_clause(F, clause, atom, actions);
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

/** Remove a clause from a formula */
void remove_clause(Formula *F, Clause *clause, Atom atom, Action *actions) {
    // Set clause as satisfiable
    F->sat_clauses[clause->id] = 1;
    F->length--;
    // Update count of positive-negative literals
    update_count_positive_negative(F, clause, REMOVE);
    // Add action
    push_action(atom, clause, NONE, actions);
}

/** Remove a literal from a clause */
void remove_literal(Formula *F, Clause *clause, Atom atom, Action *actions) {
    Literal literal;
    // Set literal as dropped
    //clause->dropped_literals[atom] = 1;
    clause->length--;
    // Update count of positive-negative literals
    literal = clause->arr_literals[atom];
    if(literal == NEGATIVE) {
        F->count_negatives[atom]--;
    } else {
        F->count_positives[atom]--;
    }
    // Add action
    push_action(atom, clause, literal, actions);
}

/** Update count of positive-negative literals in a formula */
void update_count_positive_negative(Formula *F, Clause *clause, Operation op) {
    int incr = op == ADD ? 1 : -1;
    LiteralNode *node = clause->lst_literals;
    // Iterate literals
    while(node) {
		if(node->literal == NEGATIVE) {
			F->count_negatives[node->atom] += incr;
		} else {
			F->count_positives[node->atom] += incr;
		}
		// Update node
		node = node->next;
	}
}

/** Prepend a new action */
void push_action(Atom atom, Clause *clause, Literal literal, Action *actions) {
    ActionNode *action;
    action = malloc(sizeof(ActionNode));
    action->step = atom;
    action->clause = clause;
    action->literal = literal;
    action->prev = actions->first;
    actions->first = action;
}
