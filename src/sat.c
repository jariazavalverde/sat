/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
 * 
 *H*/

#include "sat.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F) {
    int success = 1, decide = 1;
    Graph *G;
    Clause *clause;
    Trace *trace;
    // Initialize structures
    trace = trace_alloc(F->variables);
    G = graph_alloc(F->variables);
    // Unit propagation
    if(unit_propagation(F, G, trace) == 0)
		return 0;
    while(F->length > 0 && success) {
        // Split cases
        if(decide)
			success = split_cases(F, G, trace);
		decide = 1;
        // Unit propagation
        if(success)
			success = unit_propagation(F, G, trace);
        // Backtracking
        if(!success && G->decision_level > 0) {
			clause = analyze_conflict(F, G, trace);
            success = backtracking(F, G, trace, clause);
            decide = 0;
        }
    }
    // Free structures
    trace_free(trace);
    graph_free(G);
    // Return result
    return F->length == 0;
}

/** Propagate a value of a variable */
int replace_variable(Formula *F, Graph *G, Trace *trace, Atom atom, Bool value) {
    Literal literal;
    Clause *clause;
    ClauseNode *node = F->occurrences[atom];
    // Assign interpretation
    F->interpretation[atom] = value;
    // Iterate clauses containing the literal
    while(node != NULL) {
        clause = node->clause;
        if(!F->sat_clauses[clause->id]) {
            literal = clause->arr_literals[atom]->literal;
            // If same sign of literal, remove clause
            if(literal == NEGATIVE && value == FALSE || literal == POSITIVE && value == TRUE) {
                formula_retract_clause(F, clause->id);
                trace_push(trace, clause, atom, NONE);
            // else, remove the literal from the clause
            } else {
                if(clause->length == 1) {
					// Add conflictive node
					graph_set_node(G, atom, value, G->decision_level, CONFLICTIVE, clause);
                    return 0;
                }
                clause_retract_literal(F, clause->id, atom);
				trace_push(trace, clause, atom, clause->arr_literals[atom]->literal);
            }
        }
        // Update node
        node = node->next;
    }
    return 1;
}

/** Unit propagation */
int unit_propagation(Formula *F, Graph *G, Trace *trace) {
    LiteralNode *literal_node;
    ClauseNode *clause_node, *next;
    Clause *clause;
    Atom atom;
    Bool value;
    int change = 1;
    while(change) {
		change = 0;
		clause_node = F->lst_clauses;
		F->lst_unitaries = NULL;
		// Iterate unitary clauses
		while(clause_node != NULL) {
			// Update unitary nodes
			clause = clause_node->clause;
			literal_node = clause->lst_literals;
			atom = literal_node->atom;
			// Check if variable has value
			if(F->sat_clauses[clause->id] == 0 && clause->length == 1 && F->interpretation[atom] == UNKNOWN) {
				change = 1;
				value = literal_node->literal == NEGATIVE ? FALSE : TRUE;
				// Push action
				trace_push(trace, NULL, atom, NONE);
				// Add graph node
				graph_set_node(G, atom, value, G->decision_level, FORCED, clause);
				// Replace variable
				if(replace_variable(F, G, trace, atom, value) == 0)
					return 0;
			}
			clause_node = clause_node->next;
		}
	}
    return 1;
}

/** Split cases */
int split_cases(Formula *F, Graph *G, Trace *trace) {
    Atom atom;
    Literal literal;
    Bool value;
    ClauseNode *clause_node;
    LiteralNode *literal_node;
    // Get uninterpreted literal
    clause_node = F->lst_clauses;
    if(clause_node != NULL) {
		// Update level of the implication graph
		G->decision_level++;
		literal_node = clause_node->clause->lst_literals;
		if(literal_node != NULL) {
			atom = literal_node->atom;
			literal = literal_node->literal;
			value = literal == NEGATIVE ? FALSE : TRUE;
			trace_push(trace, NULL, atom, NONE);
			graph_set_node(G, atom, value, G->decision_level, ARBITRARY, NULL);
			return replace_variable(F, G, trace, atom, value);
		}
		return 0;
    }
    return 1;
}

