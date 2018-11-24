/*H*
 * 
 * FILENAME: cdcl.h
 * DESCRIPTION: Conflict-driven clause learning algorithm
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 24.11.2018
 * 
 *H*/

#include "structures.h"



#ifndef SAT_CDCL_H
#define SAT_CDCL_H



/** Data structures for implication graphs */
typedef struct CDCL_Node {
	Atom atom;					    // Atom of the node
	Bool value;				        // Value for the atom
	Decision decision;			    // Kind of decision
	int level;					    // Decision level
	Clause *antecedents;		    // Array of antecedent nodes of the node
	int degree;				        // Number of antecedents
} CDCL_Node;

typedef struct CDCL_Graph {
	CDCL_Node **nodes;			    // Nodes of the graph
	int size;					    // Maximum number of nodes
	int decision_level;		        // Current decision level
} CDCL_Graph;



#endif



/**
  * 
  * This function creates an implication graph of $nbvar variables,
  * returning a pointer to a newly initialized CDCL_Graph struct.
  * 
  **/
CDCL_Graph *cdcl_graph_alloc(int nbvar);

/**
  * 
  * This function frees a previously allocated graph $G.
  * The graph nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the graph nodes will not be deallocated.
  * 
  **/
void cdcl_graph_free(CDCL_Graph *G);

/**
  * 
  * This function sets the value of the $atom-th node of a graph.
  * If the $decision is CONFLICTIVE, the node is set in the last
  * position of the array of nodes (the implication graph can only
  * have at most one CONFLICTIVE node). If there is a previous node
  * in the same index, it will not be deallocated.
  * 
  **/
void cdcl_graph_set_node(CDCL_Graph *G, Atom atom, Bool value, int level, Decision decision, Clause *clause);

/**
  * 
  * This function checks the satisfiability of the formula $F,
  * following the Conflict-driven clause learning algorithm. If $F
  * is satisfiable, the function returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  * The original formula $F can be modified by assertion or retraction
  * of clauses and literals, or by addition of new clauses.
  * 
  **/
int cdcl_check_sat(Formula *F);

/**
  * 
  * This function propagates the value $value of the variable $atom in
  * the formula $F. If the literal $atom in a clause is POSITIVE and
  * $value is TRUE, or the literal $atom is NEGATIVE and $value is FALSE,
  * the clause is satisfied and retracted from $F. Otherwise, the literal
  * is retracted from the clause. If the length of a clause becomes zero
  * (empty clause), $F is unsatisfiable and the function returns 0.
  * Otherwise, the function succeeds and returns 1.
  * 
  **/
int cdcl_replace_variable(Formula *F, CDCL_Graph *G, Trace *trace, Atom atom, Bool value);

/** 
  * 
  * This function selects a variable from the formula $F and assigns it a
  * value (TRUE or FALSE). The value is propagated in $F and the function
  * returns the result of the propagation.
  * 
  **/
int cdcl_decision_state(Formula *F, CDCL_Graph *G, Trace *trace);

/**
  * If an unsatisfied clause has all but one of its variables evaluated
  * at false (unit clause), then the free variable must be true in order
  * for the clause to be true. This function iterates the unit clauses
  * of the formula $F and tries to satisfy all of them. If any unit 
  * clause can not be satisfied, the function fails and returns 0.
  * Otherwise, the function succeeds and returns 1.
  * 
  **/
int cdcl_unit_propagation(Formula *F, CDCL_Graph *G, Trace *trace);

/**
  * 
  * This function analyzes the conflict produced in the formula $F
  * with the implication graph $G, and generates a new clause. The 
  * learnt clause is asserted in $F and returned by the function.
  * 
  **/
Clause *cdcl_analyze_conflict(Formula *F, CDCL_Graph *G, Trace *trace);

/**
  * 
  * The resolution rule is a single valid inference rule that produces
  * a new clause implied by two clauses containing complementary
  * literals. This function applies the resolution rule over the clauses
  * $clause_a and $clause_b. The first clause, $clause_a, is also used
  * as the result of the resolution. The pointer $ptr is the reference
  * to the literal node used as resolvent, and is updated to the previous
  * literal node pointed by $ptr. New literals are added at the
  * beginning of the list of literals of $clause_a, so literals of
  * $clause_a can be iterated from the last to the first in a single
  * loop, in order to resolve all literals in the decision level.
  * 
  **/
void cdcl_resolution(Clause *clause_a, Clause *clause_b, LiteralNode **ptr);

/**
  * 
  * This function performs a backtracking in the formula $F until the
  * clause $clause becomes unit. This function also  frees the undone
  * trace nodes from the trace $trace, and the undone graph nodes from 
  * the implication graph $G. If the clause $clause is unit after the
  * backtracking, the function succeeds and return 1. Otherwise the
  * function fails and return 0.
  * 
  **/
int cdcl_backtracking(Formula *F, CDCL_Graph *G, Trace *trace, Clause *clause);
