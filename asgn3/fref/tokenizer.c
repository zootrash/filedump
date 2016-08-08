/* tokenizer.c

		source file for tokenizer.

		Dillon Vicente, August 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

/* tokenize
 * breaks user input into separate tokens separated by
 * delimiters. Returns an array of strings, tokens.
 */
char **tokenize(char *line, char *delimiters) {	
	size_t line_s = strlen(line);
	size_t tokens_s = token_len(line, line_s, delimiters);
	char **tokens = (char **)malloc(sizeof(tokens)*tokens_s);

	if(line[0] == '\n') {
		return tokens;
	}

	int i;
	char *start_first = begin_p(line, line_s, delimiters);
	char *end_first = stop_p(start_first, line, line_s, delimiters);	
	char *word_p = malloc(sizeof(*word_p));
	char *start_p = malloc(sizeof(*start_p));
	char *end_p = malloc(sizeof(*end_p));

	for(i=0; i<tokens_s; i++){
		word_p = make_word(start_first, end_first);
		tokens[i] = word_p;
	
		start_p = shift_b(start_first, end_first, delimiters);
		end_p = shift_s(start_p, end_first, delimiters);

		start_first = start_p;
		end_first = end_p;
	}

	tokens[tokens_s] = '\0';
	return tokens;
}

/* token_count
 * returns a count of the number of strings in the null-
 * terminated array (excluding the null element)
 */
int token_count(char **tokens) {
	int i;
	int t_count = 0;

	for(i=0; tokens[i]; i++) {
		t_count++;
	}
	return t_count;
}

/* compare_char
 * compares the delimiters to a single char.
 * ret 1 if delimiter matches given char; otherwise 0.
 */
int compare_char(char *delims, char c) {
	int i;
	size_t delimSize = strlen(delims);

	for(i=0; i<delimSize; i++) {
		if(delims[i] == c || c == '\n') {
			return 1;
		}
	}

	return 0;
}

/* getword_s
 * returns the size of a word, given a starting and end 
 * pointer.
 */
size_t getword_s(char *start, char *end) {
	size_t size = 1;

	while(start != end) {
		start++;
		size++;
	}

	return size;
}

/* begin_p 
 * returns the beginning pointer of a word in the given
 * line.
 */
char *begin_p(char *line, size_t size, char *delims) {
	int i = 0;
	char *start_p = malloc(sizeof(*start_p));
	start_p = &(line[i]);
	int delim_c = compare_char(delims, *start_p);

	while(delim_c == 1) {
		i++;
		start_p = &(line[i]);
		delim_c = compare_char(delims, *start_p);
	}

	return start_p;
}

/* stop_p 
 * returns the pointer relative to the starting pointer,
 * stopping before the next token.
 */
char *stop_p(char *start_p, char *line, size_t size, char *delims) {
	int delim_e = no_delims(line, size, delims);
	int delim_c = 0;
	char *end_p = malloc(sizeof(*end_p));
	end_p = start_p;

	if(delim_e == 0) {
		end_p = &(line[size-1]);
		// why -2?
		//end_p = &(line[size-2]);
	} else {
		delim_c = compare_char(delims, *end_p);
		while(delim_c == 0) {
			delim_c = compare_char(delims, *end_p);	
			if(delim_c == 1) {
				end_p--;
			} else {
				end_p++;
			}
		}
	}

	return end_p;
}

/* no_delims
 * checks to see if there are any delimiters in the given line.
 * Returns 1 if a delimiter exists, 0 otherwise.
 */
int no_delims(char *line, size_t size, char *delims) {
	int i;
	int check = 0;

	for(i=0; i<size; i++) {
		check = compare_char(delims, line[i]);
		if(check == 1) {
			return 1;
		}
	}

	return 0;
}

/* make_word
 * makes a word from given pointers start and end.
 */
char *make_word(char *start, char *end) {
	int i;
	size_t word_s = getword_s(start, end);
	char *word_p;
	word_p = (char *)malloc(sizeof(word_p)*word_s);

	for(i=0; i<word_s; i++) {
		word_p[i] = *start;
		start++;
	}	

	return word_p;	
}

/* shift_b
 * shifts the beginning pointer to the next token.
 */
char *shift_b(char *start, char *end, char *delims) {
	int delim_c;
	char *start_p = malloc(sizeof(*start_p));
	start_p = end+1;
	delim_c = compare_char(delims, *start_p);

	while(delim_c == 1) {
		start_p++;
		delim_c = compare_char(delims, *start_p);
	}	

	return start_p;
} 

/* shift_s
 * shifts the stoppint pointer to the next token, relative
 * to the newly shifted beginning pointer.
 */
char *shift_s(char *start, char *end, char *delims) {
	int delim_c;
	char *end_p = malloc(sizeof(*end_p));
	end_p = start;
	delim_c = compare_char(delims, *end_p);

	while(delim_c == 0) {
		delim_c = compare_char(delims, *end_p);
		if(delim_c == 1) {
			end_p--;
		} else {
			end_p++;
		}
	}

	return end_p;
}

/* token_len
 * returns the number of words from input. Used to
 * initialize the size of token array.
 */
size_t token_len(char *line, size_t size, char *delims) {
	int i;
	size_t t_len = 0;	
	int sw = 0;
	int delim_c;

	for(i=0; i<size; i++) {
		delim_c = compare_char(delims, line[i]);
		if(delim_c == 0) {
			sw++;
			if(sw == 1) {
				t_len++;
			}
		} else {
			sw = 0;
		}	
	}

	return t_len;
}

/* get_token_len
 * return the number of tokens.
 */
int get_token_len(char **tokens) {
	int token_len = 0;
	int i;
	for(i=0; tokens[i]; i++) {
		token_len++;
	}
	return token_len;
}
