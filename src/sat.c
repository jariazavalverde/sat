/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 24.11.2018
 * 
 *H*/

#include "sat.h"



/**
  *
  * This function checks the satisfiability of the formula $F. If $F
  * is satisfiable, the function returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  *  
  **/
int formula_check_sat(Formula *F) {
	clock_t begin, end;
	int sat;
	begin = clock();
	sat = cdcl_check_sat(F);
	end = clock();
	F->execution_time = (double)(end - begin) / CLOCKS_PER_SEC;
	return sat;
}
