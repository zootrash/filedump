/* redir.c

		source file for redir

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redir.h"

// check user input for redirecting input
// Return 0 if "<" is detected, 1 otherwise.
int check_input(char *line) {
	int i;
	for(i=0; line[i]; i++) {
		if(line[i] == '<') {
			return 0;
		}
	}
	return 1;
}

// check user input for redirecting input
// Return 0 if ">" is detected, 1 otherwise.
int check_output(char *line) {
	int i;
	for(i=0; line[i]; i++) {
		if(line[i] == '>') {
			return 0;
		}
	}
	return 1;
}
