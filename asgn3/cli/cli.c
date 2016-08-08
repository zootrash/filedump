/* cli.c

		main program for the command line interpreter.
		takes in commands and interprets them as internal or external commands, including
		any bash functions.

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "misc.h"

#define MAX_LINE 1024

// main
int main() {

	char line[MAX_LINE];

	// store cwd in path
	char cwdpath[MAX_PATH_SIZE];
	strcpy(cwdpath, getcwd(cwdpath, MAX_PATH_SIZE));

	char **tokens;
	int num_tokens;

	// for child process
	pid_t status;
	int childexit;

	// return values for external commands
	int fref_ret;
	int listf_ret;
	int calc_ret;
	int bash_ret;

	// exec paths for external commands
	char fref_path[MAX_PATH_SIZE];
	char listf_path[MAX_PATH_SIZE];
	char calc_path[MAX_PATH_SIZE];

	// start prompt
	printf("$> ");
	while(fgets(line, MAX_LINE, stdin)) {
		// check for redirection
		int c_in = check_input(line);
		int c_out = check_output(line);

		// child process for redirection
		if(c_in == 0 || c_out == 0) {

			// tokenize user input and number of tokens
			// if only "<" exists
			if(c_in == 0 && c_out == 1) {
				tokens = tokenize(line, " ");
				num_tokens = token_count(tokens);

				// check if user entered "exit"
				if(exit_cli(tokens[0]) == 0) {
					return -1;
				}

				// check if user entered "cd"
				if((check_cd(tokens[0]) == 0) && num_tokens <= 1) {
					cd_home();
					status = fork();
					if(status < 0) {
						printf("cd fork failed.\n");
					}

					if(status == 0) {
						// if "~" is entered
						if((check_tilda(tokens[1]) == 0) && strlen(tokens[1]) == 1) {
							if((strcmp(tokens[num_tokens-1], "<") == 0)) {
								cd_home();
							} else {
								freopen(tokens[num_tokens-1], "r", stdin);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								cd_home();
							}

						// if "~/..." is entered, strcat	
						} else if((corr_tilda_path(tokens) == 0) && strlen(tokens[1]) > 1) {
							if((strcmp(tokens[num_tokens-1], "<") == 0)) {
								cd_tilda(tokens[1]);
							} else {
								freopen(tokens[num_tokens-1], "r", stdin);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								cd_tilda(tokens[1]);
							}

						// if "cd ." is entered
						} else if(check_s_dot(tokens[1]) == 0) {
							if((strcmp(tokens[num_tokens-1], "<") == 0)) {
								s_dot();	
							} else {
								freopen(tokens[num_tokens-1], "r", stdin);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								s_dot();
							}

						// if "cd .." is entered
						} else if(check_d_dot(tokens[1]) == 0) {
							if((strcmp(tokens[num_tokens-1], "<") == 0)) {
								d_dot();	
							} else {
								freopen(tokens[num_tokens-1], "r", stdin);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								d_dot();
							}

						// if "cd ..." is entered as a rel/abs path
						} else {
							// cd <
							if(num_tokens == 2) {
								freopen(tokens[1], "r", stdin);
								cd_home();

							// cd directory <
							} else if(num_tokens == 3) {
									if((strcmp(tokens[num_tokens-1], "<") == 0)) {
										cd_rel(tokens[1]);
									} else {
										freopen(tokens[num_tokens-1], "r", stdin);
										tokens[num_tokens-1] = '\0';
										tokens[num_tokens-2] = '\0';
										cd_rel(tokens[1]);
									}

							// cd directory < ...
							} else if(num_tokens > 3) {
									if((strcmp(tokens[num_tokens-1], "<") == 0)) {
										cd_rel(tokens[1]);
									} else {
										freopen(tokens[num_tokens-1], "r", stdin);
										tokens[num_tokens-1] = '\0';
										tokens[num_tokens-2] = '\0';
										cd_rel(tokens[1]);
									}
							
							} else {
								printf("wrong number of arguments.\n");
							}
						}
						return 0;
					// cd child terminate

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}
				}

			// if "pwd" is entered by user.
			if(check_pwd(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("pwd fork failed.\n");
				}

				if(status == 0) {
					if((strcmp(tokens[num_tokens-1], "<") == 0)) {
						get_pwd();
					} else {
						freopen(tokens[num_tokens-1], "r", stdin);
						get_pwd();
					}
					return 0;
				// pwd child terminate

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			}

			// if "fref" is entered
			if(check_fref(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("fref fork failed.\n");
				}

				// fref child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(fref_path, getcwd(fref_path, MAX_PATH_SIZE));
					strcat(fref_path, "/");
					strcat(fref_path, tokens[0]);
					chdir(cwdpath);

					// open stdout file and parse the file
					if((strcmp(tokens[num_tokens-1], "<") == 0)) {
						tokens[num_tokens-1] = '\0';
						fref_ret = execv(fref_path, tokens);
					} else {
						freopen(tokens[num_tokens-1], "r", stdin);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						fref_ret = execv(fref_path, tokens);
					}
					if(fref_ret == -1) {
						perror("execv(fref) failed: ");
					}
					return 0;
				// fref child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end fref

			// if "listf" is entered by user
			if(check_listf(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("listf fork failed.\n");
				}

				// listf child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(listf_path, getcwd(listf_path, MAX_PATH_SIZE));
					strcat(listf_path, "/");
					strcat(listf_path, tokens[0]);
					chdir(cwdpath);

					// open stdout file and parse the file
					if((strcmp(tokens[num_tokens-1], "<") == 0)) {
						tokens[num_tokens-1] = '\0';
						listf_ret = execv(listf_path, tokens);
					} else {
						freopen(tokens[num_tokens-1], "<", stdin);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						listf_ret = execv(listf_path, tokens);
					}
					if(listf_ret == -1) {
						perror("execv(listf) failed: ");
					}
					return 0;
				// listf child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end check listf

			// if "calc" is entered by user
			if(check_calc(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("calc fork failed.\n");
				}

				// calc child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(calc_path, getcwd(calc_path, MAX_PATH_SIZE));
					strcat(calc_path, "/");
					strcat(calc_path, tokens[0]);
					chdir(cwdpath);

					// open stdout file and parse the file
					if((strcmp(tokens[num_tokens-1], "<") == 0)) {
						tokens[num_tokens-1] = '\0';
						calc_ret = execv(calc_path, tokens);
					} else {
						freopen(tokens[num_tokens-1], "r", stdin);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						calc_ret = execv(calc_path, tokens);
					}
					if(calc_ret == -1) {
						perror("execv(calc) failed: ");
					}
					return 0;
				// calc child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end check calc

			// if other command is entered by user
			if((exit_cli(tokens[0]) != 0) && (check_cd(tokens[0]) != 0) && (check_pwd(tokens[0]) != 0) && (check_fref(tokens[0]) != 0) && (check_listf(tokens[0]) !=0) && (check_calc(tokens[0]) !=0)) {
				status = fork();
				if(status < 0) {
					printf("bash fork failed.\n");
				}

				// bash child process
				if(status == 0) {
					if((strcmp(tokens[num_tokens-1], "<") == 0)) {
						tokens[num_tokens-1] = '\0';
						bash_ret = execvp(tokens[0], tokens);
					} else {
						freopen(tokens[num_tokens-1], "r", stdin);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						bash_ret = execvp(tokens[0], tokens);
					}
					if(bash_ret == -1) {
						perror("execvp(bash) failed: ");
					}
					return 0;
				// bash child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end check bash

			// if only ">" exists
			} else if(c_in == 1 && c_out == 0) {
				tokens = tokenize(line, " ");
				num_tokens = token_count(tokens);

				// if user entered "exit"
				if(exit_cli(tokens[0]) == 0) {
					return -1;
				}

				// if user entered "cd"
				if((check_cd(tokens[0]) == 0) && num_tokens <= 1) {
					cd_home();
				} else if((check_cd(tokens[0]) == 0) && num_tokens > 1) {
					status = fork();
					if(status < 0) {
						printf("cd fork failed.\n");
					}

					if(status == 0) {
						// if "~" is entered
						if((check_tilda(tokens[1]) == 0) && strlen(tokens[1]) == 1) {
							if((strcmp(tokens[num_tokens-1], ">") == 0)) {
								cd_home();
							} else {
								freopen(tokens[num_tokens-1], "w", stdout);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								cd_home();
							}

						// if "~/..." is entered, strcat	
						} else if((corr_tilda_path(tokens) == 0) && strlen(tokens[1]) > 1) {
							if((strcmp(tokens[num_tokens-1], ">") == 0)) {
								cd_tilda(tokens[1]);
							} else {
								freopen(tokens[num_tokens-1], "w", stdout);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								cd_tilda(tokens[1]);
							}

						// if "cd ." is entered
						} else if(check_s_dot(tokens[1]) == 0) {
							if((strcmp(tokens[num_tokens-1], ">") == 0)) {
								s_dot();	
							} else {
								freopen(tokens[num_tokens-1], "w", stdout);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								s_dot();
							}

						// if "cd .." is entered
						} else if(check_d_dot(tokens[1]) == 0) {
							if((strcmp(tokens[num_tokens-1], ">") == 0)) {
								d_dot();	
							} else {
								freopen(tokens[num_tokens-1], "w", stdout);
								tokens[num_tokens-1] = '\0';
								tokens[num_tokens-2] = '\0';
								d_dot();
							}

						// if "cd ..." is entered as a rel/abs path
						} else {
							// cd >
							if(num_tokens == 2) {
								freopen(tokens[1], "w", stdout);
								cd_home();

							// cd directory > 
							} else if(num_tokens == 3) {
									if((strcmp(tokens[num_tokens-1], ">") == 0)) {
										cd_rel(tokens[1]);
									} else {
										freopen(tokens[num_tokens-1], "w", stdout);
										tokens[num_tokens-1] = '\0';
										tokens[num_tokens-2] = '\0';
										cd_rel(tokens[1]);
									}

							// cd directory > ...
							} else if(num_tokens > 3) {
									if((strcmp(tokens[num_tokens-1], ">") == 0)) {
										cd_rel(tokens[1]);
									} else {
										freopen(tokens[num_tokens-1], "w", stdout);
										tokens[num_tokens-1] = '\0';
										tokens[num_tokens-2] = '\0';
										cd_rel(tokens[1]);
									}
							
							} else {
								printf("wrong number of arguments.\n");
							}
						}
						return 0;
					// cd child terminate

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}
				}

			// if "pwd" is entered by user.
			if(check_pwd(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("pwd fork failed.\n");
				}

				if(status == 0) {
					if((strcmp(tokens[num_tokens-1], ">") == 0)) {
						get_pwd();
					} else {
						freopen(tokens[num_tokens-1], "w", stdout);
						get_pwd();
					}
					return 0;
				// pwd child terminate

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			}

			// if "fref" is entered
			if(check_fref(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("fref fork failed.\n");
				}

				// fref child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(fref_path, getcwd(fref_path, MAX_PATH_SIZE));
					strcat(fref_path, "/");
					strcat(fref_path, tokens[0]);
					chdir(cwdpath);

					// open stdout file and parse the file
					if((strcmp(tokens[num_tokens-1], ">") == 0)) {
						tokens[num_tokens-1] = '\0';
						fref_ret = execv(fref_path, tokens);
					} else {
						freopen(tokens[num_tokens-1], "w", stdout);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						fref_ret = execv(fref_path, tokens);
					}
					if(fref_ret == -1) {
						perror("execv(fref) failed: ");
					}
					return 0;
				// fref child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end fref

			// if "listf" is entered by user
			if(check_listf(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("listf fork failed.\n");
				}

				// listf child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(listf_path, getcwd(listf_path, MAX_PATH_SIZE));
					strcat(listf_path, "/");
					strcat(listf_path, tokens[0]);
					chdir(cwdpath);

					// open stdout file and parse the file
					if((strcmp(tokens[num_tokens-1], ">") == 0)) {
						tokens[num_tokens-1] = '\0';
						listf_ret = execv(listf_path, tokens);
					} else {
						freopen(tokens[num_tokens-1], "w", stdout);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						listf_ret = execv(listf_path, tokens);
					}
					if(listf_ret == -1) {
						perror("execv(listf) failed: ");
					}
					return 0;
				// listf child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end check listf

			// if "calc" is entered by user
			if(check_calc(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("calc fork failed.\n");
				}

				// calc child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(calc_path, getcwd(calc_path, MAX_PATH_SIZE));
					strcat(calc_path, "/");
					strcat(calc_path, tokens[0]);
					chdir(cwdpath);

					// open stdout file and parse the file
					if((strcmp(tokens[num_tokens-1], ">") == 0)) {
						tokens[num_tokens-1] = '\0';
						calc_ret = execv(calc_path, tokens);
					} else {
						freopen(tokens[num_tokens-1], "w", stdout);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						calc_ret = execv(calc_path, tokens);
					}
					if(calc_ret == -1) {
						perror("execv(calc) failed: ");
					}
					return 0;
				// calc child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end check calc

			// if other command is entered by user
			if((exit_cli(tokens[0]) != 0) && (check_cd(tokens[0]) != 0) && (check_pwd(tokens[0]) != 0) && (check_fref(tokens[0]) != 0) && (check_listf(tokens[0]) !=0) && (check_calc(tokens[0]) !=0)) {
				status = fork();
				if(status < 0) {
					printf("bash fork failed.\n");
				}

				// bash child process
				if(status == 0) {
					if((strcmp(tokens[num_tokens-1], ">") == 0)) {
						tokens[num_tokens-1] = '\0';
						bash_ret = execvp(tokens[0], tokens);
					} else {
						freopen(tokens[num_tokens-1], "w", stdout);
						tokens[num_tokens-1] = '\0';
						tokens[num_tokens-2] = '\0';
						bash_ret = execvp(tokens[0], tokens);
					}
					if(bash_ret == -1) {
						perror("execvp(bash) failed: ");
					}
					return 0;
				// bash child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			} // end check bash
				
			// if both "<" and ">" exists
			} else if(c_in == 0 && c_out == 0) {
				tokens = tokenize(line, " ");
				num_tokens = token_count(tokens);

				// is user entered "exit"
				if(exit_cli(tokens[0]) == 0) {
					return -1;
				}

				// if user entered "cd"
				if((check_cd(tokens[0]) == 0) && num_tokens <= 1) {
					cd_home();
				} else if((check_cd(tokens[0]) == 0) && num_tokens > 1) {
					status = fork();
					if(status < 0) {
						printf("cd fork failed.\n");
					}

					if(status == 0) {
						// if "~" is entered
						if((check_tilda(tokens[1]) == 0) && strlen(tokens[1]) == 1) {
							// cd ~ < > || cd ~ > <
							if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") == 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") == 0))) {
								cd_home();

							//cd ~ < file > || cd ~ > file <
							} else if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0))) {
								if((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) {
									cd_home();
								} else if((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0)) {
									freopen(tokens[num_tokens-2], "w", stdout);
									tokens[num_tokens-1] = '\0';
									tokens[num_tokens-2] = '\0';
									tokens[num_tokens-3] = '\0';
									cd_home();
								}

							// cd ~ < file > file
							} else {
									if((strcmp(tokens[num_tokens-2], ">") == 0)) {
										freopen(tokens[num_tokens-3], "r", stdin);
										freopen(tokens[num_tokens-1], "w", stdout);
										cd_home();
									}
							}

					// if "~/..." is entered, strcat
					} else if((corr_tilda_path(tokens) == 0) && strlen(tokens[1]) > 1) {
							// cd ~/... < > || cd ~/... > <	
							if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") == 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") == 0))) {
								cd_home();	

							// cd ~/... < file > || cd ~/... > file <
							} else if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0))) {
								if((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) {
									cd_tilda(tokens[1]);
								} else if((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0)) {
									freopen(tokens[num_tokens-2], "w", stdout);
									tokens[num_tokens-1] = '\0';
									tokens[num_tokens-2] = '\0';
									tokens[num_tokens-3] = '\0';
									cd_tilda(tokens[1]);
								}

							// cd ~/... < file > file
							} else {
									if((strcmp(tokens[num_tokens-2], ">") == 0)) {
										freopen(tokens[num_tokens-3], "r", stdin);
										freopen(tokens[num_tokens-1], "w", stdout);
										cd_tilda(tokens[1]);
									}
							}

					// if "cd ." is entered
					} else if(check_s_dot(tokens[1]) == 0) {
							// cd . < > || cd . > <	
							if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") == 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") == 0))) {
								s_dot();	

							// cd . < file > || cd . > file <
							} else if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0))) {
								if((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) {
									s_dot();
								} else if((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0)) {
									freopen(tokens[num_tokens-2], "w", stdout);
									tokens[num_tokens-1] = '\0';
									tokens[num_tokens-2] = '\0';
									tokens[num_tokens-3] = '\0';
									s_dot();
								}

							// cd . < file > file
							} else {
									if((strcmp(tokens[num_tokens-2], ">") == 0)) {
										freopen(tokens[num_tokens-3], "r", stdin);
										freopen(tokens[num_tokens-1], "w", stdout);
										s_dot();
									}
							}

					// if "cd .." is entered
					} else if(check_d_dot(tokens[1]) == 0) {
							// cd .. < > || cd .. > <	
							if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") == 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") == 0))) {
								d_dot();	

							// cd .. < file > || cd .. > file <
							} else if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0))) {
								if((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) {
									d_dot();
								} else if((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0)) {
									freopen(tokens[num_tokens-2], "w", stdout);
									tokens[num_tokens-1] = '\0';
									tokens[num_tokens-2] = '\0';
									tokens[num_tokens-3] = '\0';
									d_dot();
								}

							// cd .. < file > file
							} else {
									if((strcmp(tokens[num_tokens-2], ">") == 0)) {
										freopen(tokens[num_tokens-3], "r", stdin);
										freopen(tokens[num_tokens-1], "w", stdout);
										d_dot();
									}
							}

					// if "cd ..." is entered as a rel/abs path
					} else {
							// cd ... < > || cd ... > <	
							if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") == 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") == 0))) {
								cd_rel(tokens[1]);	

							// cd ... < file > || cd ... > file <
							} else if(((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) || ((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0))) {
								if((strcmp(tokens[num_tokens-1], ">") == 0) && (strcmp(tokens[num_tokens-2], "<") != 0)) {
									cd_rel(tokens[1]);
								} else if((strcmp(tokens[num_tokens-1], "<") == 0) && (strcmp(tokens[num_tokens-2], ">") != 0)) {
									freopen(tokens[num_tokens-2], "w", stdout);
									tokens[num_tokens-1] = '\0';
									tokens[num_tokens-2] = '\0';
									tokens[num_tokens-3] = '\0';
									cd_rel(tokens[1]);
								}

							// cd ... < file > file
							} else {
									if((strcmp(tokens[num_tokens-2], ">") == 0)) {
										freopen(tokens[num_tokens-3], "r", stdin);
										freopen(tokens[num_tokens-1], "w", stdout);
										cd_rel(tokens[1]);
									}
							}

					}
					return 0;
					// cd child process terminated

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}
				} // end cd check

				// if "pwd" is entered by user
				if(check_pwd(tokens[0]) == 0) {
					status = fork();
					if(status < 0) {
						printf("pwd fork failed.\n");
					}

					if(status == 0) {
						// pwd < file > file
						if((strcmp(tokens[num_tokens-1], ">") != 0) && (strcmp(tokens[num_tokens-2], ">") == 0)) {
							freopen(tokens[num_tokens-1], "w", stdout);
							freopen(tokens[num_tokens-3], "r", stdin);
							get_pwd();
						} else {
							get_pwd();
						}
					return 0;
					// pwd child terminated

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}
				}// end pwd check

				// if "fref" is entered
				if(check_fref(tokens[0]) == 0) {
					status = fork();
					if(status < 0) {
						printf("fref fork failed.\n");
					}

					if(status == 0) {
						chdir("..");
						chdir(tokens[0]);
						strcpy(fref_path, getcwd(fref_path, MAX_PATH_SIZE));
						strcat(fref_path, "/");
						strcat(fref_path, tokens[0]);
						chdir(cwdpath);
	
						if((strcmp(tokens[num_tokens-1], ">") != 0) && (strcmp(tokens[num_tokens-2], ">") == 0)) {

							// fref "regexp" file1 file2 < test.txt > test.txt
							freopen(tokens[num_tokens-1], "w", stdout);
							freopen(tokens[num_tokens-3], "r", stdin);
							tokens[num_tokens-1] = '\0';
							tokens[num_tokens-2] = '\0';
							tokens[num_tokens-3] = '\0';
							tokens[num_tokens-4] = '\0';
							fref_ret = execv(fref_path, tokens);

						}
						if(fref_ret == -1) {
							perror("execv(fref) failed: ");
						}
						return 0;
						// fref child terminated

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}
				}// end fref check

				// if "listf" is entered
				if(check_listf(tokens[0]) == 0) {
					status = fork();
					if(status < 0) {
						printf("listf fork failed.\n");
					}

					if(status == 0) {
						chdir("..");
						chdir(tokens[0]);
						strcpy(listf_path, getcwd(listf_path, MAX_PATH_SIZE));
						strcat(listf_path, "/");
						strcat(listf_path, tokens[0]);
						chdir(cwdpath);

						if((strcmp(tokens[num_tokens-1], ">") != 0) && (strcmp(tokens[num_tokens-2], ">") == 0)) {	
							// listf ... < test.txt > test.txt
							freopen(tokens[num_tokens-1], "w", stdout);
							freopen(tokens[num_tokens-3], "r", stdin);
							tokens[num_tokens-1] = '\0';
							tokens[num_tokens-2] = '\0';
							tokens[num_tokens-3] = '\0';
							tokens[num_tokens-4] = '\0';
							listf_ret = execv(listf_path, tokens);
						}
						if(listf_ret == -1) {
							perror("execv(listf) failed: ");
						}
						return 0;
					// listf child terminated

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}
				} // end listf check

				// if other command is entered by user
				if((exit_cli(tokens[0]) != 0) && (check_cd(tokens[0]) != 0) && (check_pwd(tokens[0]) != 0) && (check_fref(tokens[0]) != 0) && (check_listf(tokens[0]) !=0) && (check_calc(tokens[0]) !=0)) {
					status = fork();
					if(status < 0) {
						printf("bash fork failed.\n");
					}

					// bash child process
					if(status == 0) {	
						if((strcmp(tokens[num_tokens-1], ">") != 0) && (strcmp(tokens[num_tokens-2], ">") == 0)) {	
							freopen(tokens[num_tokens-1], "w", stdout);
							freopen(tokens[num_tokens-3], "r", stdin);
							tokens[num_tokens-1] = '\0';
							tokens[num_tokens-2] = '\0';
							tokens[num_tokens-3] = '\0';
							tokens[num_tokens-4] = '\0';
							bash_ret = execvp(tokens[0], tokens);
						}
						if(bash_ret == -1) {
							perror("execvp(bash) failed: ");
						}
						return 0;
					// bash child terminated

					// parent waiting for child to terminate
					} else {
						wait(&childexit);
					}

				} // end bash check

			} // end redirection both ways check

			printf("$> ");

		// else, if redirection does not exist
		} else {
				wait(&childexit);
				tokens = tokenize(line, " ");
				num_tokens = token_count(tokens);

				// check if user entered "exit"
				if(exit_cli(tokens[0]) == 0) {
					return -1;
				}

				// check if user wants to "cd"
				if((check_cd(tokens[0]) == 0) && num_tokens == 1) {
					cd_home();
				} else if((check_cd(tokens[0]) == 0) && num_tokens > 1) {

				// if "~" is only entered as input, cd_home.
				if((check_tilda(tokens[1]) == 0) && strlen(tokens[1]) == 1) {
					cd_home();

				// if "~/..." is entered, strcat.
				} else if((corr_tilda_path(tokens) == 0) && strlen(tokens[1]) > 1) {
					cd_tilda(tokens[1]);

				// if "cd ." is entered
				} else if(check_s_dot(tokens[1]) == 0) {
					s_dot();	

				// if "cd .." is entered
				} else if(check_d_dot(tokens[1]) == 0) {
					d_dot();
			

				// if "cd ..." is entered as a rel/abs path.
				} else {
					if(num_tokens == 2) {
						cd_rel(tokens[1]);
					} else {
						printf("wrong number of arguments.\n");
					}
				}
			}

			// if "pwd" is entered by user.
			if(check_pwd(tokens[0]) == 0) {
				get_pwd();
			}

			// if "fref" is entered by user,
			// create a child process for fref
			if(check_fref(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("Fref fork failed.\n");
				}

				// fref child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(fref_path, getcwd(fref_path, MAX_PATH_SIZE));
					strcat(fref_path, "/");
					strcat(fref_path, tokens[0]);
					chdir(cwdpath);
					fref_ret = execv(fref_path, tokens);
					if(fref_ret == -1) {
						perror("execv(fref) failed: ");
					}
					printf("$> ");
					return 0;
				// fref child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			}

			// if "listf" is entered by user
			if(check_listf(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("Listf fork failed.\n");
				}

				// listf child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(listf_path, getcwd(listf_path, MAX_PATH_SIZE));
					strcat(listf_path, "/");
					strcat(listf_path, tokens[0]);
					chdir(cwdpath);
					listf_ret = execv(listf_path, tokens);
					if(listf_ret == -1) {
						perror("execv(listf) failed: ");
					}
					printf("$> ");
					return 0;
				// listf child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			}

			// if "calc" is entered by user
			if(check_calc(tokens[0]) == 0) {
				status = fork();
				if(status < 0) {
					printf("Calc fork failed.\n");
				}

				// calc child process
				if(status == 0) {
					chdir("..");
					chdir(tokens[0]);
					strcpy(calc_path, getcwd(calc_path, MAX_PATH_SIZE));
					strcat(calc_path, "/");
					strcat(calc_path, tokens[0]);
					chdir(cwdpath);
					calc_ret = execv(calc_path, tokens);
					if(calc_ret == -1) {
						perror("execv(calc) failed: ");
					}
					printf("$> ");
					return 0;
				// calc child terminated

				// parent waiting for child to terminate
				} else {
					wait(&childexit);
				}
			}

			// if other command is entered by user
			if((exit_cli(tokens[0]) != 0) && (check_cd(tokens[0]) != 0) && (check_pwd(tokens[0]) != 0) && (check_fref(tokens[0]) != 0) && (check_listf(tokens[0]) !=0) && (check_calc(tokens[0]) !=0)) {
				// do bash commands
				status = fork();
				if(status < 0) {
					printf("Bash fork failed.\n");
				}

				// bash child process
				if(status == 0) {
					bash_ret = execvp(tokens[0], tokens);
					if(bash_ret == -1) {
						perror("execv(bash) failed: ");
					}
					printf("$> ");
					return 0;
				// bash child terminated

				// parent waiting for bash child to terminate
				} else {
					wait(&childexit);
				}
			}

			printf("$> ");
		}
	} 
	return 0;
}
