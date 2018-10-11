/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 12.10.2018
 * 
 *H*/

#include "sat.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I) {
    int success = 1;
    Atom atom;
    Action actions;
    Graph G;
    // Initialize structures
    init_interpretation(I, F->variables);
    init_action(&actions);
    init_graph(&G, F->variables);
    // Unit propagation
    if(unit_propagation(F, &G, I, &actions) == 0)
		return 0;
    while(F->length > 0 && success) {
        // Split cases
        success = split_cases(F, &G, I, &actions);
        // Unit propagation
        if(success)
			success = unit_propagation(F, &G, I, &actions);
        // Backtracking
        if(!success) {
			atom = analyze_conflict(F, &G);
            success = backtracking(F, &G, I, atom, &actions);
        }
    }
    return F->length == 0;
}

/** Propagate a value of a variable */
int replace_variable(Formula *F, Graph *G, Interpretation *I, Action *actions, Atom atom, Bool value) {
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
                if(clause->length == 1) {
					// Add conflictive node
					add_graph_node(G, atom, value, G->max_level, CONFLICTIVE, clause);
                    return 0;
                }
                remove_literal(F, actions, clause, atom);
            }
        }
        // Update node
        node = node->next;
    }
    return 1;
}

/** Unit propagation */
int unit_propagation(Formula *F, Graph *G, Interpretation *I, Action *actions) {
    LiteralNode *literal_node;
    ClauseNode *clause_node, *next;
    Clause *clause;
    Atom atom;
    Bool value;
    // Push action
    /*if(F->lst_unitaries != NULL) {
		atom = F->lst_unitaries->clause->lst_literals->atom;
		F->attempts[atom] = BOTH;
		push_action(actions, NULL, atom, NONE);
	}*/
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
			literal_node = clause->lst_literals;
			atom = literal_node->atom;
			value = literal_node->literal == NEGATIVE ? FALSE : TRUE;
			// Push action
			F->attempts[atom] = BOTH;
			push_action(actions, NULL, atom, NONE);
			// Add graph node
			add_graph_node(G, atom, value, G->max_level, FORCED, clause);
			// Replace variable
			if(replace_variable(F, G, I, actions, atom, value) == 0)
				return 0;
			clause_node = next;
		}
	}
    return 1;
}

/** Split cases */
int split_cases(Formula *F, Graph *G, Interpretation *I, Action *actions) {
    Atom atom;
    Literal literal, attempt;
    Bool value;
    ClauseNode *clause_node;
    LiteralNode *literal_node;
    // Update level of the implication graph
    G->max_level++;
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
				value = literal == NEGATIVE ? FALSE : TRUE;
				push_action(actions, NULL, atom, NONE);
				add_graph_node(G, atom, value, G->max_level, ARBITRARY, NULL);
				F->attempts[atom] = attempt != NONE ? BOTH : literal;
				return replace_variable(F, G, I, actions, atom, value);
			}
			return 0;
		} else {
			atom = F->selected_atom;
			attempt = F->attempts[atom];
			F->selected_atom = -1;
			F->attempts[atom] = BOTH;
			value = attempt == POSITIVE ? FALSE : TRUE;
			set_graph_node(G, atom, value);
			return replace_variable(F, G, I, actions, atom, value);
		}
    }
    return 1;
}

