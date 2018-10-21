/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 21.10.2018
 * 
 *H*/

#include <stdlib.h>
#include <time.h>
#include "structures.h"



/**
  *
  * This function checks the satisfiability of the formula $F. If $F
  * is satisfiable, the functions returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  *  
  **/
int formula_check_sat(Formula *F);

/**
  * 
  * This function checks the satisfiability of the formula $F,
  * following the Conflict-driven clause learning algorithm. If $F
  * is satisfiable, the functions returns 1 and the interpretation for
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
int cdcl_replace_variable(Formula *F, Graph *G, Trace *trace, Atom atom, Bool value);

/** 
  * 
  * This function selects a variable from the formula $F and assigns it a
  * value (TRUE or FALSE). The value is propagated in $F and the function
  * returns the result of the propagation.
  * 
  **/
int cdcl_decision_state(Formula *F, Graph *G, Trace *trace);

/**
  * If an unsatisfied clause has all but one of its variables evaluated
  * at false (unit clause), then the free variable must be true in order
  * for the clause to be true. This function iterates the unit clauses
  * of the formula $F and tries to satisfy all of them. If any unit 
  * clause can not be satisfied, the function fails and returns 0.
  * Otherwise, the function succeeds and returns 1.
  * 
  **/
int cdcl_unit_propagation(Formula *F, Graph *G, Trace *trace);

/**
  * 
  * This function analyzes the conflict produced in the formula $F
  * with the implication graph $G, and generates a new clause. The 
  * learnt clause is asserted in $F and returned by the function.
  * 
  **/
Clause *cdcl_analyze_conflict(Formula *F, Graph *G, Trace *trace);

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
int cdcl_backtracking(Formula *F, Graph *G, Trace *trace, Clause *clause);
