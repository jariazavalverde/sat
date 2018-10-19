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
	G->max_level = 0;
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
  * Clause nodes underlying the trace nodes not will be deallocated.
  * 
  **/
void trace_free(Trace *trace) {
	int i;
	TraceNode *trace_node = trace->lst_traces, *next;
	// Free literal nodes
	while(trace_node != NULL) {
		next = trace_node->next;
		free(trace_node);
		trace_node = next;
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
  * Clause nodes underlying the graph nodes not will be deallocated.
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

/** Add a new node into a graph */
int add_graph_node(Graph *G, Atom atom, Bool value, int level, Decision decision, Clause *clause) {
	int index = decision == CONFLICTIVE ? G->size : atom;
	int i, degree = clause == NULL || decision == ARBITRARY ? 0 : clause->size;
	if(G->nodes[index] != NULL)
		return 0;
	GraphNode *node = malloc(sizeof(GraphNode));
	G->nodes[index] = node;
	node->atom = atom;
	node->value = value;
	node->decision = decision;
	node->degree = degree;
	node->level = level;
	node->antecedents = clause;
	return 1;
}
