#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "util.h"
#include "dlist.h"
#include "array_1d.h"

// ====================== PUBLIC DATA TYPES ==========================

// Anonymous declarations of node and graph.
struct node {
	bool seen;
	char name[41];
	dlist *neighbours;

};

struct graph {
	array_1d *entries;
	int size;
};



// =================== NODE COMPARISON FUNCTION ======================

/**
* nodes_are_equal() - Check whether two nodes are equal.
* @n1: Pointer to node 1.
* @n2: Pointer to node 2.
*
* Returns: true if the nodes are considered equal, otherwise false.
*
*/
bool nodes_are_equal(const node *n1,const node *n2){
	return (n1 == n2);
}

// =================== GRAPH STRUCTURE INTERFACE ======================

/**
* graph_empty() - Create an empty graph.
* @max_nodes: The maximum number of nodes the graph can hold.
*
* Returns: A pointer to the new graph.
*/
graph *graph_empty(int max_nodes){

	// Allocate memory
	graph *g = calloc(1, sizeof(graph));
	g->size = max_nodes + 1;

	// Create the array to hold the table_entries.
	g-> entries = array_1d_create(0,g->size, NULL);

	return g;

}

/**
* graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
* @g: Graph to check.
*
* Returns: True if graph is empty, otherwise false.
*/
bool graph_is_empty(const graph *g){

	//If the array is empty then we return true
	return !array_1d_has_value(g->entries, array_1d_low(g->entries));
}

/**
* graph_has_edges() - Check if a graph has any edges.
* @g: Graph to check.
*
* Returns: True if graph has any edges, otherwise false.
*/
bool graph_has_edges(const graph *g){

	//If the array is empty then we return true
	return !array_1d_has_value(g->entries, array_1d_low(g->entries));
}

/**
* graph_insert_node() - Inserts a node with the given name into the graph.
* @g: Graph to manipulate.
* @s: Node name.
*
* Creates a new node with a copy of the given name and puts it into
* the graph.
*
* Returns: The modified graph.
*/
graph *graph_insert_node(graph *g, const char* s){

	// insert i arrayen, nodes.
	int pos = array_1d_low(g->entries);
	struct node *n = malloc(sizeof(struct node));
	strcpy(n->name, s);
	n->neighbours = dlist_empty(NULL);
	n->seen = false;

	while(array_1d_has_value(g->entries, pos)){
		pos++;
	}

	array_1d_set_value(g->entries, n, pos);

	return g;
}


/**
* graph_find_node() - Find a node stored in the graph.
* @g: Graph to manipulate.
* @s: Node identifier, e.g. a char *.
*
* Returns: A pointer to the found node, or NULL.
*/
node *graph_find_node(const graph *g, const char *s){

	int i = array_1d_low(g->entries);
	char name[41];
	node *n;
	strcpy(name, s);
	while(array_1d_has_value(g->entries, i)) {
		//node *temp = array_1d_inspect_value(g->entries, i);
		n = array_1d_inspect_value(g->entries ,i);
		if ( strcmp(n->name, name) == 0){
			return n;
		}
		i++;
	}
	return NULL;
}

//ska leta upp listan som börjar på "char s"

/**
* graph_node_is_seen() - Return the seen status for a node.
* @g: Graph storing the node.
* @n: Node in the graph to return seen status for.
*
* Returns: The seen status for the node.
*/
bool graph_node_is_seen(const graph *g, const node *n){

	int pos = array_1d_low(g->entries);
	node *current_node = NULL;
	while(array_1d_has_value(g->entries, pos)){
		current_node = array_1d_inspect_value(g->entries, pos);
		if(current_node->name == n->name){
			return current_node->seen;
		}
		pos++;
	}

	return false;
}

/**
* graph_node_set_seen() - Set the seen status for a node.
* @g: Graph storing the node.
* @n: Node in the graph to set seen status for.
* @s: Status to set.
*
* Returns: The modified graph.
*/
graph *graph_node_set_seen(graph *g, node *n, bool seen) {

	int pos = array_1d_low(g->entries);
	node *current_node = NULL;
	while(array_1d_has_value(g->entries, pos)){
		current_node = array_1d_inspect_value(g->entries, pos);
		if(current_node->name == n->name){
			current_node->seen = seen;
			return g;
		}
		pos++;
	}

	return g;
}


/**
* graph_reset_seen() - Reset the seen status on all nodes in the graph.
* @g: Graph to modify.
*
* Returns: The modified graph.
*/
graph *graph_reset_seen(graph *g){

	int pos = array_1d_low(g->entries);
	node *current_node = NULL;
	while(array_1d_has_value(g->entries, pos)){
		current_node = array_1d_inspect_value(g->entries, pos);
			current_node->seen = false;
			return g;

		pos++;
	}


	return g;
}

/**
* graph_insert_edge() - Insert an edge into the graph.
* @g: Graph to manipulate.
* @n1: Source node (pointer) for the edge.
* @n2: Destination node (pointer) for the edge.
*
* NOTE: Undefined unless both nodes are already in the graph.
*
* Returns: The modified graph.
*/
graph *graph_insert_edge(graph *g, node *n1, node *n2){

	node *node_cmp = NULL;
	bool first_found = false;
	bool second_found = false;
	int pos = array_1d_low(g->entries);
	while(array_1d_has_value(g->entries, pos)){

		node_cmp = array_1d_inspect_value(g->entries ,pos);
		if(strcmp(node_cmp->name, n1->name) == 0){
			first_found = true;
		}
		if(strcmp(node_cmp->name, n2->name) == 0){
			second_found = true;
		}
		if(first_found == true && second_found == true){
			dlist_insert(n1->neighbours, n2->name, dlist_first(n1->neighbours));
		}

		pos++;
	}
	return g;
}


/**
* graph_choose_node() - Return an arbitrary node from the graph.
* @g: Graph to inspect.
*
* Returns: A pointer to an arbitrayry node.
*
* NOTE: The return value is undefined for an empty graph.
*/
node *graph_choose_node(const graph *g){

	node *arbt_node = NULL;
	arbt_node = array_1d_inspect_value(g->entries, array_1d_low(g->entries));
	return arbt_node;
}



/**
* graph_neighbours() - Return a list of neighbour nodes.
* @g: Graph to inspect.
* @n: Node to get neighbours for.
*
* Returns: A pointer to a list of nodes. Note: The list must be
* dlist_kill()-ed after use.
*/
dlist *graph_neighbours(const graph *g,const node *n){

	int pos = array_1d_low(g->entries);
	node *current_node = NULL;
	while(array_1d_has_value(g->entries, pos)){
		current_node = array_1d_inspect_value(g->entries, pos);
		if(current_node->name == n->name){
			return current_node->neighbours;
		}
		pos++;
	}
	return NULL;
}


/**
* graph_kill() - Destroy a given graph.
* @g: Graph to destroy.
*
* Return all dynamic memory used by the graph.
*
* Returns: Nothing.
*/
void graph_kill(graph *g){

	int pos = array_1d_low(g->entries);
	node *current_node = NULL;
	while(array_1d_has_value(g->entries, pos)){
		current_node = array_1d_inspect_value(g->entries, pos);
		dlist_kill(current_node->neighbours);
		free(current_node);
		pos++;
	}
	array_1d_kill(g->entries);
	free(g);
}
