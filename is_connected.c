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


bool find_path(graph *g,node *src,node *dest){
	node *node_temp;
	dlist_pos *pos = 0;
	char *temp_name = NULL;
	dlist list_temp = src->neighbours;

		if(nodes_are_equal(src, dest)){
			return true;
		}
		// ta bort start noden. gå igenom barnens listor. bygg kö.
		else{
			queue *q = queue_empty(NULL);
			q = queue_enqueue(q, src);
			pos = dlist_first(list_temp);

			while(!dlist_is_end(list_temp, pos)){
				temp_name = dlist_inspect(list_temp, pos);
				node_temp = graph_find_node(g, temp_name);
				queue_enqueue(q, node_temp);

				pos = dlist_next(list_temp, pos);
			}

			graph_node_set_seen(g, src, true);
			queue_dequeue(q);

			while (!queue_is_empty(q)) {
				node_temp = queue_front(q);
				list_temp = node_temp->neighbours;
				pos = dlist_first(list_temp);
				if(node_temp->name == dest->name){
					return true;
				}

				while(!dlist_is_end(list_temp), pos){

					temp_name = dlist_inspect(list_temp, pos);
					node_temp = graph_find_node(g, temp_name);

					if(!graph_node_is_seen(g, node_temp)){
						queue_enqueue(q, node_temp);
					}

					pos = dlist_next(list_temp, pos);
					graph_node_set_seen(g, node_temp, true);
					queue_dequeue(q);
				}
			}

			/*node_temp = graph_find_node(g,src->name);
			q = queue_enqueue(q, node_temp);
			while(!queue_is_empty(q)){
				dlist *list_temp = graph_neighbours(g, node_temp);
				int pos = dlist_first(list_temp);
				while(!dlist_is_end(list_temp, pos)){
					char name_temp = dlist_inspect(list_temp, pos);
					if(name_temp == dest){
						return true;
					}
					else{
						if(graph_node_is_seen(g, node_temp)){
							continue;
						}
						q = queue_enqueue(q, graph_find_node(g, name_temp));
						pos = dlist_next(list_temp);
					}

				}
					graph_node_set_seen(g, node_temp, true);
					queue_dequeue(q);
					node_temp = queue_front(q);
				}*/


			//}
		}
}


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
int last_non_white_space(const char *s)
{
	// Start at last char.
	int i = strlen(s) - 1;
	// Move back until we hit beginning-of-line as long as we're
	// loooking at white-space.
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
bool line_is_blank(const char *s) {
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

void read_file(graph *g, const char *name){ // const char name är namnet på filen

	const int BUFSIZE = 41;
	int maxnodes = 0;
	char line[BUFSIZE];
	char src[BUFSIZE];
	char dest[BUFSIZE];
	FILE *in; // Pointer to input file
	in = fopen(name, 'r');
	bool first_non_comment = true;


	if(in == NULL){
		printf("Couldn't open file, program terminated.");
		exit(EXIT_FAILURE);
	}
	while (fgets(line, BUFSIZE, in) != NULL){
		if(line_is_comment(line) || line_is_blank(line)){
			continue;
		}

		if(first_non_comment){
			if (line_is_digit(line)){
				maxnodes = atoi(line);
				continue;
			}
			else{
				exit(EXIT_FAILURE);
			}
		}
		sscanf(line, "%s %s ", src, dest); //lägg till och ta in en till och se satt det är en kommentar

		if(strcmp(src, dest) == 0){
			exit(EXIT_FAILURE);
		}
		else if(strcmp(dest, NULL) == 0){
			exit(EXIT_FAILURE);
		}

		graph_insert_node(g, src);
		graph_insert_edge(g, src, dest);
		//puts(line);
		first_non_comment = false;
	}

	fclose(in);
}


int main(void){

	graph *g = NULL;
	g = graph_empty(10);
	read_file(g, "airmap1.map");
	return 0;
}
