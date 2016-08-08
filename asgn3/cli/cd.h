/* cd.h

		header file for cd

		Dillon Vicente, August 2016
*/

#ifndef CHANGEDIR_DEF
#define CHANGEDIR_DEF 1

#define MAX_PATH_SIZE 200

#include "exit.h"

/* change_dir
 * cd to path. Returns 0 if chdir() was successfully
 * returned, -1 otherwise.
 */
int change_dir(char *path);

/* check_cd
 * if "cd" is entered by user, return 0.
 * Otherwise, return 1.
 */
int check_cd(char *token);

/* cd_home
 * cd to home.
 */
void cd_home();

/* check_tilda
 * if "~" is entered by user, return 0.
 * Otherwise, return 1.
 */
int check_tilda(char *token);

/* corr_tilda_path
 * if the path starts with "~/..." return 0.
 * Otherwise, return 1.
 */
int corr_tilda_path(char **tokens);

/* tilda_parse
 * if the path starts with "~/...", return the path
 * after the "~/".
 */
char *tilda_parse(char *path);

/* cd_tilda
 * cd to the path that begins with "~/...".
 */
void cd_tilda(char *token);

/* check_s_dot
 * if "cd ." is entered by user, return 0.
 * Otherwise return 1.
 */
int check_s_dot(char *token);

/* s_dot
 * do command "cd ."
 */
void s_dot();

/* check_d_dot
 * if "cd .." is entered by user, return 0.
 * Otherwise return 1.
 */
int check_d_dot(char *token);

/* d_dot
 * do command "cd .."
 */
void d_dot();

/* cd_rel
 * first checks if dir is an existing directory
 * in the cwd. If it is, append the directory to 
 * the cwd and change directories. Otherwise,
 * gives an error message.
 */
void cd_rel(char *dir);

#endif
