/* listf.h

	header file for listf

	Dillon Vicente, August 2016
*/

#ifndef LISTF_DEF
#define LISTF_DEF 1

#define MAX_NUMFILES 50
#define MAX_FILENAME_SIZE 100
#define MAX_OPTIONS_SIZE 50
#define MAX_PATH_SIZE 200

#define TWO_DIGITS 10
#define THR_DIGITS 100
#define FOU_DIGITS 1000
#define FIV_DIGITS 10000
#define SIX_DIGITS 100000
#define SEV_DIGITS 1000000
#define EIG_DIGITS 10000000
#define NIN_DIGITS 100000000
#define TEN_DIGITS 1000000000

#define TIME_LENGTH 2
#define EPOCH_YEAR 1900
#define MILLENIUM 2000

#include "tokenizer.h"

// sort files in alphabetical order
char **sort_files(char **files);

// if "listf" is entered by the user, display
// all of the files in the current directory.
void display_allfiles(char **files, int f);

// display all files each on a new line
void display_nfiles(char **files);

// check if argument begins with a "-"
// if option begins with a "-", return 1.
// Otherwise return 0.
int check_dash(char *option);

// check if all following arguments are options.
// Return 1 if all the arguments are options.
// Otherwise, return 0.
int verify_options(char **o_flags);

// check if all following arguments are directories.
// Return 1 if all the arguments are directories.
// Otherwise, return 0.
int verify_dirs(char **dirs);

// compares an option to a single flag.
// Return 1 if option matches given flag; otherwise 0.
int compare_flag(char *option, char flag);

// check if l is in the list of options. If l exists,
// the following three functions can be set. 
// Return 1 if 'l' is detected. Otherwise, return 0.
int l_set(char **options);

// given only option flags, check if 'a' is
// specified with l also set. 
// Return 1 if 'a' is detected.  Otherwise, return 0.
int a_set(char **options);

// given only option flags, check if 'c' is
// specified with l also set.  
// Return 1 if 'c' is detected. Otherwise, return 0.
int c_set(char **options);

// given only option flags, check if 'm' is
// specified with l also set. 
// Return 1 if 'm' is detected. Otherwise, return 0.
int m_set(char **options);

// check user rwx bits
void check_usr_perms(char *file, struct stat file_info);

// check group rwx bits
void check_grp_perms(char *file, struct stat file_info);

// check other rwx bits
void check_oth_perms(char *file, struct stat file_info);

// get the length of the largest file's bytes in a
// directory
int get_digits(char **files, struct stat file_info);

// print all contents for the -l option, provided the 
// -l flag is set, with no specified directories.
void l_arg_all(char **files, int a_flag, int c_flag, int m_flag);

// return a list of all files from a given directory.
char **grab_files(char *dirname);

// return a list of all files from a given directory
// without printing its values.
char **grab_nfiles(char *dirname);

#endif
