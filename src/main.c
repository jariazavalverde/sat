/*H*
 * 
 * FILENAME: main.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
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
	FILE *stream;
	clock_t begin, end;
	double time_spent;
	int i, sat, op_time = 0, op_info = 0;
	Formula *F;
	// Check minimum number of arguments
	if(argc < 2) {
		printf("Error: input file was not specified.\n");
		return 1;
	}
	// Check options
	for(i = 2; i < argc; i++) {
		if(strcmp(argv[i], "-t") == 0)
			op_time = 1;
		else if(strcmp(argv[i], "-i") == 0)
			op_info = 1;
	}
	// Read formula
	stream = fopen(argv[1], "r");
	// Formula is ok
	if(stream != NULL) {
		F = formula_fread_dimacs(stream);
		fclose(stream);
		if(F != NULL) {
			// Show information
			if(op_info) {
				formula_printf(F);
				printf("\nNumber of clauses: %d\n", F->length);
				printf("Number of variables: %d\n", F->variables);
			}
			begin = clock();
			// Check satisfiability
			sat = formula_check_sat(F);
			end = clock();
			// Show execution time
			if(op_time) {
				time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
				printf("Execution time: %f seconds.\n", time_spent);
			}
			// Show SAT result
			printf(sat ? "SAT\n" : "UNSAT\n");
			if(sat) {
				formula_printf_interpretation(F);
				printf("\n");
			}
			// Free formula
			formula_free(F);
		// Error reading formula
		} else {
			printf("Error: file must be in DIMACS format.\n");
		}
	// Error opening file
	} else {
		printf("Error: failed to open file.\n");
	}
	return 0;
}
