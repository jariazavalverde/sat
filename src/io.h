/*H*
 * 
 * FILENAME: io.h
 * DESCRIPTION: Read propositional formulas in DIMACS format
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
 * 
 *H*/

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"



/** Read a formula in DIMACS format */
Formula *formula_read_dimacs(char *path);
/** Write a formula for the stardard output */
void formula_write(Formula *F);
/** Write a clause for the stardard output */
void clause_write(Clause *clause);
/** Write a literal for the stardard output */
void literal_write(Atom atom, Literal literal);
/** Write a interpretation for the stardard output */
void formula_write_interpretation(Formula *F);
