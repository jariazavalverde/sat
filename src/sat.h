/*H*
 * 
 * FILENAME: sat.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 24.11.2018
 * 
 *H*/

#include <time.h>
#include "cdcl.h"



/**
  *
  * This function checks the satisfiability of the formula $F. If $F
  * is satisfiable, the function returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  *  
  **/
int formula_check_sat(Formula *F);
