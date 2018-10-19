/*H*
 * 
 * FILENAME: io.c
 * DESCRIPTION: Read propositional formulas in DIMACS format
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 20.10.2018
 * 
 *H*/

#include "io.h"



/**
  * 
  * This function reads and allocates a formula in DIMACS format from
  * the from the open stream $stream. If the program is not in DIMACS
  * format, it returns a NULL pointer.
  * 
  **/
Formula *formula_fread_dimacs(FILE *stream) {
	Formula *F;
	int i, j, length, var, nbvar, nbclauses;
	char ch;
	Atom atom;
	Clause *clause;
	Literal literal;
	LiteralNode *literal_node, *last_literal_node;
	ClauseNode *clause_node = NULL, *last_clause_node, *unit_clause, *occurrence;
	// Check stream
	if(stream == NULL)
		return NULL;
	// Drop comments
	ch = fgetc(stream);
	while(ch == 'c') {
		while(ch != '\n' && ch != EOF)
			ch = fgetc(stream);
		ch = fgetc(stream);
	}
	while(ch == '\n')
		ch = fgetc(stream);
	// Read header
	if(ch != 'p' || fscanf(stream, " cnf %d %d\n", &nbvar, &nbclauses) != 2)
		return NULL;
	// Create formula
	F = formula_alloc(nbvar, nbclauses);
	// Read clauses
	for(i = 0; i < nbclauses; i++) {
		F->sat_clauses[i] = 0;
		length = 0;
		last_clause_node = clause_node;
		clause = clause_alloc(nbvar);
		clause->id = i;
		clause_node = malloc(sizeof(ClauseNode));
		literal_node = NULL;
		F->arr_clauses[i] = clause_node;
		if(F->lst_clauses == NULL)
			F->lst_clauses = clause_node;
		while(fscanf(stream, "%d", &var) == 1 && var != 0) {
			atom = var > 0 ? var : -var;
			atom--;
			if(clause->arr_literals[atom] == NULL) {
				length++;
				literal = var > 0 ? POSITIVE : NEGATIVE;
				last_literal_node = literal_node;
				literal_node = malloc(sizeof(LiteralNode));
				clause->arr_literals[atom] = literal_node;
				literal_node->atom = atom; 
				literal_node->literal = literal;
				literal_node->next = NULL;
				literal_node->prev = last_literal_node;
				if(clause->lst_literals == NULL)
					clause->lst_literals = literal_node;
				if(last_literal_node != NULL)
					last_literal_node->next = literal_node;
				// Occurrence
				occurrence = malloc(sizeof(ClauseNode));
				occurrence->clause = clause;
				occurrence->next = F->occurrences[atom];
				occurrence->prev = NULL;
				if(F->occurrences[atom] != NULL)
					F->occurrences[atom]->prev = occurrence;
				F->occurrences[atom] = occurrence;
			}
			fgetc(stream); // read space
		}
		clause->length = length;
		clause->size = length;
		clause->literals = malloc(length * sizeof(int));
		literal_node = clause->lst_literals;
		for(j = 0; j < length; j++) {
			clause->literals[j] = literal_node->atom;
			literal_node = literal_node->next;
		}
		// Unitary clause
		unit_clause = malloc(sizeof(ClauseNode));
		unit_clause->clause = clause;
		unit_clause->next = NULL;
		unit_clause->prev = NULL;
		F->arr_unit_clauses[i] = unit_clause;
		if(length == 1) {
			unit_clause->next = F->lst_unit_clauses;
			if(F->lst_unit_clauses != NULL)
				F->lst_unit_clauses->prev = unit_clause;
			F->lst_unit_clauses = unit_clause;
		}
		clause_node->clause = clause;
		clause_node->next = NULL;
		clause_node->prev = last_clause_node;
		if(last_clause_node != NULL)
			last_clause_node->next = clause_node;
		fgetc(stream); // read break line
	}
	// Return the formula
	return F;
}

/**
  * 
  * This function writes the clauses of the formula $F into the
  * standard output.
  * 
  **/
void formula_printf(Formula *F) {
	ClauseNode *clause_node = F->lst_clauses;
	while(clause_node != NULL) {
		clause_printf(clause_node->clause);
		clause_node = clause_node->next;
	}
}

/**
  * 
  * This function writes the literals of the clause $clause into the
  * standard output.
  * 
  **/
void clause_printf(Clause *clause) {
	LiteralNode *literal_node;
	printf("( ");
	literal_node = clause->lst_literals;
	while(literal_node != NULL) {
		literal_printf(literal_node->atom, literal_node->literal);
		literal_node = literal_node->next;
	}
	printf(")");
}

/**
  * 
  * This function writes the literal ($atom, $literal) into the
  * standard output. A positive literal is represented by its atom
  * identifier. A negative literal is represented by its atom identifier
  * preceded by the minus "-" sign.
  * 
  **/
void literal_printf(Atom atom, Literal literal) {
	printf(literal == NEGATIVE ? "-%d " : "%d ", atom+1);
}

/**
  * 
  * This function writes the interpretation of the formula $F into the
  * standard output. If a variable has no assigned value, it is not
  * written.
  * 
  **/
void formula_printf_interpretation(Formula *F) {
	int i;
	Bool value;
	for(i = 0; i < F->variables; i++) {
		value = F->interpretation[i];
		if(value == TRUE)
			printf("%d ", i+1);
		else if(value == FALSE)
			printf("-%d ", i+1);
	}
}
