/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 08.10.2018
 * 
 *H*/

#include "sat.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I) {
    int success = 1;
    Action actions = {NULL, 0, 0};
    init_interpretation(I, F->variables);
    while(F->length > 0 && success) {
        // Split cases
        success = split_cases(F, I, &actions);
        // Unit propagation
        if(success)
			success = unit_propagation(F, I, &actions);
        // Backtracking
        if(!success)
            success = backtracking(F, I, &actions);
    }
    return F->length == 0;
}

/** Propagate a value of a variable */
int replace_variable(Formula *F, Interpretation *I, Action *actions, Atom atom, Bool value) {
    Literal literal;
    Clause *clause;
    ClauseNode *node = F->occurrences[atom];
    // Assign interpretation
    I->bindings[atom] = value;
    // Iterate clauses containing the literal
    while(node != NULL) {
        clause = node->clause;
        if(!F->sat_clauses[clause->id]) {
            literal = clause->arr_literals[atom]->literal;
            // If same sign of literal, remove clause
            if(literal == NEGATIVE && value == FALSE || literal == POSITIVE && value == TRUE) {
                remove_clause(F, actions, clause, atom);
            // else, remove the literal from the clause
            } else {
                if(clause->length == 1)
                    return 0;
                remove_literal(F, actions, clause, atom);
            }
        }
        // Update node
        node = node->next;
    }
    return 1;
}

/** Unit propagation */
int unit_propagation(Formula *F, Interpretation *I, Action *actions) {
    LiteralNode *literal_node;
    ClauseNode *clause_node, *next;
    Clause *clause;
    Atom atom;
    int push = 0;
    while(F->lst_unitaries != NULL) {
		clause_node = F->lst_unitaries;
		F->lst_unitaries = NULL;
		// Iterate unitary clauses
		while(clause_node != NULL) {
			// Update unitary nodes
			next = clause_node->next;
			clause_node->prev = NULL;
			clause_node->next = NULL;
			clause = clause_node->clause;
			if(!F->sat_clauses[clause->id] && clause->length == 1) {
				literal_node = clause->lst_literals;
				atom = literal_node->atom;
				if(!push) {
					F->attempts[atom] = BOTH;
					push_action(actions, NULL, atom, NONE);
					push = 1;
				}
				// Replace variable
				if(replace_variable(F, I, actions, atom, literal_node->literal == NEGATIVE ? FALSE : TRUE) == 0)
					return 0;
			}
			clause_node = next;
		}
	}
    return 1;
}

/** Split cases */
int split_cases(Formula *F, Interpretation *I, Action *actions) {
    Atom atom;
    Literal literal, attempt;
    ClauseNode *clause_node;
    LiteralNode *literal_node;
    // Get uninterpreted literal
    clause_node = F->lst_clauses;
    if(clause_node != NULL) {
		if(F->selected_atom == -1) {
			literal_node = clause_node->clause->lst_literals;
			if(literal_node != NULL) {
				atom = literal_node->atom;
				literal = literal_node->literal;
				attempt = F->attempts[atom];
				if(attempt == BOTH)
					return 0;
				push_action(actions, NULL, atom, NONE);
				F->attempts[atom] = attempt != NONE ? BOTH : (literal == NEGATIVE ? NEGATIVE : POSITIVE);
				return replace_variable(F, I, actions, atom, attempt == NONE && literal == NEGATIVE || attempt == POSITIVE ? FALSE : TRUE);
			}
			return 0;
		} else {
			atom = F->selected_atom;
			attempt = F->attempts[atom];
			F->selected_atom = -1;
			F->attempts[atom] = BOTH;
			return replace_variable(F, I, actions, atom, attempt == POSITIVE ? FALSE : TRUE);
		}
    }
    return 1;
}

