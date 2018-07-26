/*H*
 * 
 * FILENAME: io.c
 * DESCRIPTION: Read propositional formulas in DIMACS format
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 26.07.2018
 * 
 *H*/

#include "io.h"



/** Read a formula in DIMACS format */
int dimacs_read_file(char *path, Formula *F) {
    FILE *file;
    int i, j, length, var, nbvar, nbclauses;
    char ch;
    Atom atom;
    Clause *clause;
    Literal literal;
    LiteralNode *literal_node, *last_literal_node;
    ClauseNode *clause_node = NULL, *last_clause_node, *unitaries, *occurrence;
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
    F->unitaries = NULL;
    F->count_positives = malloc(nbvar*sizeof(int));
    F->count_negatives = malloc(nbvar*sizeof(int));
    F->sat_clauses = malloc(nbvar*sizeof(int));
    F->occurrences = malloc(nbvar*sizeof(ClauseNode*));
    F->arr_clauses = malloc(nbclauses*sizeof(ClauseNode*));
    F->attempts = malloc(nbvar*sizeof(Literal));
    for(i = 0; i < nbvar; i++) {
        F->count_positives[i] = 0;
        F->count_negatives[i] = 0;
        F->sat_clauses[i] = 0;
        F->occurrences[i] = NULL;
        F->attempts[i] = NONE;
    }
    // Read clauses
    for(i = 0; i < nbclauses; i++) {
        length = 0;
        last_clause_node = clause_node;
        clause = malloc(sizeof(Clause));
        clause->arr_literals = malloc(nbvar*sizeof(LiteralNode*));
        clause_node = malloc(sizeof(ClauseNode));
        clause->id = i;
        clause->lst_literals = NULL;
        literal_node = NULL;
        F->arr_clauses[i] = clause_node;
        for(j = 0; j < nbvar; j++)
            clause->arr_literals[j] = NULL;
        if(F->lst_clauses == NULL)
            F->lst_clauses = clause_node;
        while(fscanf(file, "%d", &var) == 1 && var != 0) {
            length++;
            atom = var > 0 ? var : -var;
            atom--;
            literal = var > 0 ? POSITIVE : NEGATIVE;
            if(literal == NEGATIVE)
                F->count_negatives[atom]++;
            else
                F->count_positives[atom]++;
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
            fgetc(file); // read space
        }
        clause->length = length;
        // Unitary clause
        if(length == 1) {
            unitaries = malloc(sizeof(ClauseNode));
            unitaries->clause = clause;
            unitaries->next = F->unitaries;
            unitaries->prev = NULL;
            if(F->unitaries != NULL)
                F->unitaries->prev = unitaries;
            F->unitaries = unitaries;
        }
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

/** Write a interpretation */
void write_interpretation(Interpretation *I) {
    int i;
    Bool value;
    for(i = 0; i < I->length; i++) {
        value = I->bindings[i];
        if(value != UNKNOWN)
            printf(" %d:%s ", i+1, value == FALSE ? "false" : "true");
    }
}
