/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 27.07.2018
 * 
 *H*/

#include <stdlib.h>
#include "structures.h"


/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I);
/** Propagate a value of a variable */
int replace_variable(Formula *F, Interpretation *I, Action *actions, Atom atom, Bool value);
/** 1-Literal rule (unit propagation) */
int unit_propagation(Formula *F, Interpretation *I, Action *actions);
/** Positive-Negative rule */
int positive_negative(Formula *F, Interpretation *I, Action *actions);
/** Split cases */
int split_cases(Formula *F, Interpretation *I, Action *actions);
/** Remove a clause from a formula */
void remove_clause(Formula *F, Action *actions, Clause *clause, Atom atom);
/** Remove a literal from a clause */
void remove_literal(Formula *F, Action *actions, Clause *clause, Atom atom);
/** Add a clause to a formula */
void add_clause(Formula *F, Clause *clause);
/** Add a literal to a clause */
void add_literal(Formula *F, Clause *clause, Atom atom, Literal literal);
/** Update count of positive-negative literals in a formula */
void update_count_positive_negative(Formula *F, Clause *clause, Operation op);
/** Prepend a new action */
void push_action(Action *actions, Clause *clause, Atom atom, Literal literal);
/** Perform a backtracking */
int backtracking(Formula *F, Interpretation *I, Action *actions);
