/* misc.h

		header file for misc

		Dillon Vicnete, August 2016
*/

#ifndef MISC_DEF
#define MISC_DEF 1

#include "redir.h"

// if "fref" is entered by user, return 0.
// Otherwise, return 1.
int check_fref(char *token);

// if "listf" is entered by user, return 0.
// Otherwise, return 1.
int check_listf(char *token);

// if "calc" is entered by user, return 0.
// Otherwise, return 1.
int check_calc(char *token);

#endif
