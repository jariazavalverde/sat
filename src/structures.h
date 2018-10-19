/*H*
 * 
 * FILENAME: structures.h
 * DESCRIPTION: Data structures for SAT problem in CNF
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 19.10.2018
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

/** Data structure for literals */
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
    int original_size;            // Original size
    int alloc_size;               // Number of clauses allocated
    int variables;                // Number of unique variables
    int *sat_clauses;             // Satisfiable clauses
    ClauseNode **arr_unitaries;   // Array of one-literal clauses
    ClauseNode *lst_unitaries;    // Linked-list of one-literal clauses
    ClauseNode **occurrences;     // Clauses where occurs each variable
    Bool *interpretation;         // Array of values for interpretation
} Formula;

/** Data structures for trace execution */
typedef struct TraceNode {
	Clause *clause;               // Clause removed
	Atom atom;                    // Atom
	Literal literal;              // Literal
	struct TraceNode *next;       // Next node
	struct TraceNode *prev;       // Previous node
} TraceNode;

typedef struct Trace {
	TraceNode *lst_traces;        // First node
	TraceNode **decisions;        // Array of pointer to decision nodes
	int length;                   // Number of nodes
} Trace;

/** Data structures for implication graphs */
typedef struct GraphNode {
	Atom atom;                    // Atom of the node
	Bool value;                   // Value for the atom
	Decision decision;            // Kind of decision
	int level;                    // Decision level
	Clause *antecedents;          // Array of antecedent nodes of the node
	int degree;                   // Number of antecedents
} GraphNode;

typedef struct Graph {
	GraphNode **nodes;            // Nodes of the graph
	int size;                     // Maximum number of nodes
	int max_level;                // Maximum level assigned
} Graph;



#endif



/**
  * 
  * This function creates a formula of $nbvar variables and $nbclauses
  * clauses, returning a pointer to a newly initialized Formula struct.
  * 
  **/
Formula *formula_alloc(int nbvar, int nbclauses);
/**
  * 
  * This function creates a clause of $nbvar variables, returning a
  * pointer to a newly initialized Clause struct.
  * 
  **/
Clause *clause_alloc(int nbvar);
/**
  * 
  * This function creates a trace of $nbvar variables, returning a
  * pointer to a newly initialized Trace struct.
  * 
  **/
Trace *trace_alloc(int nbvar);
/** Initialize a new implication graph */
void init_graph(Graph *G, int size);
/** Add a new node into a graph */
int add_graph_node(Graph *G, Atom atom, Bool value, int level, Decision decision, Clause *clause);
