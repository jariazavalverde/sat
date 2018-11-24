/*H*
 * 
 * FILENAME: main.c
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 24.11.2018
 * COMPILING: gcc -I/usr/include -L/usr/lib main.c io.h io.c structures.c structures.h cdcl.h cdcl.c sat.h sat.c -o sat
 * 
 *H*/

#include <stdio.h>
#include <string.h>
#include "io.h"
#include "structures.h"
#include "sat.h"



int main(int argc, char **argv) {
	FILE *stream;
	double time_spent;
	int i, sat, op_stats = 0;
	Formula *F;
	// Check minimum number of arguments
	if(argc < 2) {
		printf("Error: input file was not specified.\n");
		return 1;
	}
	// Check options
	for(i = 1; i < argc-1; i++) {
		if(strcmp(argv[i], "-st") == 0)
			op_stats = 1;
	}
	// Read formula
	stream = fopen(argv[argc-1], "r");
	// File is ok
	if(stream != NULL) {
		F = formula_fread_dimacs(stream);
		fclose(stream);
		// Formula is ok
		if(F != NULL) {
			// Check satisfiability
			sat = formula_check_sat(F);
			// Show SAT result
			printf(sat ? "sat\n" : "unsat\n");
			if(sat)
				formula_printf_interpretation(F);
			// Show statistics
			if(op_stats)
				formula_printf_statistics(F);
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
