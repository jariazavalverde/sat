/*H*
 * 
 * FILENAME: read.h
 * DESCRIPTION: Read propositional formulas in DIMACS format
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * 
 *H*/

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"



/** Read a formula in DIMACS format */
int dimacs_read_file(char *path, Formula *F);
/** Write a formula for the stardard output */
void write_formula(Formula *F);
/** Write a literal for the stardard output */
void write_literal(Atom atom, Literal literal);
