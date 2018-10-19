/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Operations with structures for SAT problem
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
 * 
 *H*/

#include "structures.h"



/**
  * 
  * This function creates a formula of $nbvar variables and $nbclauses
  * clauses, returning a pointer to a newly initialized Formula struct.
  * 
  **/
Formula *formula_alloc(int nbvar, int nbclauses) {
	int i;
	Formula *F = malloc(sizeof(Formula));
	F->arr_clauses = malloc(nbclauses * sizeof(ClauseNode*));
	F->arr_unitaries = malloc(nbclauses * sizeof(ClauseNode*));
	F->sat_clauses = malloc(nbclauses * sizeof(ClauseNode*));
	F->occurrences = malloc(nbvar * sizeof(ClauseNode*));
	F->interpretation = malloc(nbvar * sizeof(Bool));
    F->lst_clauses = NULL;
    F->lst_unitaries = NULL;
    F->length = nbclauses;
    F->size = nbclauses;
    F->original_size = nbclauses;
    F->alloc_size = nbclauses;
    F->variables = nbvar;
    for(i = 0; i < nbclauses; i++) {
		F->arr_clauses[i] = NULL;
		F->arr_unitaries[i] = NULL;
		F->sat_clauses[i] = 0;
	}
	for(i = 0; i < nbvar; i++) {
		F->occurrences[i] = NULL;
		F->interpretation[i] = UNKNOWN;
	}
    return F;
}

/**
  * 
  * This function creates a clause of $nbvar variables, returning a
  * pointer to a newly initialized Clause struct.
  * 
  **/
Clause *clause_alloc(int nbvar) {
	int i;
	Clause *clause = malloc(sizeof(Clause));
	clause->id = 0;
	clause->arr_literals = malloc(nbvar * sizeof(LiteralNode*));
	clause->lst_literals = NULL;
	clause->length = 0;
	clause->size = 0;
	clause->literals = NULL;
	for(i = 0; i < nbvar; i++)
		clause->arr_literals[i] = NULL;
	return clause;
}

/**
  * 
  * This function creates a trace of $nbvar variables, returning a
  * pointer to a newly initialized Trace struct.
  * 
  **/
Trace *trace_alloc(int nbvar) {
	int i;
	Trace *trace = malloc(sizeof(Trace));
	trace->lst_traces = NULL;
	trace->decisions = malloc(nbvar * sizeof(TraceNode*));
	trace->length = 0;
	for(i = 0; i < nbvar; i++)
		trace->decisions[i] = NULL;
	return trace;
}

/**
  * 
  * This function creates an implication graph of $nbvar variables,
  * returning a pointer to a newly initialized Graph struct.
  * 
  **/
Graph *graph_alloc(int nbvar) {
	int i;
	Graph *G = malloc(sizeof(Graph));
	G->nodes = malloc((nbvar+1) * sizeof(GraphNode));
	G->size = nbvar;
	G->decision_level = 0;
	for(i = 0; i <= nbvar; i++)
		G->nodes[i] = NULL;
	return G;
}

/**
  * 
  * This function frees a previously allocated formula $F.
  * The clause nodes underlying the formula will also be deallocated.
  * 
  **/
void formula_free(Formula *F) {
	int i;
	// Free clause nodes
	ClauseNode *clause_node, *next;
    for(i = 0; i < F->size; i++) {
		clause_free(F->arr_clauses[i]->clause);
		free(F->arr_clauses[i]);
		free(F->arr_unitaries[i]);
	}
	// Free clause nodes from occurrences
	for(i = 0; i < F->variables; i++) {
		clause_node = F->occurrences[i];
		while(clause_node != NULL) {
			next = clause_node->next;
			free(clause_node);
			clause_node = next;
		}
	}
	// Free arrays
    free(F->arr_clauses);
	free(F->arr_unitaries);
	free(F->sat_clauses);
	free(F->occurrences);
	free(F->interpretation);
	// Free formula
	free(F);
}

