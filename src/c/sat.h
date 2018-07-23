/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * 
 *H*/

#include <stdlib.h>
#include "structures.h"


/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I);
/** 1-Literal rule (unit propagation) */
void unit_propagation(Formula *F, Interpretation *I, Action *actions);
/** Positive-Negative rule */
void positive_negative(Formula *F, Interpretation *I, Action *actions);
/** Remove a clause from a formula */
void remove_clause(Formula *F, Clause *clause, Atom atom, Action *actions);
/** Remove a literal from a clause */
void remove_literal(Formula *F, Clause *clause, Atom atom, Action *actions);
/** Update count of positive-negative literals in a formula */
void update_count_positive_negative(Formula *F, Clause *clause, Operation op);
/** Prepend a new action */
void push_action(Atom atom, Clause *clause, Literal literal, Action *actions);
