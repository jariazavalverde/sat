/*H*
 * 
 * FILENAME: 2sat.h
 * DESCRIPTION: 2-satisfiability algorithm
 * AUTHORS: José Antonio Riaza Valverde
 * UPDATED: 25.11.2018
 * 
 *H*/

#include "2sat.h"



/**
  * 
  * This function creates an implication graph of $nbvar variables,
  * returning a pointer to a newly initialized SAT2_Graph struct.
  * 
  **/
SAT2_Graph *sat2_graph_alloc(int nbvar) {
	int i;
	int nodes = nbvar * 2;
	SAT2_Graph *G = malloc(sizeof(SAT2_Graph));
	G->adjacents = malloc(nodes * sizeof(SAT2_Node*));
	G->nbvar = nbvar;
	G->nb_nodes = nodes;
	for(i = 0; i < nodes; i++)
		G->adjacents[i] = NULL;
	return G;
}

/**
  * 
  * This function frees a previously allocated graph $G.
  * The graph nodes underlying the trace will also be deallocated.
  * Clause nodes underlying the graph nodes will not be deallocated.
  * 
  **/
void sat2_graph_free(SAT2_Graph *G) {
	int i;
	SAT2_Node *node, *next;
	for(i = 0; i < G->nb_nodes; i++) {
		node = G->adjacents[i];
		while(node != NULL) {
			next = node->next;
			free(node);
			node = next;
		}
	}
	free(G->adjacents);
	free(G);
}

/**
  * 
  * This function adds a new adjacent ($atom_to, $lit_to) to the
  * element ($atom_from, $lit_from) of the graph $G.
  *  
  **/
void sat2_graph_add_adjacent(SAT2_Graph *G, Atom atom_from, Literal lit_from, Atom atom_to, Literal lit_to) {
	int index_from = lit_from == NEGATIVE ? G->nbvar + atom_from : atom_from;
	int index_to = lit_to == NEGATIVE ? G->nbvar + atom_to : atom_to;
	SAT2_Node *node = malloc(sizeof(SAT2_Node));
	node->node = index_to;
	node->next = G->adjacents[index_from];
	G->adjacents[index_from] = node;
}

/**
  * 
  * This function checks the satisfiability of the formula $F,
  * following the Apswall algorithm for 2-SAT problem. If $F is
  * satisfiable, the function returns 1 and the interpretation for
  * $F is set in $F->interpretation. Otherwise, the function returns 0.
  * 
  **/
int sat2_check_sat(Formula *F) {
	// Construct graph
	SAT2_Graph *G = sat2_implicative_normal_form(F);
	// Get strongly connected components
	int *components = sat2_kosaraju(G);
	// Free structures
	free(components);
	sat2_graph_free(G);
	// Return result
	return 0;
}

/**
  * 
  * This function returns a pointer to the graph representing the 
  * formula $F in implicative normal form. For each variable x there
  * will be two vertices v_x and v_{\neg x}. The edges will correspond
  * to the implications.
  *  
  **/
SAT2_Graph *sat2_implicative_normal_form(Formula *F) {
	int i;
	Clause* clause;
	LiteralNode *fst, *snd;
	SAT2_Graph *G = sat2_graph_alloc(F->nbvar);
	for(i = 0; i < F->nbclauses; i++) {
		clause = F->arr_clauses[i]->clause;
		fst = clause->arr_literals[clause->literals[0]];
		snd = clause->arr_literals[clause->literals[1]];
		sat2_graph_add_adjacent(G, fst->atom, literal_not(fst->literal), snd->atom, snd->literal);
		sat2_graph_add_adjacent(G, snd->atom, literal_not(snd->literal), fst->atom, fst->literal);
	}
	return G;
}

/**
  * 
  * This function returns a pointer to the array of components of the
  * graph $G, following the Kosaraju's algorithm to find the strongly
  * connected components.
  *  
  **/
int *sat2_kosaraju(SAT2_Graph *G) {
	int i, *visited, *components;
	SAT2_Node *list;
	visited = malloc(G->nb_nodes * sizeof(int));
	components = malloc(G->nb_nodes * sizeof(int));
	for(i = 0; i < G->nb_nodes; i++) {
		visited[i] = 0;
		components[i] = -1;
	}
	for(i = 0; i < G->nb_nodes; i++)
		list = sat2_kosaraju_visit(G, list, visited, i);
	while(list != NULL) {
		sat2_kosaraju_assign(G, components, list->node, list->node);
		list = list->next;
	}
	free(visited);
	return components;
}

/**
  * 
  * This function visits the adjacent nodes of the node $u of the graph
  * $G, and marks it as visited in the array $visited. This function
  * returns a pointer to the visited node, followed by the node $list.
  *  
  **/
SAT2_Node *sat2_kosaraju_visit(SAT2_Graph *G, SAT2_Node *list, int *visited, int u) {
	SAT2_Node *adjacent, *node;
	if(!visited[u]) {
		visited[u] = 1;
		adjacent = G->adjacents[u];
		while(adjacent != NULL) {
			list = sat2_kosaraju_visit(G, list, visited, adjacent->node);
			adjacent = adjacent->next;
		}
		node = malloc(sizeof(SAT2_Node));
		node->node = u;
		node->next = list;
		return node;
	}
	return list;
}

/**
  * 
  * This function assigns the strongly connected component to each node
  * of the graph $G in the array $components.
  *  
  **/
void sat2_kosaraju_assign(SAT2_Graph *G, int *components, int u, int root) {
	SAT2_Node *adjacent;
	if(components[u] == -1) {
		components[u] = root;
		while(adjacent != NULL) {
			sat2_kosaraju_assign(G, components, adjacent->node, root);
			adjacent = adjacent->next;
		}
	}
}
