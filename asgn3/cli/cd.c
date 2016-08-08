/* cd.c

		source file for cd

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "cd.h"

/* change_dir
 * cd to path. Returns 0 if chdir() was successfully
 * returned, -1 otherwise.
 */
int change_dir(char *path) {
	int success = chdir(path);	
	if(success == 0) {

		if((check_s_dot(path) == 0) || check_d_dot(path) == 0) {
			char path[MAX_PATH_SIZE];
			strcpy(path, getcwd(path, MAX_PATH_SIZE));
			printf("cwd changed to %s\n", path);
		} else {
			printf("cwd changed to %s\n", path);
		}
	} else {
		printf("no such directory: %s\n", path);
	}
	return success;
}

/* check_cd
 * if "cd" is entered by user, return 0.
 * Otherwise, return 1.
 */
int check_cd(char *token) {
	int check = strcmp(token, "cd");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}

/* cd_home
 * cd to home.
 */
void cd_home() {
	change_dir(getenv("HOME"));
}

/* check_tilda
 * if "~" is entered by user, return 0.
 * Otherwise, return 1.
 */
int check_tilda(char *token) {
	int check = strcmp(token, "~");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}

/* corr_tilda_path
 * if the path starts with "~/..." return 0.
 * Otherwise, return 1.
 */
int corr_tilda_path(char **tokens) {
	int check;
	if(tokens[1][0] == '~' && tokens[1][1] == '/') {
		check = 0;
	} else {
		check = 1;
	}
	return check;
}

/* tilda_parse
 * if the path starts with "~/...", return the path
 * after the "~/".
 */
char *tilda_parse(char *path) {
	char *parsed_path = malloc(sizeof(char)*MAX_PATH_SIZE);
	int i = 0;
	int j;
	int p_len = strlen(path);

	for(j=2; path[j]; j++) {
		parsed_path[i] = path[j];
		i++;
	}

	parsed_path[p_len-2] = '\0';
	return parsed_path;
}

/* cd_tilda
 * cd to the path that begins with "~/...".
 */
void cd_tilda(char *token) {
	int token_len = strlen(token);
	char home[MAX_PATH_SIZE];
	char path[MAX_PATH_SIZE];

	if(token_len == 2) {
		if(token[0] == '~' && token[1] == '/') {
			cd_home();
		}
	} else if(token_len > 2) {
		strcpy(home, getenv("HOME"));
		strcat(home, "/");
		strcpy(path, tilda_parse(token));
		strcat(home, path);
		home[MAX_PATH_SIZE] = '\0';
		change_dir(home);	
	}
}

/* check_s_dot
 * if "cd ." is entered by user, return 0.
 * Otherwise return 1.
 */
int check_s_dot(char *token) {
	int check = strcmp(token, ".");
	if(check == 0) {
		return 0;
	}	else {
		return 1;
	}
}

/* s_dot
 * do command "cd ."
 */
void s_dot() {
	change_dir(".");
}

/* check_d_dot
 * if "cd .." is entered by user, return 0.
 * Otherwise return 1.
 */
int check_d_dot(char *token) {
	int check = strcmp(token, "..");
	if(check == 0) {
		return 0;
	} else {
		return 1;
	}
}

/* d_dot
 * do command "cd .."
 */
void d_dot() {
	change_dir("..");
}

/* cd_rel 
 * first checks if dir is an existing directory
 * in the cwd. If it is, append the directory to
 * the cwd and change directories. Otherwise,
 * gives an error message.
 */
void cd_rel(char *dir) {
	// store cwd in path
	char path[MAX_PATH_SIZE];
	strcpy(path, getcwd(path, MAX_PATH_SIZE));

	// check if dir is an existing relative dir.
	// if it is, append directory to pathname
	// and change its directory
	struct stat rel_path;
	if((stat(dir, &rel_path) == 0) && S_ISDIR(rel_path.st_mode)) {
		strcat(path, "/");
		strcat(path, dir);
		change_dir(path);	
	}	else {
		printf("no such directory: %s\n", dir);
	}
}
