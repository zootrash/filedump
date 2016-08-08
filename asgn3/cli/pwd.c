/* pwd.c

		source file for pwd

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pwd.h"

/* get_pwd
 * prints the cwd.
 */
void get_pwd() {
	char pwd[MAX_PATH_SIZE];
	strcpy(pwd, getcwd(pwd, MAX_PATH_SIZE));
	printf("%s ", pwd);
}

/* check_pwd
 * if "pwd" is entered by user, return 0.
 * Otherwise, return 1.
 */
int check_pwd(char *token) {
	int check = strcmp(token, "pwd");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}
