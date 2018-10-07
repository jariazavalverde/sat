/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Data structures for SAT problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 07.10.2018
 * 
 *H*/



#include <stdlib.h>



#ifndef SAT_STRUCTURES_H
#define SAT_STRUCTURES_H



/** Atom identifier */
typedef int Atom;

/** Possible states of literals */
typedef enum {NONE = -1, NEGATIVE = 0, POSITIVE = 1, BOTH = 2} Literal;

/** Boolean values */
typedef enum {UNKNOWN = -1, FALSE = 0, TRUE = 1} Bool;

/** Possible operations */
typedef enum {REMOVE, ADD} Operation;

/** Data structure for literals linked-lists */
typedef struct LiteralNode {
	Atom atom;                    // Atom
    Literal literal;              // Literal
    struct LiteralNode *next;     // Next literal
    struct LiteralNode *prev;     // Previous literal
} LiteralNode;

/** Data structure for clauses */
typedef struct Clause {
	int id;                       // Auto-increment identifier
	LiteralNode **arr_literals;   // Array of literals
    LiteralNode *lst_literals;    // Linked-list of literals
    int length;                   // Number of literals in the list
} Clause;

/** Data structure for clauses linked-lists */
typedef struct ClauseNode {
	Clause *clause;               // Clause
    struct ClauseNode *next;      // Next literal
    struct ClauseNode *prev;      // Previous literal
} ClauseNode;

/** Data structure for formulas */
typedef struct Formula {
    ClauseNode **arr_clauses;     // Array of clauses
    ClauseNode *lst_clauses;      // Linked-list of clauses
    int length;                   // Number of clauses in the list
    int variables;                // Number of unique variables
    int *sat_clauses;             // Satisfiable clauses
    int *count_positives;         // Number of positive literals of each variable
    int *count_negatives;         // Number of negative literals of each variable
    Literal *attempts;            // Control for backtracking
    ClauseNode *unitaries;        // Clauses with a one literal
    ClauseNode **occurrences;     // Clauses where occurs each variable
    Atom selected_atom;           // Selected atom for iterate
} Formula;

/** Data structure for interpretations */
typedef struct Interpretation {
    Bool *bindings;               // Values
    int length;                   // Number of bindings
} Interpretation;

/** Data structures for node actions */
typedef struct ActionNode {
	Clause *clause;               // Clause removed
	Atom atom;                    // Atom
	Literal literal;              // Literal
	struct ActionNode *prev;      // Previous node
	int id;
} ActionNode;

/** Data structures for record actions */
typedef struct Action {
	ActionNode *first;            // First node
	int length;                   // Number of nodes
	int last_id;
} Action;



#endif



/** Initialiaze a new interpretation */
void init_interpretation(Interpretation *I, int length);
