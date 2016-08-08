/* fref.c

		source file for fref
		searches files for any regular expression and ouputs results to stdout.

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "fref.h"

int main(int argc, char *argv[]) {

	char regex[MAX_REGEX_SIZE];

	// gets the file line and tokenizes it
	char file_line[MAX_FILELINE_SIZE];
	char **files;

	// file pointing to the current file being searched
	// line number starts at 1
	FILE *fp;
	int line_num = 1;

	// every line in a file
	// every file i
	char line[MAX_LINE_SIZE];
	int i;

	// matching regex to a line in a file check
	int match_regex;

	// check if regex has quotes
	int regex_check;


	if(argc < 3) {
		// get regex
		printf("Enter your regular expression: \n");
		fgets(regex, MAX_REGEX_SIZE, stdin);

		// chop newline from regex
		strcpy(regex, chop_newline(regex));

		// check if regex has quotes, if so, parse them
		regex_check = check_regex(regex);
		if(regex_check == 1) {
			strcpy(regex, parse_quotes(regex));
		}

		// get files
		printf("Enter your files: \n");
		fgets(file_line, MAX_FILELINE_SIZE, stdin);
		files = tokenize(file_line, " ");

		// for every line in the program, display
		// where the matching substrings are
		for(i=0; files[i]; i++) {
			fp = open_file(files[i]);
			printf("%s\n", files[i]);
			if(fp == 0) {
				printf("Exiting program.\n");
				return 1;
			}
			while(fgets(line, MAX_LINE_SIZE, fp)) {
				match_regex = match(regex, line);
				if(match_regex == 1) {
					printf("%s:%d %s", files[i], line_num, line);
				}	
				line_num++;
			}
			printf("\n");
		}
		
	} else if(argc >= 3) {
		
		// get regex from first arg
		strcpy(regex, argv[1]);
		strcpy(regex, chop_newline(regex));

		// check if regex has quotes
		regex_check = check_regex(regex);
		if(regex_check == 1) {
			strcpy(regex, parse_quotes(regex));
		}

		// get files, starting at the right argument
		int k;
		int m = 0;
		files = malloc(sizeof(files)*MAX_FILES_SIZE);
		for(k=2; argv[k]; k++) {	
			files[m] = argv[k];
			m++;
		}

		// for every line in the program, display
		// where the matching substrings are
		for(i=0; files[i]; i++) {
			fp = open_file(files[i]);
			printf("%s\n", files[i]);
			if(fp == 0) {
				printf("Exiting program.\n");
				return 1;
			}
			while(fgets(line, MAX_LINE_SIZE, fp)) {
				match_regex = match(regex, line);
				if(match_regex == 1) {
						printf("%s:%d %s", files[i], line_num, line);
				}
				line_num++;
			}
			printf("\n");
		}
	} else {
		printf("Invalid number of arguments.\n");
	}

	return 0;
}

// opens an existing file to read from
FILE *open_file(char *name) {
	FILE *fp = fopen(name, "r");
	if(fp == 0) {
		printf("File %s not found.\n", name);
	}
	return fp;
}

// chop and replace the newline character from
// user input gathered with fgets
char *chop_newline(char *string) {
	int str_len = strlen(string);
	if(string[str_len-1] == '\n') {
		string[str_len-1] = '\0';
	}
	return string;
}

// if reg_ex includes quotes, return 1.
// If it does not, return 0.
// reg_ex must be in format: "regex"\0
int check_regex(char *regex) {
	int regex_len = strlen(regex);

	// regex_len-1 is the " before the \0
	if(regex[0] == '"' && regex[regex_len-1] == '"') {
		return 1;
	}	else {
		return 0;
	}
}

// parse the quotes of a regular expression.
char *parse_quotes(char *regex) {
	int str_len = strlen(regex);
	char *new_regex = malloc(sizeof(char)*str_len);
	int i;
	int j;
	for(i=0; regex[i]; i++) {
		if(regex[i] != '"') {
			new_regex[j] = regex[i];
			j++;
		}	
	}
	return new_regex;
}
