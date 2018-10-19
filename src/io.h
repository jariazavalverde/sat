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



/**
  * 
  * This function reads and allocates a formula in DIMACS format from
  * the from the open stream $stream. If the program is not in DIMACS
  * format, it returns a NULL pointer.
  * 
  **/
Formula *formula_fread_dimacs(FILE *stream);

/**
  * 
  * This function writes the clauses of the formula $F into the
  * standard output.
  * 
  **/
void formula_printf(Formula *F);

/**
  * 
  * This function writes the literals of the clause $clause into the
  * standard output.
  * 
  **/
void clause_printf(Clause *clause);

/**
  * 
  * This function writes the literal ($atom, $literal) into the
  * standard output. A positive literal is represented by its atom
  * identifier. A negative literal is represented by its atom identifier
  * preceded by the minus "-" sign.
  * 
  **/
void literal_printf(Atom atom, Literal literal);

/**
  * 
  * This function writes the interpretation of the formula $F into the
  * standard output. If a variable has no assigned value, it is not
  * written.
  * 
  **/
void formula_printf_interpretation(Formula *F);
