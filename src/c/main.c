/*H*
 * 
 * FILENAME: main.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 26.07.2018
 * COMPILING: gcc -I/usr/include -L/usr/lib main.c io.h io.c structures.c structures.h sat.h sat.c -o sat
 * 
 *H*/

#include <stdio.h>
#include "io.h"
#include "structures.h"
#include "sat.h"



int main(int argc, char **argv) {
	int sat;
	Formula F;
	Interpretation I;
	int read;
	if(argc < 2)
		return 1;
	read = dimacs_read_file(argv[1], &F);
	if(read == 0) {
		write_formula(&F);
		printf("\nNumber of clauses: %d\n", F.length);
		printf("Number of variables: %d\n", F.variables);
		sat = check_sat(&F, &I);
		write_formula(&F);
        printf(sat ? "SAT\n" : "UNSAT\n");
		write_interpretation(&I);
		printf("\n");
	} else {
		printf("error %d\n", read);
	}
	return 0;
}
