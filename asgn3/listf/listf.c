/* listf.c

		source file for listf
		lists file info for directories and files with optional flags.

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "listf.h"

int main(int argc, char *argv[]) {
	// current directory and its info
	DIR *curr_dir;
	struct dirent *curr_p;

	// list of files
	char **files = malloc(sizeof(files)*MAX_NUMFILES);
	int f = 0;

	// check if arguments are all options or
	// all directories
	int all_options = verify_options(argv);
	int all_dirs = verify_dirs(argv);

	// check if current directory is null
	if((curr_dir = opendir(".")) == NULL) {
		printf("Directory does not exist.\n");
		return 1;
	}	

	// if only "listf" is entered:
	if(argc == 1) {	
		while((curr_p = readdir(curr_dir)) != NULL) {
			// filter out . and ..
			if((strcmp(curr_p->d_name, ".") != 0) && (strcmp(curr_p->d_name, "..") != 0)) {
				files[f] = curr_p->d_name;
				f++;
			}	
		}
		display_allfiles(files, f);
		closedir(curr_dir);

	// if "listf" is followed only with options
	} else if(argc >= 2 && all_options == 1) {

		// set -l flag
		int l_flag = l_set(argv);
		if(l_flag == 0) {
			printf("Missing -l flag.\n");
			return 1;
		}

		// fill files and sort them alphabetically
		while((curr_p = readdir(curr_dir)) != NULL) {
			// filter out . and .. 
			if((strcmp(curr_p->d_name, ".") != 0) && (strcmp(curr_p->d_name, "..") != 0)) {
				files[f] = curr_p->d_name;
				f++;
			}
		}
		files = sort_files(files);

		// assumes l flag exists
		int a_flag = a_set(argv);
		int c_flag = c_set(argv);
		int m_flag = m_set(argv);
		
		// pass flag to l_arg_all for options set.
		l_arg_all(files, a_flag, c_flag, m_flag);
		closedir(curr_dir);
		
	// if "listf" is followed only with directories
	} else if(argc >= 2 && all_dirs == 1) {
		int v;

		// for each directory, print out its contents
		for(v=1; argv[v]; v++) {
			char **dfiles = malloc(sizeof(dfiles)*MAX_NUMFILES);
			dfiles = grab_files(argv[v]);
			free(dfiles);
		}

	// if "listf" is followed by both options and directories
	} else if(argc >= 2 && all_options == 0 && all_dirs == 0) {
		// set -l flag
		int l_flag = l_set(argv);
		if(l_flag == 0) {
			printf("Missing -l flag.\n");
			return 1;
		}

		// assumes l flag exists
		int a_flag = a_set(argv);
		int c_flag = c_set(argv);
		int m_flag = m_set(argv);
		int v;

		// path for cwd
		char cwdpath[MAX_PATH_SIZE];
		strcpy(cwdpath, getcwd(cwdpath, MAX_PATH_SIZE));

		for(v=1; argv[v]; v++) {
			char **dfiles = malloc(sizeof(dfiles)*MAX_NUMFILES);
			// check if argument is a directory or a flag.
			if(check_dash(argv[v]) == 0) {
				//if((curr_dir = opendir(argv[v])) == NULL) {
				//	printf("Could not open directory %s\n", argv[v]);
				//} else {
					dfiles = grab_nfiles(argv[v]);
					if((curr_dir = opendir(argv[v])) != NULL) {
						chdir(argv[v]);
					}
					l_arg_all(dfiles, a_flag, c_flag, m_flag);	
					chdir(cwdpath);
					free(dfiles);
					printf("\n");
				//}
			}
		}

	} else {
		printf("Invalid number of arguments.\n");
	}
}

// sort files in alphabetical order
char **sort_files(char **files) {
	int f;
	int i;
	int f_count = token_count(files);
	char temp_file[MAX_FILENAME_SIZE];

	if(f_count == 0) {
		printf("no entries found.\n");
	} else if(f_count == 1) {
		printf("%d entry found.\n", f_count);
	} else {
		printf("%d entries found.\n", f_count);
	}

	for(f=1; f<f_count; f++) {
		for(i=1; i<f_count; i++) {
			if(strcmp(files[i-1], files[i]) > 0) {
				strcpy(temp_file, files[i-1]);
				strcpy(files[i-1], files[i]);
				strcpy(files[i], temp_file);
			}
		}
	}

	return files;
}

// if "listf" is entered by the user, display
// all of the files in the current directory.
void display_allfiles(char **files, int f) {
		files = sort_files(files);
		for(f=0; files[f]; f++) {
			printf("%s ", files[f]);
		} 
		printf("\n");
}

// display all files each on a new line
void display_nfiles(char **files) {
	int f;
	//files = sort_files(files);
	for(f=0; files[f]; f++) {
		printf("%s\n", files[f]);
	}
	printf("\n");
}

// check if argument begins with a "-"
// if options begins with a "-", return 1.
// Otherwise return 0.
int check_dash(char *option) {
	if(option[0] == '-') {
		return 1;	
	}	else {
		return 0;
	}
}

// check if all following arguments are options.
// Return 1 if all the arguments are options.
// Otherwise, return 0.
int verify_options(char **o_flags) {
	int i;
	for(i=1; o_flags[i]; i++) {
		if(check_dash(o_flags[i]) == 0) {
			return 0;
		}
	}
	return 1;
}

// check if all following arguments are directories.
// Return 1 if all the arguments are directories.
// Otherwise, return 0.
int verify_dirs(char **dirs) {
	int i;
	for(i=1; dirs[i]; i++) {
		if(check_dash(dirs[i]) == 1) {
			return 0;
		}
	}
	return 1;
}

// compares an option to a single flag.
// Return 1 if option matches given flag; otherwise 0.
int compare_flag(char *option, char flag) {
	int i;
	size_t option_size = strlen(option);

	for(i=0; i<option_size; i++) {
		if(option[i] == flag) {
			return 1;
		}
	}
	
	return 0;
}

// check if l is in the list of options. If l exists,
// the following three functions can be set.
// Return 1 if 'l' is detected. Otherwise, return 0.
int l_set(char **options) {
	int i;
	for(i=1; options[i]; i++) {
		if(check_dash(options[i]) == 1) {
			if((compare_flag(options[i], 'l') == 1)) {
				return 1;
			}
		}
	}	
	return 0;
}

// given only option flags, check if 'a' is
// specified with l also set.
// Return 1 if 'a' is detected. Otherwise, return 0.
int a_set(char **options) {
	int i;
	for(i=1; options[i]; i++) {
		if(check_dash(options[i]) == 1){
			if(compare_flag(options[i], 'a') == 1) {
				return 1;
			}
		}
	}	
	return 0;
}

// given only option flags, check if 'c' is
// specified with l also set.
// Return 1 if 'c' is detected. Otherwise, return 0.
int c_set(char **options) {
	int i;
	for(i=1; options[i]; i++) {
		if(check_dash(options[i]) == 1) {
			if(compare_flag(options[i], 'c') == 1) {
				return 1;
			}
		}
	}	
	return 0;
}

// given only option flags, check if 'm' is
// specified with l also set.
// Return 1 if 'm' is detected. Otherwise, return 0.
int m_set(char **options) {
	int i;
	for(i=1; options[i]; i++) {
		if(check_dash(options[i]) == 1) {
			if(compare_flag(options[i], 'm') == 1) {
				return 1;
			}
		}
	}	
	return 0;
}

// check user rwx bits
void check_usr_perms(char *file, struct stat file_info) {
	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IRUSR)) == S_IRUSR)) {
 		printf("r");
	} else {
		printf("-");
	}

	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IWUSR)) == S_IWUSR)) {
 		printf("w");
	} else {
		printf("-");
	}

	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IXUSR)) == S_IXUSR)) {
 		printf("x");
	} else {
		printf("-");
	}
}

// check group rwx bits
void check_grp_perms(char *file, struct stat file_info) {
	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IRGRP)) == S_IRGRP)) {
 		printf("r");
	} else {
		printf("-");
	}

	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IWGRP)) == S_IWGRP)) {
 		printf("w");
	} else {
		printf("-");
	}

	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IXGRP)) == S_IXGRP)) {
 		printf("x");
	} else {
		printf("-");
	}
}

// check other rwx bits
void check_oth_perms(char *file, struct stat file_info) {
	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IROTH)) == S_IROTH)) {
 		printf("r");
	} else {
		printf("-");
	}

	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IWOTH)) == S_IWOTH)) {
 		printf("w");
	} else {
		printf("-");
	}

	if((stat(file, &file_info) == 0) && ((file_info.st_mode & (S_IXOTH)) == S_IXOTH)) {
 		printf("x");
	} else {
		printf("-");
	}
}

// get the length of the largest file's bytes in a
// directory
int get_digits(char **files, struct stat file_info) {
	int i;
	long int max = 0;
	for(i=0; files[i]; i++) {
		if(stat(files[i], &file_info) == 0) {
			if(file_info.st_size > max) {
				max = file_info.st_size;
			}
		}
	}

	if(max < TWO_DIGITS ) {
		return 1;
	} else if(max < THR_DIGITS) {
		return 2;
	} else if(max < FOU_DIGITS) {
		return 3;
	} else if(max < FIV_DIGITS) {
		return 4;
	} else if(max < SIX_DIGITS) {
		return 5;
	} else if(max < SEV_DIGITS) {
		return 6;
	} else if(max < EIG_DIGITS) {
		return 7;
	} else if(max < NIN_DIGITS) {
		return 8;
	} else if(max < TEN_DIGITS) {
		return 9;
	} else {
		return -1;
	}
}

// print contents for the -l option, provided the -l
// flag is set
void l_arg_all(char **files, int a_flag, int c_flag, int m_flag) {

	// store cwd in path
	char path[MAX_PATH_SIZE];
	strcpy(path, getcwd(path, MAX_PATH_SIZE));

	// stat to get each file's info
	struct stat file_info;

	// directory will be in a loop to loop through files
	// and check their file types
	int i;
	char filetype;
	for(i=0; files[i]; i++) {
		if(stat(files[i], &file_info) == 0) { 
			if(S_ISDIR(file_info.st_mode)) {
				filetype = 'd';
			} else if(S_ISREG(file_info.st_mode)) {
				filetype = '-';
			} else if(S_ISLNK(file_info.st_mode)){
				filetype = 'l';
			} else if((S_ISDIR(file_info.st_mode) == 0) && (S_ISREG(file_info.st_mode) == 0) && (S_ISLNK(file_info.st_mode) == 0)) {
				filetype = 'o';
			}

			// print permissions
			printf(" %c", filetype);
			check_usr_perms(files[i], file_info);
			check_grp_perms(files[i], file_info);
			check_oth_perms(files[i], file_info);

			// print number of links
			printf(" %lu", file_info.st_nlink);

			// print uid
			struct passwd *user_id;
			user_id = getpwuid(file_info.st_uid);
			printf(" %s", user_id->pw_name);

			// print gid
			struct group *group_id;
			group_id = getgrgid(file_info.st_gid);
			printf(" %s", group_id->gr_name);

			// print filesizes right-aligned by bytes
			int digits = get_digits(files, file_info);
			printf(" %*ld", digits, file_info.st_size);

			// right align time
			int time_length = TIME_LENGTH;

			// option flags to be displayed
			char mod_flag = 'M';
			char acc_flag = 'A';
			char cre_flag = 'C';

			// -l or -m or -lm or -l -m
			if((a_flag == 0 && c_flag == 0 && m_flag == 0) || (a_flag == 0 && c_flag == 0 && m_flag == 1)) {
				// get last modfied time
				time_t time = file_info.st_mtime;
				struct tm local;
				localtime_r(&time, &local);
				int year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", mod_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);

			// -la or -l -a
			} else if(a_flag == 1 && c_flag == 0 && m_flag == 0) {
				// get last accessed time
				time_t time = file_info.st_atime;
				struct tm local;
				localtime_r(&time, &local);
				int year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", acc_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);

			// -lc or -l -c
			} else if(a_flag == 0 && c_flag == 1 && m_flag == 0) {
				// get last creation time
				time_t time = file_info.st_ctime;
				struct tm local;
				localtime_r(&time, &local);
				int year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", cre_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);
				
			// -lma or -l -m -a
			} else if((a_flag == 1 && c_flag == 0 && m_flag == 1)) {
				// get last modified time
				time_t time = file_info.st_mtime;
				struct tm local;
				localtime_r(&time, &local);
				int year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;

				printf(" %c%*d/%*d/%*d-%*d:%*d", mod_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);
				// get last accessed time
				time = file_info.st_atime;
				localtime_r(&time, &local);
				year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", acc_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);

			// -lmc or -l -m -c
			} else if(a_flag == 0 && c_flag == 1 && m_flag == 1) {
				// get last modified time
				time_t time = file_info.st_mtime;
				struct tm local;
				localtime_r(&time, &local);
				int year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;

				printf(" %c%*d/%*d/%*d-%*d:%*d", mod_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);
				// get last creation time
				time = file_info.st_ctime;
				localtime_r(&time, &local);
				year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", cre_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);

			// -lmac or -l -m -a -c
			} else if(a_flag == 1 && c_flag == 1 && m_flag == 1) {
				// get last modified time
				time_t time = file_info.st_mtime;
				struct tm local;
				localtime_r(&time, &local);
				int year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;

				printf(" %c%*d/%*d/%*d-%*d:%*d", mod_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);
				// get last accessed time
				time = file_info.st_atime;
				localtime_r(&time, &local);
				year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", acc_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);
				// get last creation time
				time = file_info.st_ctime;
				localtime_r(&time, &local);
				year = (EPOCH_YEAR + local.tm_year) - MILLENIUM;
				printf(" %c%*d/%*d/%*d-%*d:%*d", cre_flag, TIME_LENGTH, local.tm_mon, TIME_LENGTH, local.tm_mday, TIME_LENGTH, year, TIME_LENGTH, local.tm_hour, time_length, local.tm_min);
			}

			//print file name ending with a newline character
			printf(" %s\n", files[i]);
		} 
		
	} 
} 

// return a list of all files from a given directory.
char **grab_files(char *dirname) {
	DIR *curr_dir;
	struct dirent *curr_p;
	struct stat file_info;
	char **files = malloc(sizeof(files)*MAX_NUMFILES);
	int f = 0;

	if((curr_dir = opendir(dirname)) == NULL) {
		if(stat(dirname, &file_info) == 0) {
			if(S_ISREG(file_info.st_mode)) {
				files[f] = dirname;
				files[f+1] = '\0';
			} else {
				printf("unknown file type.\n");
				return NULL;
			}
		}
	}	else {
		printf("%s\n", dirname);
		while((curr_p = readdir(curr_dir)) != NULL) {
			// filer out . and ..
			if((strcmp(curr_p->d_name, ".") != 0) && (strcmp(curr_p->d_name, "..") != 0)) {
				files[f] = curr_p->d_name;
				f++;
			}
		}
	}
	files = sort_files(files);
	display_nfiles(files);
	return files;
}

// return a list of all files from a given directory
// without printing its values.
char **grab_nfiles(char *dirname) {
	DIR *curr_dir;
	struct dirent *curr_p;
	struct stat file_info;
	char **files = malloc(sizeof(files)*MAX_NUMFILES);
	int f = 0;
	//int f_count;

	if((curr_dir = opendir(dirname)) == NULL) {
		if(stat(dirname, &file_info) == 0) {
			if(S_ISREG(file_info.st_mode)) {
				files[f] = dirname;
				files[f+1] = '\0';
			} else {
				printf("unknown file type.\n");
				return NULL;
			}
		}
	}	else {
		printf("%s\n", dirname);
		while((curr_p = readdir(curr_dir)) != NULL) {
			// filer out . and ..
			if((strcmp(curr_p->d_name, ".") != 0) && (strcmp(curr_p->d_name, "..") != 0)) {
				files[f] = curr_p->d_name;
				f++;
			}
		}
	}

	/*
	f_count = token_count(files);
	printf("f_count %d\n", f_count);
	if(f_count > 2) {
		files = sort_files(files);
	} */
	files = sort_files(files);
	return files;
}
