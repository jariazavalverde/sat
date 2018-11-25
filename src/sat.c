/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 25.11.2018
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
int formula_check_sat(Formula *F, Algorithm algorithm) {
	clock_t begin, end;
	int sat;
	begin = clock();
	if(F->problem == CNF_2 && (algorithm == APSWALL || algorithm == AUTO))
		sat = sat2_check_sat(F);
	else
		sat = cdcl_check_sat(F);
	end = clock();
	F->execution_time = (double)(end - begin) / CLOCKS_PER_SEC;
	return sat;
}
