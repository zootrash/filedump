/* misc.c

		source file for misc

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <string.h>

#include "misc.h"

// if "fref" is entered by user, return 0.
// Otherwise return 1.
int check_fref(char *token) {
	int check = strcmp(token, "fref");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}

// if "listf" is entered by user, return 0.
// Otherwise, return 1.
int check_listf(char *token) {
	int check = strcmp(token, "listf");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}

// if "calc" is entered by user, return 0.
// Otherwise, return 1.
int check_calc(char *token) {
	int check = strcmp(token, "calc");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}
