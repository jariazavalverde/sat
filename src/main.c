/*H*
 * 
 * FILENAME: main.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 27.07.2018
 * COMPILING: gcc -I/usr/include -L/usr/lib main.c io.h io.c structures.c structures.h sat.h sat.c -o sat
 * 
 *H*/

#include <stdio.h>
#include <time.h>
#include "io.h"
#include "structures.h"
#include "sat.h"



int main(int argc, char **argv) {
	clock_t begin, end;
	double time_spent;
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
		begin = clock();
		sat = check_sat(&F, &I);
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Execution time: %f seconds\n", time_spent);
        printf(sat ? "SAT\n" : "UNSAT\n");
		write_interpretation(&I);
		printf("\n");
	} else {
		printf("error %d\n", read);
	}
	return 0;
}
