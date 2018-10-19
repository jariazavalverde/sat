/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
 * 
 *H*/

#include <stdlib.h>
#include "structures.h"



/** Check satisfiability of a formula */
int check_sat(Formula *F);
/** Propagate a value of a variable */
int replace_variable(Formula *F, Graph *G, Trace *trace, Atom atom, Bool value);
/** Unit propagation */
int unit_propagation(Formula *F, Graph *G, Trace *trace);
/** Split cases */
int split_cases(Formula *F, Graph *G, Trace *trace);
/** Analyze the conflict in the implication graph */
Clause *analyze_conflict(Formula *F, Graph *G, Trace *trace);
/** Perform a backtracking */
int backtracking(Formula *F, Graph *G, Trace *trace, Clause *clause);
