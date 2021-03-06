/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Data structures for SAT problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 25.11.2018
 * 
 *H*/

#include <stdlib.h>



#ifndef SAT_STRUCTURES_H
#define SAT_STRUCTURES_H



/** Atom identifier */
typedef int Atom;

/** Possible states of literals */
typedef enum {NONE = -1, NEGATIVE = 0, POSITIVE = 1, BOTH = 2} Literal;

/** Boolean values */
typedef enum {UNKNOWN = -1, FALSE = 0, TRUE = 1} Bool;

/** Possible kinds of formulas */
typedef enum {CNF, CNF_2, CNF_HORN} Problem;

/** Possible algorithms */
typedef enum {AUTO, CDCL, APSVALL} Algorithm;

/** Possible kinds of nodes in implication graphs */
typedef enum {ARBITRARY, FORCED, CONFLICTIVE} Decision;

/** Data structure for literals */
typedef struct LiteralNode {
	Atom atom;					    // Atom
	Literal literal;			    // Literal
	struct LiteralNode *next;	    // Next literal
	struct LiteralNode *prev;	    // Previous literal
} LiteralNode;

/** Data structure for clauses */
typedef struct Clause {
	int id;					        // Auto-increment identifier
	int *literals;				    // Array of identifiers of literals
	LiteralNode **arr_literals;     // Array of literals
	LiteralNode *lst_literals;	    // Linked-list of literals
	int length;				        // Number of literals in the list
	int size;					    // Total number of literals
} Clause;

typedef struct ClauseNode {
	Clause *clause;			        // Clause
	struct ClauseNode *next;	    // Next literal
	struct ClauseNode *prev;	    // Previous literal
} ClauseNode;

/** Data structure for formulas */
typedef struct Formula {
	ClauseNode **arr_clauses;	    // Array of clauses
	ClauseNode *lst_clauses;	    // Linked-list of clauses
	int length;				        // Number of clauses in the list
	int nbclauses;					// Total number of clauses
	int original_size;			    // Original size
	int alloc_size;			        // Number of clauses allocated
	int nbvar;				        // Number of unique variables
	int *sat_clauses;			    // Satisfiable clauses
	ClauseNode **arr_unit_clauses;  // Array of unit clauses
	ClauseNode *lst_unit_clauses;   // Linked-list of unit clauses
	int *unit;                      // Array of identifiers of unit clauses
	ClauseNode **occurrences;	    // Clauses where occurs each variable
	Bool *interpretation;		    // Array of values for interpretation
	Problem problem;                // Type of problem (CNF, 2-CNF, HORN-CNF, ...)
	int nb_learnt_clauses;          // Total number of learnt clauses
	int nb_decisions;               // Total number of decisions
	int nb_propagations;            // Total number of unit propagations
	int nb_conflicts;               // Total number of conflicts
	double execution_time;          // Total execution time
} Formula;

/** Data structures for trace execution */
typedef struct TraceNode {
	Clause *clause;			        // Clause removed
	Atom atom;					    // Atom
	Literal literal;			    // Literal
	struct TraceNode *next;	        // Next node
	struct TraceNode *prev;	        // Previous node
} TraceNode;

typedef struct Trace {
	TraceNode *lst_traces;		    // First node
	TraceNode **decisions;		    // Array of pointer to decision nodes
	int length;				        // Number of nodes
} Trace;



#endif



/**
  * 
  * This function creates a formula of $nbvar variables and $nbclauses
  * clauses, returning a pointer to a newly initialized Formula struct.
  * 
  **/
Formula *formula_alloc(int nbvar, int nbclauses);

/**
  * 
  * This function creates a clause of $nbvar variables, returning a
  * pointer to a newly initialized Clause struct.
  * 
  **/
Clause *clause_alloc(int nbvar);

/**
  * 
  * This function creates a trace of $nbvar variables, returning a
  * pointer to a newly initialized Trace struct.
  * 
  **/
Trace *trace_alloc(int nbvar);

/**
  * 
  * This function frees a previously allocated formula $F.
  * The clause nodes underlying the formula will also be deallocated.
  * 
  **/
void formula_free(Formula *F);

/**
  * 
  * This function frees a previously allocated clause $clause.
  * The literal nodes underlying the clause will also be deallocated.
  * 
  **/
void clause_free(Clause *clause);

/**
  * 
  * This function frees a previously allocated trace $trace.
  * The trace nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the trace nodes will not be deallocated.
  * 
  **/
void trace_free(Trace *trace);

/**
  * 
  * This function appends the clause $clause into the formula $F. If
  * there is not enough space in memory, the formula $F is rellocated.
  * 
  **/
void formula_append_clause(Formula *F, Clause *clause);

/**
  * 
  * This function retracts the clause with id $clause_id from the
  * formula $F. If the clause is unit, it is also retracted from the
  * unit clauses array of $F.
  * 
  **/
void formula_retract_clause(Formula *F, Atom clause_id);

/**
  * 
  * This function asserts the clause with id $clause_id into the
  * formula $F. If the clause is unit, it is also asserted into the
  * unit clauses array of $F.
  * 
  **/
void formula_assert_clause(Formula *F, Atom clause_id);

/**
  * 
  * This function retracts the unit clause with id $clause_id from the
  * formula $F.
  * 
  **/
void formula_retract_unit_clause(Formula *F, int clause_id);

/**
  * 
  * This function asserts the unit clause with id $clause_id into the
  * formula $F.
  * 
  **/
void formula_assert_unit_clause(Formula *F, int clause_id);

/**
  * 
  * This function retracts the literal $atom from the clause with id
  * $clause_id of the formula $F. If the clause becomes unit, it is also
  * asserted into the unit clauses array of $F.
  * 
  **/
void clause_retract_literal(Formula *F, int clause_id, Atom atom);

/**
  * 
  * This function asserts the literal $atom into the clause with id
  * $clause_id of the formula $F. If the clause becomes unit, it is also
  * asserted into the unit clauses array of $F. If the clause was unit
  * but it becomes not unit, it is retracted from the unit clauses
  * array of $F.
  * 
  **/
void clause_assert_literal(Formula *F, int clause_id, Atom atom, Literal literal);

/**
  * 
  * This function allocates and pushes a new trace node
  * ($clause, $atom, $literal) into the trace $trace.
  * 
  **/
void trace_push(Trace *trace, Clause *clause, Atom atom, Literal literal);

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
void trace_append(Trace *trace, Clause *clause, Atom atom, Literal literal);


/**
  * 
  * This function returns the negation of the literal $literal.
  * 
  **/
Literal literal_not(Literal literal);
