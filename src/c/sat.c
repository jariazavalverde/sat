/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 26.07.2018
 * 
 *H*/

#include "sat.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I) {
    Action actions = {NULL, 0};
    init_interpretation(I, F->variables);
    while(F->length > 0) {
        // 1-literal rule
        unit_propagation(F, I, &actions);
        // positive-negative rule
        positive_negative(F, I, &actions);
        // split
        // ...
    }
    return 0;
}

/** Propagate a value of a variable */
void replace_variable(Formula *F, Interpretation *I, Action *actions, Atom atom, Bool value) {
	Literal literal;
	Clause *clause;
	ClauseNode *node = F->occurrences[atom];
	// Assign interpretation
	I->bindings[atom] = value;
	// Iterate clauses containing the literal
	while(node != NULL) {
		clause = node->clause;
		literal = clause->arr_literals[atom]->literal;
		// If same sign of literal, remove clause
		if(literal == NEGATIVE && value == FALSE || literal == POSITIVE && value == TRUE) {
			remove_clause(F, clause, atom, actions);
		// else, remove the literal from the clause
		} else {
			remove_literal(F, clause, atom, actions);
		}
		// Update node
		node = node->next;
	}
}

/** 1-Literal rule (unit propagation) */
void unit_propagation(Formula *F, Interpretation *I, Action *actions) {
    LiteralNode *literal_node;
    // Iterate unitary clauses
    while(F->unitaries != NULL) {
        literal_node = F->unitaries->clause->lst_literals;
        // Replace variable
        replace_variable(F, I, actions, literal_node->atom, literal_node->literal == NEGATIVE ? FALSE : TRUE);
        // Update unitary nodes
        F->unitaries = F->unitaries->next;
    }
}

/** Positive-Negative rule */
void positive_negative(Formula *F, Interpretation *I, Action *actions) {
    int i;
    // Iterate variables
    for(i = 0; i < F->variables; i++) {
	    // If variable is positive
	    if(F->count_positives[i] > 0 && F->count_negatives[i] == 0)
		    replace_variable(F, I, actions, i, TRUE);
		// If variable is negative
		else if(F->count_negatives[i] > 0 && F->count_positives[i] == 0)
		    replace_variable(F, I, actions, i, FALSE);
	}
}

/** Remove a clause from a formula */
void remove_clause(Formula *F, Clause *clause, Atom atom, Action *actions) {
    int clause_id;
    ClauseNode *prev, *next;
    // Set clause as satisfiable
    F->sat_clauses[clause->id] = 1;
    F->length--;
    // Remove node
    clause_id = clause->id;
    prev = F->arr_clauses[clause_id]->prev;
    next = F->arr_clauses[clause_id]->next;
    if(prev == NULL) {
        F->lst_clauses = next;
    } else {
        prev->next = next;
        if(next != NULL)
            next->prev = prev;
    }
    // Update count of positive-negative literals
    update_count_positive_negative(F, clause, REMOVE);
    // Add action
    push_action(atom, clause, NONE, actions);
}

/** Remove a literal from a clause */
void remove_literal(Formula *F, Clause *clause, Atom atom, Action *actions) {
    Literal literal;
    LiteralNode *next, *prev;
    ClauseNode *unitary;
    // Remove node
    clause->length--;
    prev = clause->arr_literals[atom]->prev;
    next = clause->arr_literals[atom]->next;
    if(prev == NULL) {
        clause->lst_literals = next;
    } else {
        prev->next = next;
        if(next != NULL)
            next->prev = prev;
    }
    // Update unitary clauses
    if(clause->length == 1) {
        unitary = malloc(sizeof(ClauseNode));
        unitary->clause = clause;
        unitary->next = F->unitaries == NULL ? NULL : F->unitaries->next;
        unitary->prev = F->unitaries;
        if(F->unitaries != NULL)
            F->unitaries->next = unitary;
        else
		    F->unitaries = unitary;
    }
    // Update count of positive-negative literals
    literal = clause->arr_literals[atom]->literal;
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
