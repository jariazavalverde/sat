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
    int success = 1;
    Action actions = {NULL, 0};
    init_interpretation(I, F->variables);
    while(F->length > 0 && success) {
        // 1-literal rule
        success = unit_propagation(F, I, &actions);
        // Positive-negative rule
        if(success)
            success = positive_negative(F, I, &actions);
        // Split cases
        if(success)
            success = split_cases(F, I, &actions);
        // Backtracking
        if(!success && F->length > 0)
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
        // Update node
        node = node->next;
    }
    return 1;
}

/** 1-Literal rule (unit propagation) */
int unit_propagation(Formula *F, Interpretation *I, Action *actions) {
    LiteralNode *literal_node;
    // Iterate unitary clauses
    while(F->unitaries != NULL) {
        literal_node = F->unitaries->clause->lst_literals;
        // Replace variable
        if(replace_variable(F, I, actions, literal_node->atom, literal_node->literal == NEGATIVE ? FALSE : TRUE) == 0)
            return 0;
        // Update unitary nodes
        F->unitaries = F->unitaries->next;
    }
    return 1;
}

/** Positive-Negative rule */
int positive_negative(Formula *F, Interpretation *I, Action *actions) {
    int i;
    // Iterate variables
    for(i = 0; i < F->variables; i++) {
        // If variable is positive
        if(F->count_positives[i] > 0 && F->count_negatives[i] == 0)
            if(replace_variable(F, I, actions, i, TRUE) == 0)
                return 0;
        // If variable is negative
        else if(F->count_negatives[i] > 0 && F->count_positives[i] == 0)
            if(replace_variable(F, I, actions, i, FALSE) == 0)
                return 0;
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
        literal_node = clause_node->clause->lst_literals;
        if(literal_node != NULL) {
            atom = literal_node->atom;
            literal = literal_node->literal;
            attempt = F->attempts[atom];
            if(attempt == BOTH)
                return 0;
            push_action(actions, NULL, atom, NONE);
            F->attempts[atom] = attempt != NONE ? BOTH : (literal == NEGATIVE ? FALSE : TRUE);
            return replace_variable(F, I, actions, atom, attempt == NONE && literal == NEGATIVE || attempt == POSITIVE ? FALSE : TRUE);
        }
    }
    return 0;
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
    if(prev == NULL) {
        F->lst_clauses = next;
        if(next)
			F->lst_clauses->prev = NULL;
    } else {
        prev->next = next;
        if(next != NULL)
            next->prev = prev;
    }
    // Update count of positive-negative literals
    update_count_positive_negative(F, clause, REMOVE);
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
    if(prev == NULL) {
        clause->lst_literals = next;
        if(next)
			clause->lst_literals->prev = NULL;
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
    if(literal == NEGATIVE)
        F->count_negatives[atom]--;
    else
        F->count_positives[atom]--;
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
    // Update count of positive-negative literals
    update_count_positive_negative(F, clause, ADD);
}

/** Add a literal to a clause */
void add_literal(Formula *F, Clause *clause, Atom atom, Literal literal) {
    LiteralNode *node = clause->arr_literals[atom];
    // Add node
    clause->length++;
    node->prev = NULL;
    node->next = clause->lst_literals;
    if(clause->lst_literals != NULL)
        clause->lst_literals->prev = node;
    clause->lst_literals = node;
    // Update unitary clauses
    if(clause->length == 2) {
        // TODO
    }
    // Update count of positive-negative literals
    if(literal == NEGATIVE)
        F->count_negatives[atom]++;
    else
        F->count_positives[atom]++;
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
void push_action(Action *actions, Clause *clause, Atom atom, Literal literal) {
    ActionNode *action;
    action = malloc(sizeof(ActionNode));
    action->clause = clause;
    action->atom = atom;
    action->literal = literal;
    action->prev = actions->first;
    actions->first = action;
}

/** Perform a backtracking */
int backtracking(Formula *F, Interpretation *I, Action *actions) {
    ActionNode *action = actions->first, *prev;
    // Pull actions
    while(action != NULL && (action->clause != NULL || action->literal != NONE)) {
        // Restore clause or literal
        if(action->literal == NONE)
		    add_clause(F, action->clause);
		else
		    add_literal(F, action->clause, action->atom, action->literal);
		// Update node
		prev = action->prev;
		free(action);
		action = prev;
    }
    if(action == NULL)
        return 0;
    I->bindings[action->atom] = NONE;
    actions->first = action->prev;
    free(action);
    return 1;
}
