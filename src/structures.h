/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Data structures for SAT problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * DATE: 12.10.2018
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

/** Possible kinds of nodes in implication graphs */
typedef enum {ARBITRARY, FORCED, CONFLICTIVE} Decision;

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
	int *literals;                // Array of identifiers of literals
	LiteralNode **arr_literals;   // Array of literals
    LiteralNode *lst_literals;    // Linked-list of literals
    int length;                   // Number of literals in the list
    int size;                     // Total number of literals
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
    int size;                     // Total number of clauses
    int variables;                // Number of unique variables
    int *sat_clauses;             // Satisfiable clauses
    Literal *attempts;            // Control for backtracking
    ClauseNode **arr_unitaries;   // Array of one-literal clauses
    ClauseNode *lst_unitaries;    // Linked-list of one-literal clauses
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
	struct ActionNode *next;      // Next node
	struct ActionNode *prev;      // Previous node
} ActionNode;

/** Data structures for record actions */
typedef struct Action {
	ActionNode *first;            // First node
	ActionNode **decisions;       // Array of pointer to decision nodes
	int length;                   // Number of nodes
} Action;

/** Data structure for nodes of implication graphs */
typedef struct GraphNode {
	Atom atom;                    // Atom of the node
	Bool value;                   // Value for the atom
	Decision decision;            // Kind of decision
	int level;                    // Decision level
	int *antecedents;             // Array of antecedent nodes of the node
	int degree;                   // Number of antecedents
} GraphNode;

/** Data structure for implication graphs */
typedef struct Graph {
	GraphNode **nodes;            // Nodes of the graph
	int size;                     // Maximum number of nodes
	int max_level;                // Maximum level assigned
} Graph;



#endif



/** Initialiaze a new interpretation */
void init_interpretation(Interpretation *I, int length);
/** Initializa a new action */
void init_action(Action *actions, int size);
/** Initialize a new implication graph */
void init_graph(Graph *G, int size);
/** Add a new node into a graph */
int add_graph_node(Graph *G, Atom atom, Bool value, int level, Decision decision, Clause *clause);
/** Set the value of a graph node */
int set_graph_node(Graph *G, Atom atom, Bool value);