/**
  * 
  * This function frees a previously allocated clause $clause.
  * The literal nodes underlying the clause will also be deallocated.
  * 
  **/
void clause_free(Clause *clause) {
	int i;
	// Free literal nodes
	for(i = 0; i < clause->size; i++)
		free(clause->arr_literals[clause->literals[i]]);
	// Free arrays
	free(clause->literals);
	free(clause->arr_literals);
	// Free clause
	free(clause);
}

/**
  * 
  * This function frees a previously allocated trace $trace.
  * The trace nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the trace nodes will not be deallocated.
  * 
  **/
void trace_free(Trace *trace) {
	int i;
	TraceNode *trace_node = trace->lst_traces, *prev;
	// Free literal nodes
	while(trace_node != NULL) {
		prev = trace_node->prev;
		free(trace_node);
		trace_node = prev;
	}
	// Free arrays
	free(trace->decisions);
	// Free trace
	free(trace);
}

/**
  * 
  * This function frees a previously allocated graph $G.
  * The graph nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the graph nodes will not be deallocated.
  * 
  **/
void graph_free(Graph *G) {
	int i;
	// Free graph nodes
	for(i = 0; i <= G->size; i++)
		if(G->nodes[i] != NULL)
			free(G->nodes[i]);
	// Free arrays
	free(G->nodes);
	// Free graph
	free(G);
}

/**
  * 
  * This function retracts the clause with id $clause_id from the
  * formula $F. If the clause is unit, it is also retracted from the
  * unit clauses array of $F.
  * 
  **/
void formula_retract_clause(Formula *F, Atom clause_id) {
    ClauseNode *prev, *next;
    // Set clause as satisfied
    F->sat_clauses[clause_id] = 1;
    F->length--;
    // Retract clause node
    prev = F->arr_clauses[clause_id]->prev;
    next = F->arr_clauses[clause_id]->next;
    if(prev == NULL)
        F->lst_clauses = next;
    else
        prev->next = next;
    if(next != NULL)
        next->prev = prev;
    // Retract unit clause
    if(F->arr_clauses[clause_id]->clause->length == 1)
		formula_retract_unit_clause(F, clause_id);
}

/**
  * 
  * This function asserts the clause with id $clause_id into the
  * formula $F. If the clause is unit, it is also asserted into the
  * unit clauses array of $F.
  * 
  **/
void formula_assert_clause(Formula *F, Atom clause_id) {
    ClauseNode *clause_node = F->arr_clauses[clause_id];
    // Set clause as unsatisfied
    F->sat_clauses[clause_id] = 0;
    F->length++;
    // Assert clause node
    clause_node->prev = NULL;
    clause_node->next = F->lst_clauses;
    if(F->lst_clauses != NULL)
        F->lst_clauses->prev = clause_node;
    F->lst_clauses = clause_node;
    // Assert unit clause
    if(clause_node->clause->length == 1)
		formula_assert_unit_clause(F, clause_id);
}

/**
  * 
  * This function retracts the unit clause with id $clause_id from the
  * formula $F.
  * 
  **/
