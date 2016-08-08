/* tokenizer.h

		header file for tokenizer.c

		Dillon Vicente, August 2016
*/

#ifndef TOKENIZER_DEF
#define TOKENIZER_DEF 1

/* tokenize 
 *break up the line into individual tokens, based on delimiters
 * returns a null-terminated array of strings
 */
char **tokenize(char *line, char *delimiters);

/* token_count
 * returns a count of the number of strings in the null-terminated
 * array (excluding the null element)
 */
int token_count(char **tokens);

/* compare_char
 * compares the delimiters to a single char.
 * returns 1 if the delimiter matches given char; otherwise 0.
 */
int compare_char(char *delims, char c);

/* getword_s
 * returns the size of a word, given a starting and end pointer.
 */
size_t getword_s(char *start, char *end);

/* begin_p
 * returns the beginning pointer of a word in the given line.
 */
char *begin_p(char *line, size_t size, char *delims);

/* stop_p
 * returns the pointer relative to the starting pointer, stopping
 * before the next token.
 */
char *stop_p(char *start_p, char *line, size_t size, char *delims);

/* no_delims
 * checks to see if there are any delimiters in the given line.
 * Returns 1 if a delimiter exists, 0 otherwise.
 */
int no_delims(char *line, size_t size, char *delims);

/* make_word
 * makes a word from given pointers start and end.
 */
char *make_word(char *start, char *end);

/* shift_b
 * shifts the beginning pointer to the next token.
 */
char *shift_b(char *start, char *end, char *delims);

/* shift_s
 * shifts the stopping pointer to the nex token, relative to the
 * the newly shifted beginning pointer.
 */
char *shift_s(char *start, char *end, char *delims);

/* token_len
 * returns the number of words from input. Used to initialize the
 * size of the token array.
 */
size_t token_len(char *line, size_t size, char *delims);

/* get_token_len
 * returns the number of tokens.
 */
int get_token_len(char **tokens);

#endif
