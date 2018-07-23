/*H*
 * 
 * FILENAME: sat.h
 * DESCRIPTION: Boolean satisfiability problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * 
 *H*/



// DATA STRUCTURES

/** Atom identifier */
typedef int Atom;

/** Possible states of literals stacks */
typedef enum {NONE, NEGATIVE, POSITIVE, BOTH} Literal;

/** Data structure for literals linked-lists */
typedef struct LiteralNode {
	Atom atom;                    // Atom
    Literal literal;              // Literal
    struct LiteralNode *next;     // Next literal
    struct LiteralNode *prev;     // Previous literal
} LiteralNode;

/** Data structure for clauses */
typedef struct Clause {
	Literal *arr_literals;        // Array of literals
    LiteralNode *lst_literals;    // Linked-list of literals
    int length;                   // Number of literals in the list
} Clause;

/** Data structure for clauses linked-lists */
typedef struct ClauseNode {
	Clause *clause;               // Clause
    struct ClauseNode *next;      // Next literal
    struct ClauseNode *prev;      // Previous literal
} ClauseNode;

/** Data structure for formulae */
typedef struct Formula {
    ClauseNode *clauses;          // Linked-list of clauses
    int length;                   // Number of clauses in the list
    int variables;                // Number of unique variables
    int *count_positives;         // Number of positive literals of each variable
    int *count_negatives;         // Number of negative literals of each variable
    ClauseNode *unitaries;        // Clauses with a one literal
    ClauseNode **occurrences;     // Clauses where occurs each variable
} Formula;

/** Data structure for interpretations */
typedef struct Interpretation {
    int *bindings;                // Values
    int length;                   // Number of bindings
} Interpretation;

/** */
typedef struct Action {
	Atom step;                    // Step
	Clause *clause;               // Clause removed
	Literal literal;              // Literal
	struct Action *prev;          // Previous node
} Action;



// HEADERS

/** Check satisfiability of a formula */
int check_sat(Formula *F, Interpretation *I);
/** 1-Literal rule (unit propagation) */
void unit_propagation(Formula *F, Interpretation *I, Action *actions);
/** Positive-Negative rule */
void positive_negative(Formula *F, Interpretation *I, Action *actions);
/** Remove a clause from a formula */
void remove_clause(Formula *F, Clause *clause, Action *actions);
/** Remove a literal from a clause */
void remove_literal(Formula *F, Clause *clause, Literal literal, Action *actions);
