/* pwd.h

		header file for pwd

		Dillon Vicente, August 2016
*/

#ifndef PWD_DEF
#define PWD_DEF 1

#include "cd.h"

/* get_pwd
 * prints the cwd.
 */
void get_pwd();

/* check_pwd
 * if "pwd" is entered by user, return 0.
 * Otherwise, return 1.
 */
int check_pwd(char *token);

#endif
