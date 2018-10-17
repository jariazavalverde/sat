/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 17.10.2018
 * 
 *H*/

#include <stdlib.h>
#include "structures.h"


/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I);
/** Propagate a value of a variable */
int replace_variable(Formula *F, Graph *G, Interpretation *I, Action *actions, Atom atom, Bool value);
/** Unit propagation */
int unit_propagation(Formula *F, Graph *G, Interpretation *I, Action *actions);
/** Split cases */
int split_cases(Formula *F, Graph *G, Interpretation *I, Action *actions);
/** Analyze the conflict in the implication graph */
Clause *analyze_conflict(Formula *F, Graph *G, Interpretation *I, Action *actions);
/** Remove a clause from a formula */
void remove_clause(Formula *F, Action *actions, Clause *clause, Atom atom);
/** Remove a literal from a clause */
void remove_literal(Formula *F, Action *actions, Clause *clause, Atom atom);
/** Remove unitary clause */
void remove_unitary_clause(Formula *F, int clause_id);
/** Add a clause to a formula */
void add_clause(Formula *F, Clause *clause);
/** Add a literal to a clause */
void add_literal(Formula *F, Clause *clause, Atom atom, Literal literal);
/** Prepend a new action */
void push_action(Action *actions, Clause *clause, Atom atom, Literal literal);
/** Prepend a new action after assignment */
void push_action_after(Action *actions, Clause *clause, Atom atom, Literal literal);
/** Add unitary clause */
void add_unitary_clause(Formula *F, int clause_id);
/** Perform a backtracking */
int backtracking(Formula *F, Graph *G, Interpretation *I, Clause *clause, Action *actions);
