/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Operations with structures for SAT problem
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 26.07.2018
 * 
 *H*/

#include "structures.h"



/** Initialiaze a new interpretation */
void init_interpretation(Interpretation *I, int length) {
    int i;
    I->length = length;
    I->bindings = malloc(length*sizeof(Bool));
    for(i = 0; i < length; i++)
        I->bindings[i] = UNKNOWN;
}
