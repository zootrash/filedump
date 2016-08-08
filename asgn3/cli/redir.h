/* redir.h

		header file for redir

		Dillon Vicente, August 2016
*/

#ifndef REDIR_DEF
#define REDIR_DEF 1

#define MAX_TOKENSIZE 100

#include "pwd.h"

// check user input for redirecting input
// Return 0 if "<" is detected, 1 otherwise.
int check_input(char *line);

// check user input for redirecting output
// Return 0 if  ">" is detected, 0 otherwise.
int check_output(char *line);

#endif
