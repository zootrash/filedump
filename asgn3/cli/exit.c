/* exit.c

		source file for exit

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <string.h>

#include "exit.h"

/* exit_cli
 * if "exit" is entered by user, return 0.
 * Otherwise, return 1.
 */
int exit_cli(char *token) {
	int check = strcmp(token, "exit");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}