/** Remove a clause from a formula */
void remove_clause(Formula *F, Action *actions, Clause *clause, Atom atom) {
    int clause_id;
    ClauseNode *prev, *next;
    clause_id = clause->id;
    // Set clause as satisfiable
    F->sat_clauses[clause_id] = 1;
    F->length--;
    // Remove node
    prev = F->arr_clauses[clause_id]->prev;
    next = F->arr_clauses[clause_id]->next;
    if(prev == NULL)
        F->lst_clauses = next;
    else
        prev->next = next;
    if(next != NULL)
        next->prev = prev;
    // Add action
    push_action(actions, clause, atom, NONE);
}

/** Remove a literal from a clause */
void remove_literal(Formula *F, Action *actions, Clause *clause, Atom atom) {
    Literal literal;
    LiteralNode *next, *prev;
    ClauseNode *unitary;
    // Remove node
    clause->length--;
    prev = clause->arr_literals[atom]->prev;
    next = clause->arr_literals[atom]->next;
    if(prev == NULL)
        clause->lst_literals = next;
    else
        prev->next = next;
    if(next != NULL)
        next->prev = prev;
    // Update unitary clauses
    if(clause->length == 1) {
        unitary = F->arr_unitaries[clause->id];
        unitary->prev = NULL;
        if(F->lst_unitaries != NULL) {
			unitary->next = F->lst_unitaries->next;
            F->lst_unitaries->next = unitary;
        }
        F->lst_unitaries = unitary;
    }
    // Add action
    push_action(actions, clause, atom, literal);
}

/** Add a clause to a formula */
void add_clause(Formula *F, Clause *clause) {
    int clause_id = clause->id;
    ClauseNode *node = F->arr_clauses[clause_id];
    // Unset clause as satisfiable
    F->sat_clauses[clause_id] = 0;
    F->length++;
    // Add node
    node->prev = NULL;
    node->next = F->lst_clauses;
    if(F->lst_clauses != NULL)
        F->lst_clauses->prev = node;
    F->lst_clauses = node;
}

/** Add a literal to a clause */
void add_literal(Formula *F, Clause *clause, Atom atom, Literal literal) {
    LiteralNode *node = clause->arr_literals[atom];
    ClauseNode *unitary;
    // Add node
    clause->length++;
    node->prev = NULL;
    node->next = clause->lst_literals;
    if(clause->lst_literals != NULL)
        clause->lst_literals->prev = node;
    clause->lst_literals = node;
    // Update unitary clauses
    if(clause->length == 2) {
        unitary = F->arr_unitaries[clause->id];
        if(F->lst_unitaries != NULL && F->lst_unitaries->clause->id == unitary->clause->id)
			F->lst_unitaries = unitary->next;
		if(unitary->next != NULL)
			unitary->next->prev = unitary->prev;
    }
}

/** Prepend a new action */
void push_action(Action *actions, Clause *clause, Atom atom, Literal literal) {
    ActionNode *action;
    action = malloc(sizeof(ActionNode));
    action->clause = clause;
    action->atom = atom;
    action->literal = literal;
    action->prev = actions->first;
    actions->first = action;
    actions->length++;
    actions->last_id++;
    action->id = actions->last_id;
}

/** Perform a backtracking */
int backtracking(Formula *F, Interpretation *I, Action *actions) {
    ActionNode *action = actions->first, *prev;
    // Pull actions
    while(action != NULL && (action->clause != NULL || action->clause == NULL && F->attempts[action->atom] == BOTH)) {
        if(action->clause != NULL) {
            // Restore clause
            if(action->literal == NONE)
                add_clause(F, action->clause);
            // Restore literal
            else
                add_literal(F, action->clause, action->atom, action->literal);
        // Restore variable
        } else {
            I->bindings[action->atom] = UNKNOWN;
            F->attempts[action->atom] = NONE;
        }
        // Update node
        prev = action->prev;
        free(action);
        action = prev;
        actions->length--;
    }
    if(action == NULL)
        return 0;
    F->selected_atom = action->atom;
    actions->first = action;
    return 1;
}
