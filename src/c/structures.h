/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Data structures for SAT problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 23.07.2018
 * 
 *H*/

#ifndef SAT_STRUCTURES_H
#define SAT_STRUCTURES_H



/** Atom identifier */
typedef int Atom;

/** Possible states of literals */
typedef enum {NONE, NEGATIVE, POSITIVE, BOTH} Literal;

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
	Literal *arr_literals;        // Array of literals
    LiteralNode *lst_literals;    // Linked-list of literals
    int *dropped_literals;        // Dropped literals
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
    Clause *arr_clauses;          // Array of clauses
    ClauseNode *lst_clauses;      // Linked-list of clauses
    int length;                   // Number of clauses in the list
    int variables;                // Number of unique variables
    int *sat_clauses;             // Satisfiable clauses
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

/** Data structures for node actions */
typedef struct ActionNode {
	Atom step;                    // Step
	Clause *clause;               // Clause removed
	Literal literal;              // Literal
	struct ActionNode *prev;      // Previous node
} ActionNode;

/** Data structures for record actions */
typedef struct Action {
	ActionNode *first;            // First node
	int length;                   // Number of nodes
} Action;



#endif
