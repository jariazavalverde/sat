/*H*
 * 
 * FILENAME: main.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 08.10.2018
 * COMPILING: gcc -I/usr/include -L/usr/lib main.c io.h io.c structures.c structures.h sat.h sat.c -o sat
 * 
 *H*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "io.h"
#include "structures.h"
#include "sat.h"



int main(int argc, char **argv) {
	clock_t begin, end;
	double time_spent;
	int i, sat, op_time = 0, op_info = 0;
	Formula F;
	Interpretation I;
	int read;
	// Check minimum number of arguments
	if(argc < 2) {
		printf("Error: input file was not specified.\n");
		return 1;
	}
	// Options
	for(i = 2; i < argc; i++) {
		if(strcmp(argv[i], "-t") == 0)
			op_time = 1;
		else if(strcmp(argv[i], "-i") == 0)
			op_info = 1;
	}
	// Read file
	read = dimacs_read_file(argv[1], &F);
	switch(read) {
		case 0:
			// Show information
			if(op_info) {
				write_formula(&F);
				printf("\nNumber of clauses: %d\n", F.length);
				printf("Number of variables: %d\n", F.variables);
			}
			begin = clock();
			// Check satisfiability
			sat = check_sat(&F, &I);
			end = clock();
			// Show execution time
			if(op_time) {
				time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("Execution time: %f seconds.\n", time_spent);
			}
			// Show SAT result
			printf(sat ? "SAT\n" : "UNSAT\n");
			if(sat) {
				write_interpretation(&I);
				printf("\n");
			}
			break;
		case 1: printf("Error: no such file or directory.\n"); break;
		default: printf("Error: file must be in DIMACS format.\n"); break;
	}
	return 0;
}