void formula_retract_unit_clause(Formula *F, int clause_id) {
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

/**
  * 
  * This function asserts the unit clause with id $clause_id into the
  * formula $F.
  * 
  **/
void formula_assert_unit_clause(Formula *F, int clause_id) {
	ClauseNode *unitary = F->arr_unitaries[clause_id];
	if(F->lst_unitaries != NULL)
		F->lst_unitaries->prev = unitary;
	unitary->next = F->lst_unitaries;
	unitary->prev = NULL;
	F->lst_unitaries = unitary;
}

/**
  * 
  * This function retracts the literal $atom from the clause with id
  * $clause_id of the formula $F. If the clause becomes unit, it is also
  * asserted into the unit clauses array of $F.
  * 
  **/
void clause_retract_literal(Formula *F, int clause_id, Atom atom) {
	LiteralNode *next, *prev, *literal;
	Clause *clause = F->arr_clauses[clause_id]->clause;
	// Retract literal node
	clause->length--;
	literal = clause->arr_literals[atom];
	prev = literal->prev;
	next = literal->next;
	if(prev == NULL)
		clause->lst_literals = next;
	else
		prev->next = next;
	if(next != NULL)
		next->prev = prev;
	// Assert unit clause
	if(clause->length == 1)
		formula_assert_unit_clause(F, clause_id);
}

/**
  * 
  * This function asserts the literal $atom into the clause with id
  * $clause_id of the formula $F. If the clause becomes unit, it is also
  * asserted into the unit clauses array of $F. If the clause was unit
  * becomes but becomes not unit, it is retracted from the unit clauses
  * array of $F.
  * 
  **/
void clause_assert_literal(Formula *F, int clause_id, Atom atom, Literal literal) {
	Clause *clause = F->arr_clauses[clause_id]->clause;
	LiteralNode *node = clause->arr_literals[atom];
	// Assert literal node
	clause->length++;
	node->prev = NULL;
	node->next = clause->lst_literals;
	if(clause->lst_literals != NULL)
		clause->lst_literals->prev = node;
	clause->lst_literals = node;
	// Assert unit clause
	if(clause->length == 1)
		formula_assert_unit_clause(F, clause_id);
	// Retract unit clause
	else if(clause->length > 1)
		formula_retract_unit_clause(F, clause_id);
}

/**
  * 
  * This function allocates and pushes a new trace node
  * ($clause, $atom, $literal) into the trace $trace.
  * 
  **/
void trace_push(Trace *trace, Clause *clause, Atom atom, Literal literal) {
    TraceNode *node = malloc(sizeof(TraceNode));
    node->clause = clause;
    node->atom = atom;
    node->literal = literal;
    node->prev = trace->lst_traces;
    node->next = NULL;
    if(trace->lst_traces != NULL)
		trace->lst_traces->next = node;
    trace->lst_traces = node;
    trace->length++;
    if(clause == NULL)
		trace->decisions[atom] = node;
}

/**
  * 
  * This function allocates and appends a new trace node
  * ($clause, $atom, $literal) into the trace $trace, before the decision
  * node assigned to the atom $atom. The decision node of an atom is
  * characterized by having a NULL clause and an UNKNOWN literal. If the
  * decision node of the atom $atom does not exist, the new trace node
  * is not appended.
  * 
  **/
void trace_append(Trace *trace, Clause *clause, Atom atom, Literal literal) {
    TraceNode *node, *assign, *next = NULL;
    assign = trace->decisions[atom];
    if(assign != NULL) {
		next = assign->next;
		node = malloc(sizeof(TraceNode));
		node->clause = clause;
		node->atom = atom;
		node->literal = literal;
		trace->length++;
		node->next = next;
		node->prev = assign;
		assign->next = node;
		if(next != NULL)
			next->prev = node;
		else
			trace->lst_traces = node;
		if(clause == NULL)
			trace->decisions[atom] = node;
	}
}

/**
  * 
  * This function sets the value of the $atom-th node of a graph.
  * If the $decision is CONFLICTIVE, the node is set in the last
  * position of the array of nodes (the implication graph can only
  * have at most one CONFLICTIVE node). If there is a previous node
  * in the same index, it will not be deallocated.
  * 
  **/
void graph_set_node(Graph *G, Atom atom, Bool value, int level, Decision decision, Clause *clause) {
	int index = decision == CONFLICTIVE ? G->size : atom;
	int i, degree = clause == NULL || decision == ARBITRARY ? 0 : clause->size;
	GraphNode *node = malloc(sizeof(GraphNode));
	G->nodes[index] = node;
	node->atom = atom;
	node->value = value;
	node->decision = decision;
	node->degree = degree;
	node->level = level;
	node->antecedents = clause;
}
