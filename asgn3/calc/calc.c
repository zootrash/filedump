/* calc.c

	source file for calc
	simple calculator for the command line interpreter.

	Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

int main() {
	
	char line[MAX_LINE];
	char **tokens;
	int num_tokens;
	int result;
	
	while(fgets(line, MAX_LINE, stdin)) {
		tokens = tokenize(line, " +-*/");
		num_tokens = token_count(tokens);

		// checks if there were too many numbers
		if(num_tokens > 2) {
			printf("Too many arguments.\n");
		} else {

			// flags for which operation was detected
			int a_flag = a_exists(line);
			int s_flag = s_exists(line);
			int m_flag = m_exists(line);
			int d_flag = d_exists(line);
			int p_flag = 0;
			char o_flag;

			// perform arithmetic on arguments
			if(a_flag == 1 && s_flag == 0 && m_flag == 0 && d_flag == 0) {
				result = addi(tokens[0], tokens[1]);	
				o_flag = '+';
			} else if(s_flag == 1 && a_flag == 0 && m_flag == 0 && d_flag == 0) {
				result = subt(tokens[0], tokens[1]);
				o_flag = '-';
			} else if(m_flag == 1 && a_flag == 0 && s_flag == 0 && d_flag == 0) {
				result = mult(tokens[0], tokens[1]);
				o_flag = '*';
			} else if(d_flag == 1 && a_flag == 0 && s_flag == 0 && m_flag == 0) {
				// check for division by 0
				if(strcmp(tokens[1], "0") == 0) {
					printf("Cannot divide by 0.\n");
					p_flag = 1;
				} else {
					result = divi(tokens[0], tokens[1]);
					o_flag = '/';
				}
			} else {
				printf("Too many operators.\n");
				p_flag = 1;
			}
			
			// print result only if there were a correct
			// number of operators
			if(p_flag == 0) {	
				printf("%s %c %s = %d\n", tokens[0], o_flag, tokens[1], result);
			}
		}
	}
}

// addition
int addi(char *num1, char *num2) {
	int val1 = atoi(num1);
	int val2 = atoi(num2);
	return val1 + val2;
}

// subtraction
int subt(char *num1, char *num2) {
	int val1 = atoi(num1);
	int val2 = atoi(num2);
	return val1 - val2;
}

// multiplication
int mult(char *num1, char *num2) {
	int val1 = atoi(num1);
	int val2 = atoi(num2);
	return val1 * val2;
}

// division
int divi(char *num1, char *num2) {
	int val1 = atoi(num1);
	int val2 = atoi(num2);
	return val1 / val2;
}

// check if '+' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int a_exists(char *line) {
	int i;
	for(i=0; line[i]; i++) {
		if(line[i] == '+') {
			return 1;
		}
	}
	return 0;
}

// check if '-' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int s_exists(char *line) {
	int i;
	for(i=0; line[i]; i++) {
		if(line[i] == '-') {
			return 1;
		}
	}
	return 0;
}

// check if '*' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int m_exists(char *line) {
	int i;
	for(i=0; line[i]; i++) {
		if(line[i] == '*') {
			return 1;
		}
	}
	return 0;
}

// check if '/' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int d_exists(char *line) {
	int i;
	for(i=0; line[i]; i++) {
		if(line[i] == '/') {
			return 1;
		}
	}
	return 0;
}