/** Analyze the conflict in the implication graph */
Clause *analyze_conflict(Formula *F, Graph *G, Trace *trace) {
	Clause *clause = malloc(sizeof(Clause));
	ClauseNode *clause_node, *unitary, *occurrence;
	LiteralNode *literal_node = NULL, *first_literal_node = NULL, *last_literal_node = NULL, *prev, *next;
	LiteralNode **arr_literals = malloc(F->variables * sizeof(LiteralNode*));
	GraphNode *node;
	Atom atom, resolvent;
	Literal literal;
	int i, level, length, degree, match;
	// Initialize array of literals
	for(i = 0; i < F->variables; i++)
		arr_literals[i] = NULL;
	// Get conflictive clause
	node = G->nodes[G->size];
	degree = node->degree;
	length = degree;
	level = node->level;
	// Initialize new clause
	for(i = 0; i < degree; i++) {
		atom = node->antecedents->literals[i];
		literal_node = malloc(sizeof(LiteralNode));
		arr_literals[atom] = literal_node;
		literal_node->atom = atom;
		literal_node->literal = node->antecedents->arr_literals[atom]->literal;
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
		while(first_literal_node != NULL && (G->nodes[first_literal_node->atom]->level != level || G->nodes[first_literal_node->atom]->degree == 0))
			first_literal_node = first_literal_node->prev;
		if(first_literal_node != NULL) {
			resolvent = first_literal_node->atom;
			node = G->nodes[resolvent];
			degree = node->degree;
			for(i = 0; i < degree; i++) {
				atom = node->antecedents->literals[i];
				literal = node->antecedents->arr_literals[atom]->literal;
				if(arr_literals[atom] == NULL) {
					length++;
					literal_node = malloc(sizeof(LiteralNode));
					arr_literals[atom] = literal_node;
					literal_node->atom = atom;
					literal_node->literal = literal;
					literal_node->prev = NULL;
					literal_node->next = last_literal_node;
					if(last_literal_node != NULL)
						last_literal_node->prev = literal_node;
					last_literal_node = literal_node;
				} else if(literal != arr_literals[atom]->literal) {
					length--;
					next = arr_literals[atom]->next;
					prev = arr_literals[atom]->prev;
					if(prev != NULL)
						prev->next = next;
					else
						last_literal_node = next;
					if(next != NULL)
						next->prev = prev;
					if(first_literal_node == arr_literals[atom])
						first_literal_node = prev;
					free(arr_literals[atom]);
					arr_literals[atom] = NULL;
				}
			}
		}
	}
	// Create clause
	clause->id = F->size;
	clause->literals = malloc(length*sizeof(int));
	clause->arr_literals = arr_literals;
	clause->lst_literals = NULL;
	clause->length = 0;
	clause->size = length;
	clause->lst_literals = NULL;
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
	// Realloc formula
	if(F->size > F->alloc_size) {
		F->alloc_size += F->original_size;
		F->arr_clauses = realloc(F->arr_clauses, F->alloc_size * sizeof(ClauseNode*));
		F->arr_unitaries = realloc(F->arr_unitaries, F->alloc_size * sizeof(ClauseNode*));
		F->sat_clauses = realloc(F->sat_clauses, F->alloc_size * sizeof(int));
	}
	F->sat_clauses[F->size-1] = 0;
	F->arr_clauses[F->size-1] = clause_node;
	F->arr_unitaries[F->size-1] = unitary;
	if(F->lst_clauses != NULL)
		F->lst_clauses->prev = clause_node;
	F->lst_clauses = clause_node;
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
		trace_append(trace, clause, literal_node->atom, literal_node->literal);
		literal_node = literal_node->next;
	}
	return clause;
}

/** Perform a backtracking */
int backtracking(Formula *F, Graph *G, Trace *trace, Clause *clause) {
    TraceNode *node = trace->lst_traces, *prev;
    Atom atom;
    Bool value;
    // Free conflictive node
    free(G->nodes[G->size]);
    G->nodes[G->size] = NULL;
    // Pop trace nodes
    while(node != NULL && (clause->length != 1 || clause->arr_literals[node->atom] == NULL || clause->lst_literals->atom == node->atom) ) {
        if(node->clause != NULL) {
            // Assert clause
            if(node->literal == NONE)
				formula_assert_clause(F, node->clause->id);
            // Assert literal
            else
				clause_assert_literal(F, node->clause->id, node->atom, node->literal);
        // Restore variable
        } else {
			if(G->nodes[node->atom]->decision == ARBITRARY)
				G->decision_level--;
			free(G->nodes[node->atom]);
			G->nodes[node->atom] = NULL;
            F->interpretation[node->atom] = UNKNOWN;
            trace->decisions[node->atom] = NULL;
        }
        // Update trace node
        prev = node->prev;
        free(node);
        node = prev;
        trace->length--;
    }
    trace->lst_traces = node;
    if(node != NULL)
		node->next = NULL;
	return 1;
}
