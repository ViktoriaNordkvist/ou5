#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace(), etc.
#include <errno.h>
#include <stdbool.h>

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

int main(void){

	//int printit = 0;
	const int BUFSIZE = 41;
	char line[BUFSIZE];
	char node1[BUFSIZE];
	char node2[BUFSIZE];
	FILE *in; // Pointer to input file
	//int row_number = 1;
	in = fopen("airmap1.map","r");

	if(in == NULL){
		printf("Couldn't open file, program terminated.");
		exit(EXIT_FAILURE);
	}

	while (fgets(line, BUFSIZE, in) != NULL){ //Testa med EOF och NULL - Kanske inte körs om filen börjar med en tom line
		if(line_is_comment(line) || line_is_blank(line)){
			continue;
		}

		// läs bara i den två första orden
		sscanf(line, "%s %s ", node1, node2);

		printf("%s %s \n", node1, node2 );
	}



	fclose(in);
}