/** Analyze the conflict in the implication graph */
Atom analyze_conflict(Formula *F, Graph *G) {
	Clause *clause = malloc(sizeof(Clause));
	ClauseNode *clause_node, *unitary, *occurrence;
	LiteralNode *literal_node = NULL, *first_literal_node = NULL, *last_literal_node = NULL, *prev, *next;
	LiteralNode **arr_literals = malloc(F->variables * sizeof(LiteralNode*));
	Atom atom;
	int i, level, length, degree;
	// Initialize array of literals
	for(i = 0; i < F->variables; i++)
		arr_literals[i] = NULL;
	// Get conflictive clause
	GraphNode *node = G->nodes[G->size];
	degree = node->degree;
	length = degree;
	level = node->level;
	// Initialize new clause
	for(i = 0; i < degree; i++) {
		atom = node->antecedents[i];
		literal_node = malloc(sizeof(LiteralNode));
		arr_literals[atom] = literal_node;
		literal_node->atom = atom;
		literal_node->literal = G->nodes[atom]->value == TRUE ? NEGATIVE : POSITIVE;
		literal_node->prev = NULL;
		literal_node->next = last_literal_node;
		if(last_literal_node != NULL)
			last_literal_node->prev = literal_node;
		last_literal_node = literal_node;
		if(first_literal_node == NULL)
			first_literal_node = literal_node;
	}
	// While literals in level
	while(first_literal_node != NULL) {
		// Skip nodes with another level
		while(first_literal_node != NULL && (G->nodes[first_literal_node->atom]->level != level || G->nodes[first_literal_node->atom]->decision == ARBITRARY)) {
			first_literal_node = first_literal_node->prev;
		}
		if(first_literal_node != NULL) {
			length--;
			node = G->nodes[first_literal_node->atom];
			degree = node->degree;
			for(i = 0; i < degree; i++) {
				atom = node->antecedents[i];
				if(arr_literals[atom] == NULL) {
					length++;
					literal_node = malloc(sizeof(LiteralNode));
					arr_literals[atom] = literal_node;
					literal_node->atom = atom;
					literal_node->literal = G->nodes[atom]->value == TRUE ? NEGATIVE : POSITIVE;
					literal_node->prev = NULL;
					literal_node->next = last_literal_node;
					if(last_literal_node != NULL)
						last_literal_node->prev = literal_node;
					last_literal_node = literal_node;
				}/* else if(arr_literals[atom]->literal != G->nodes[atom]->value) {
					length--;
					printf("rem %d %d %d\n", atom, arr_literals[atom]->literal, G->nodes[atom]->value);
					literal_node = arr_literals[atom];
					arr_literals[atom] = NULL;
					prev = literal_node->prev;
					next = literal_node->next;
					if(next != NULL)
						next->prev = prev;
					if(prev != NULL)
						prev->next = next;
					if(last_literal_node == literal_node)
						last_literal_node = next;
				}*/
			}
			next = first_literal_node->next;
			prev = first_literal_node->prev;
			if(prev == NULL)
				last_literal_node = next;
			else
				prev->next = next;
			if(next != NULL)
				next->prev = prev;
			first_literal_node = prev;
		}
	}
	// Create clause
	clause->id = F->size;
	clause->literals = malloc(length*sizeof(int));
	literal_node = last_literal_node;
	clause->arr_literals = arr_literals;
	clause->lst_literals = last_literal_node;
	clause->length = length;
	clause->size = length;
	// Create clause node
	clause_node = malloc(sizeof(ClauseNode));
	clause_node->clause = clause;
	clause_node->next = F->lst_clauses;
	clause_node->prev = NULL;
	// Create unitary node
	unitary = malloc(sizeof(ClauseNode));
	unitary->clause = clause;
	unitary->next = NULL;
	unitary->prev = NULL;
	// Add clause to the formula
	F->size++;
	F->length++;
	F->arr_clauses = realloc(F->arr_clauses, F->size * sizeof(ClauseNode*));
	F->arr_unitaries = realloc(F->arr_unitaries, F->size * sizeof(ClauseNode*));
	F->sat_clauses = realloc(F->sat_clauses, F->size * sizeof(int));
	F->sat_clauses[F->size-1] = 0;
	F->arr_clauses[F->size-1] = clause_node;
	F->arr_unitaries[F->size-1] = unitary;
	if(F->lst_clauses != NULL)
		F->lst_clauses->prev = clause_node;
	F->lst_clauses = clause_node;
	// Find atom to backjump
	literal_node = last_literal_node;
	for(i = 0; i < length; i++) {
		clause->literals[i] = literal_node->atom;
		// Occurrence clause node
		occurrence = malloc(sizeof(ClauseNode));
		occurrence->clause = clause;
		occurrence->next = F->occurrences[literal_node->atom];
		occurrence->prev = NULL;
		if(F->occurrences[literal_node->atom] != NULL)
			F->occurrences[literal_node->atom]->prev = occurrence;
		F->occurrences[literal_node->atom] = occurrence;
		// Update level
		if(G->nodes[literal_node->atom]->decision == ARBITRARY && G->nodes[literal_node->atom]->level <= level)  {
			level = G->nodes[literal_node->atom]->level;
			atom = literal_node->atom;
		}
		literal_node = literal_node->next;
	}
	return atom;
}

/** Remove a clause from a formula */
void remove_clause(Formula *F, Action *actions, Clause *clause, Atom atom) {
    int clause_id = clause->id;
    ClauseNode *prev, *next;
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
    // Remove unitary clause
    if(clause->length == 1)
		remove_unitary_clause(F, clause_id);
    // Add action
    push_action(actions, clause, atom, NONE);
}

/** Remove a literal from a clause */
void remove_literal(Formula *F, Action *actions, Clause *clause, Atom atom) {
    Literal literal;
    LiteralNode *next, *prev;
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
    // Add unitary clause
    if(clause->length == 1)
		add_unitary_clause(F, clause->id);
    // Add action
    push_action(actions, clause, atom, literal);
}

/** Remove unitary clause */
void remove_unitary_clause(Formula *F, int clause_id) {
	ClauseNode *unitary = F->arr_unitaries[clause_id];
	if(F->lst_unitaries != NULL && F->lst_unitaries->clause->id == unitary->clause->id)
		F->lst_unitaries = unitary->next;
	else
		if(unitary->prev != NULL)
			unitary->prev->next = unitary->next;
		if(unitary->next != NULL)
			unitary->next->prev = unitary->prev;
	unitary->next = NULL;
	unitary->prev = NULL;
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
    // Add unitary clause
    if(clause->length == 1)
		add_unitary_clause(F, clause->id);
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
    // Remove unitary clause
    if(clause->length > 1)
		remove_unitary_clause(F, clause->id);
}

/** Add unitary clause */
void add_unitary_clause(Formula *F, int clause_id) {
	ClauseNode *unitary = F->arr_unitaries[clause_id];
	if(F->lst_unitaries != NULL)
		F->lst_unitaries->prev = unitary;
	unitary->next = F->lst_unitaries;
	unitary->prev = NULL;
	F->lst_unitaries = unitary;
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
}

/** Perform a backtracking */
int backtracking(Formula *F, Graph *G, Interpretation *I, Atom atom, Action *actions) {
    ActionNode *action = actions->first, *prev;
    // Pull actions
    while(action != NULL && (action->clause != NULL || F->attempts[action->atom] == BOTH || action->atom != atom)) {
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
            G->nodes[action->atom] = NULL;
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
