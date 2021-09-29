#ifndef TEXTSEARCH_FNS_H
#define TEXTSEARCH_FNS_H

#define MAXLINE 511

// Reads a line from the given file and writes into the given buffer
//
// @param in File to read from
// @param buf String buffer to write to
// @return 1 if successful 0 if not
int read_line(FILE *in, char *buf);

// Add a line from string buffer to the end of the given file
//
// @param out File to write the line to
// @param buf Buffer containing line to write
void print_line(FILE *out, const char *buf);

// Count the # of occurences of a given string in a given line
//
// @param line Line to scan for occurences
// @param str String to find occurences of
// @return # of occurences of str in line
unsigned count_occurrences(const char *line, const char *str);

// Get the length of a given string
//
// @param s String to get the length of
// @return length of s
unsigned find_string_length(const char *s);

// Check if a given string starts with a given character
//
// @param s String to check the start of
// @param pfx Character to look for at the start of s
// @return 1 if s starts with pfx, 0 otherwise
int starts_with(const char *s, const char *pfx);

// Find if two strings are equal
//
// @param s1 First string to compare
// @param s2 Second string to compare
// @return 1 if s1 is equal to s2, 0 otherwise
int strings_equal(const char *s1, const char *s2);

// Get and open the file named on the command line
//
// @param argv Command line arguments
// @param cArg Will be 1 if the -c flag was used, 0 otherwise
// @return Pointer to the file named on the command line
FILE *get_file(char **argv, int cArg);

// Get the target string named on the command line
//
// @param argv Command line arguments
// @param cArg Will be 1 if the -c flag was used, 0 otherwise
// @return Target string named on the command line
char *get_target_str(char **argv, int cArg);

// Scan the text of a file for the target string and print the lines it
// appears on. If ran with the -c flag, will print the number of occurrences
// of the target string.
//
// @param in File to scan text of
// @param targetStr String to find occurrences of
// @param cArg Will be 1 if the -c flag was used, 0 otherwise
void c_textsearch(FILE *in, char *targetStr, int cArg);

// TODO: declare helper functions here (to be implemented in
// c_textsearch_fns.c and asm_textsearch_fns.S)

#endif // TEXTSEARCH_FNS_H
