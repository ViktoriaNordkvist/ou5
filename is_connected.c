#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace(), etc.
#include <errno.h>

#include "graph.h"
#include "array_1d.h"
#include "dlist.h"
#include "queue.h"
#include "list.h"

/*
 * Name: is_connected.c
 *
 * Description: This program is used to determine wether
 * or not two airports are connected throught a flightpath
 * based on information through an inserted file.
 *
 * Authors: Viktoria Nordkvist (id19vnt@cs.umu.se)
 *	    	Tobias Bergström (id19tbm@cs.umu.se)
 *
 * Version information:
 *   2018-03-18: v1.0, first public version.
 */


 /* Return position of first non-whitespace character or -1 if only
 20 white-space is found. */
 int first_non_white_space(const char *s){
 	int i = 0; // Start at first char.
 	// Advance until we hit EOL as long as we're loooking at white-space.
 	while (s[i] && isspace(s[i])) {
 		i++;
 	}
 	if (s[i]) {
 		return i; // Return position of found a non-white-space char.
 	} else {
 		return -1; // Return fail.
 	}
 }

 /* Return position of last non-whitespace character or -1 if only
 white-space is found. */
 int last_non_white_space(const char *s){
 	// Start at last char.
 	int i = strlen(s) - 1;
 	// Move back until we hit beginning-of-line as long as we're
 	// looking at white-space.
 	while (i >= 0 && isspace(s[i])) {
 		i--;
 	}
 	if (i >= 0) {
 		return i; // Return position of found a non-white-space char.
 	} else {
 		return -1; // Return fail.
 	}
 }

 /* Return true if s only contains whitespace */
 bool line_is_blank(const char *s){
 	// Line is blank if it only contained white-space chars.
 	return first_non_white_space(s) < 0;
 }

 /* Return true if s is a comment line, i.e. first non-whitespc char is '#' */
 bool line_is_comment(const char *s){
 	int i = first_non_white_space(s);
 	return (i >= 0 && s[i] == '#');
 }

 //Returns true if the current line is an integer, false if not.
 bool line_is_digit(const char *s){

 	int temp = 0;
 	temp = atoi(s);
 	if(temp > 0){
 		return true;
 	}
 	return false;
 }

/**
* find_path() - Check if there is a path.
* @g: Graph to check.
* @n: Origin Node.
* @n: Destination Node.
*
* Returns: True if graph has any edges, otherwise false.
*/
bool find_path(graph *g,node *src,node *dest){
	//fprintf(stderr, "%s\n","Hewwo!" );

	//Variable Declarations
	node *node_temp;
	dlist_pos pos = 0;
	char *temp_name = NULL;
	dlist *list_temp = graph_neighbours(g, src);

	//If both inputed nodes are the same, return true.
	if(nodes_are_equal(src, dest)){
		//fprintf(stderr, "%s\n","Tjo bre mannen asså walla orkar inte mann." );
		return true;
	}

	//Remove the start node, go through the children lists and create a queue.
	// ta bort start noden. gå igenom barnens listor. bygg kö.
	else{
		//fprintf(stderr, "%s\n","ELSE" );
		queue *q = queue_empty(NULL);
		q = queue_enqueue(q, src);
		pos = dlist_first(list_temp);

		//Queues the origin nodes neighbouring nodes until we reach the end of the list.
		while(!dlist_is_end(list_temp, pos)){
			//fprintf(stderr, "%s\n","WHILE 1" );
			temp_name = dlist_inspect(list_temp, pos);
			node_temp = graph_find_node(g, temp_name);
			queue_enqueue(q, node_temp);

			pos = dlist_next(list_temp, pos);
		}

		graph_node_set_seen(g, src, true);
		queue_dequeue(q);

		//Loops through the queue and adds each nodes neighbours to the queue if not seen.
		while (!queue_is_empty(q)) {
			//fprintf(stderr, "%s\n","WHILE 2" );
			node_temp = queue_front(q);
			list_temp = graph_neighbours(g, node_temp);
			pos = dlist_first(list_temp);

			//Check if the current node is the destination node.
			if(nodes_are_equal(node_temp, dest)){
				//fprintf(stderr, "%s\n","Tjo bre mannen asså walla orkar inte mann." );
				queue_kill(q);
				return true;
			}

			//Loops through the node list and adds unseen neighbours to the queue.
			while(!dlist_is_end(list_temp, pos)){

				temp_name = dlist_inspect(list_temp, pos);
				node_temp = graph_find_node(g, temp_name);

				if(!graph_node_is_seen(g, node_temp)){
					queue_enqueue(q, node_temp);
				}

				pos = dlist_next(list_temp, pos);
				graph_node_set_seen(g, node_temp, true);
			}
			queue_dequeue(q);
		}

		queue_kill(q);
	}
	//fprintf(stderr, "%s\n","Nein das ist nicht gut." );
	return false;
}


