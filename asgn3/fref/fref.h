/* fref.h

		header file for fref

		Dillon Vicente, August 2016
*/

#ifndef FREF_DEF
#define FREF_DEF 1

#define MAX_REGEX_SIZE 200
#define MAX_FILELINE_SIZE 200
#define MAX_FILES_SIZE 20
#define MAX_LINE_SIZE 1024

#include "regexp.h"

// opens an existing file to read from
FILE *open_file(char *name);

// chop and replace the newline character from
// user input gathered with fgets
char *chop_newline(char *string);

// if reg_ex includes quotes, return 1. 
// If it does not, return 0.
// regex must be in format "regex"\0
int check_regex(char *regex);

// parse the quotes of a regular expression.
char *parse_quotes(char *regex);

#endif
