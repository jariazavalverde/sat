/*H*
 * 
 * FILENAME: main.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * COMPILING: gcc -I/usr/include -L/usr/lib main.c read.h read.c structures.h sat.h sat.c -o sat
 * 
 *H*/

#include <stdio.h>
#include "read.h"
#include "structures.h"
#include "sat.h"



int main(int argc, char **argv) {
	Formula F;
	int read;
	read = dimacs_read_file("../../examples/in/c2.cnf", &F);
	if(read == 0) {
		write_formula(&F);
		printf("\nNumber of clauses: %d\n", F.length);
		printf("Number of variables: %d\n", F.variables);
	} else {
		printf("error %d\n", read);
	}
	return 0;
}