/**
* read_file() - Check if there is a path.
* @g: Graph to modify.
* @n: File to be read.
*
* Returns: NULL.
*/
graph *read_file(graph *g, const char *name){ // const char name är namnet på filen

	const int BUFSIZE = 41;
	int maxnodes = 0;
	//maxnodes = maxnodes; // Något är tokigt, maxnodes används men utan detta kompilerar det inte.
	char line[BUFSIZE];
	char src[BUFSIZE];
	char dest[BUFSIZE];
	FILE *in; // Pointer to input file
	in = fopen(name, "r");
	bool first_non_comment = true;
	dlist *temp_list;
	dlist_pos pos;

	//Check if the file can be found.
	if(in == NULL){
		fprintf(stderr, "%s\n", "Error: File does not exist. Program terminated.");
		exit(EXIT_FAILURE);
	}

	// Read the file
	while (fgets(line, BUFSIZE, in) != NULL){

		//Skip current line if it is a comment or blank
		if(line_is_comment(line) || line_is_blank(line)){
			//fprintf(stderr, "%s\n", "inne i if(line_is_comment...)");
			continue;
		}

		//Check if the first the first non-comment line to see if it is an integer.
		if(first_non_comment){
			//fprintf(stderr, "%s\n", "inne i if(first_non_comment)");
			if (line_is_digit(line)){
				maxnodes = atoi(line);
				//fprintf(stderr, "%s\n", "Här allokeras minne :)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				g = graph_empty(maxnodes*2);
				first_non_comment = false;
				continue;
			}
			else{
				fprintf(stderr, "%s\n","Error: First non-comment line is not an integer. Program terminated.");
				exit(EXIT_FAILURE);
			}
		}
		//fprintf(stderr, "%s\n", "Time to scan" );
		//Scan the first two words(nodes) on the line and save them in src and dest accordingly.
		if(sscanf(line, "%s %s ", src, dest) != 2) {
			fprintf(stderr, "%s\n", "Error: The file does not have the right format\n");
			exit(EXIT_FAILURE);
		}

		//Exit with error if the inserted nodes are the same.
		if(strcmp(src, dest) == 0){
			exit(EXIT_FAILURE);
		}
		//fprintf(stderr, "%s\n", "About to print");
		//fprintf(stderr, "%s %s\n", src, dest);
		if(graph_find_node(g, src) == NULL){
			g = graph_insert_node(g, src);
		}
		if(graph_find_node(g, dest) == NULL){
			g = graph_insert_node(g, dest);
		}

		temp_list = graph_neighbours(g, graph_find_node(g, src));
		pos = dlist_first(temp_list);
		while(!dlist_is_end(temp_list, pos)){
			if(strcmp(dlist_inspect(temp_list, pos), dest)){
				fprintf(stderr, "%s\n", "Error: File can't contain more than one of the same edge");
				exit(EXIT_FAILURE);
			}
			pos = dlist_next(temp_list, pos);
		}
		g = graph_insert_edge(g, graph_find_node(g, src), graph_find_node(g, dest));

		//puts(line);
		first_non_comment = false;
	}
	//fprintf(stderr, "%s\n", "Done");
	fclose(in);
	return g;
}

//Main function. duh.
int main(void){
	const int BUFSIZE = 41;
	char origin[BUFSIZE];
	char destination[BUFSIZE];
	bool running = true;
	char quit[5] = "quit";
	node *node1;
	node *node2;
	bool path_found;
	graph *g = NULL;
	int entry1;
	int entry2;
	int entries;

	g = read_file(g,"3-directed-graph.map");


	while(running){
		fprintf(stderr, "%s", "Enter origin and destination (quit to exit): " );

		entry1 = scanf("%40s", origin);

		if(strcmp(origin, quit) == 0){
			fprintf(stderr, "%s\n", "Normal exit.");
			running = false;
			graph_kill(g);
			return 0;
		}

		entry2 = scanf("%40s", destination);

		entries = entry1 + entry2;

		if(entries < 2){
			fprintf(stderr, "%s\n", "To few arguments." );
			continue;
		}
		if(entries > 2){
			fprintf(stderr, "%s\n", "To many arguments." );
			continue;
		}

		node1 = graph_find_node(g,origin);
		node2 = graph_find_node(g,destination);


		if(node1 == NULL && node2 == NULL){
			fprintf(stderr, "%s\n", "None of the nodes exists");
			continue;
		}
		if(node1 == NULL){
			fprintf(stderr, "%s\n", "Origin does not exist");
			fprintf(stderr, "%s", "\n" );
			continue;
		}
		if(node2 == NULL){
			fprintf(stderr, "%s\n", "Destination does not exist");
			fprintf(stderr, "%s", "\n" );
			continue;
		}

		path_found = find_path(g, node1, node2);

		if(path_found){
			fprintf(stderr, "There is a path from %s to %s.\n", origin, destination);
			fprintf(stderr, "%s", "\n");
		}
		else {
			fprintf(stderr, "There is no path from %s to %s.\n", origin, destination);
			fprintf(stderr, "%s", "\n" );

		}

	}

	graph_kill(g);
	return 0;
	}
