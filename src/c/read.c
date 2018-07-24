/*H*
 * 
 * FILENAME: read.c
 * DESCRIPTION: Read propositional formulas in DIMACS format
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * 
 *H*/

#include <stdio.h>
#include <stdlib.h>
#include "read.h"



/** Read a formula in DIMACS format */
int dimacs_read_file(char *path, Formula *F) {
    FILE *file;
    int i, length, var, nbvar, nbclauses;
    char ch;
    Atom atom;
    Clause *clause;
    Literal *literals, literal;
    LiteralNode *literal_node, *last_literal_node;
    ClauseNode *clause_node = NULL, *last_clause_node;
    // Open file
    file = fopen(path, "r");
    // If file not exists, error
    if(!file)
        return 1;
    // Drop comments
    ch = fgetc(file);
    while(ch == 'c') {
		while(ch != '\n' && ch != EOF)
			ch = fgetc(file);
		ch = fgetc(file);
	}
	while(ch == '\n')
	    ch = fgetc(file);
    // Read header
    if(ch != 'p' || fscanf(file, " cnf %d %d\n", &nbvar, &nbclauses) != 2)
		return 2;
	F->length = nbclauses;
	F->variables = nbvar;
    F->lst_clauses = NULL;
    // Read clauses
    for(i = 0; i < nbclauses; ++i) {
		length = 0;
		last_clause_node = clause_node;
		clause = malloc(sizeof(Clause));
		literals = malloc(nbvar*sizeof(Literal));
		clause_node = malloc(sizeof(ClauseNode));
		clause->id = i;
		clause->arr_literals = literals;
		clause->lst_literals = NULL;
		literal_node = NULL;
		if(F->lst_clauses == NULL)
			F->lst_clauses = clause_node;
        while(fscanf(file, "%d", &var) == 1 && var != 0) {
			length++;
			atom = var > 0 ? var : -var;
			atom--;
			literal = var > 0 ? POSITIVE : NEGATIVE;
			last_literal_node = literal_node;
			literal_node = malloc(sizeof(LiteralNode));
			literal_node->atom = atom; 
			literal_node->literal = literal;
			literal_node->next = NULL;
			literal_node->prev = last_literal_node;
			if(clause->lst_literals == NULL)
			    clause->lst_literals = literal_node;
			if(last_literal_node != NULL)
			    last_literal_node->next = literal_node;
			literals[atom] = literal;
			fgetc(file); // read space
		}
		clause->length = length;
		clause_node->clause = clause;
		clause_node->next = NULL;
		clause_node->prev = last_clause_node;
		if(last_clause_node != NULL)
			last_clause_node->next = clause_node;
		fgetc(file); // read break line
	}
	// Close file
    fclose(file);
    return 0;
}

/** Write a formula for the stardard output */
void write_formula(Formula *F) {
    LiteralNode *literal_node;
    ClauseNode *clause_node = F->lst_clauses;
    while(clause_node != NULL) {
		printf("( ");
        literal_node = clause_node->clause->lst_literals;
        while(literal_node != NULL) {
			write_literal(literal_node->atom, literal_node->literal);
			literal_node = literal_node->next;
		}
		printf(")");
		clause_node = clause_node->next;
    }
}

/** Write a literal for the stardard output */
void write_literal(Atom atom, Literal literal) {
	printf(literal == NEGATIVE ? "-%d " : "%d ", atom+1);
}

