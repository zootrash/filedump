/* calc.h

	header file for calc

	Dillon Vicente, August 2016
*/

#ifndef CALC_DEF
#define CALC_DEF 1

#define MAX_LINE 1024

#include "tokenizer.h"

// addition
int addi(char *num1, char *num2);

// subtraction
int subt(char *num1, char *num2);

// multiplication
int mult(char *num1, char *num2);

// division
int divi(char *num1, char *num2);

// check if '+' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int a_exists(char *line);

// check if '-' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int s_exists(char *line);

// check if '*' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int m_exists(char *line);

// check if '/' exists within the input line.
// Returns 1 if it exists, 0 otherwise.
int d_exists(char *line);

#endif
