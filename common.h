/*
 * Commonly used generic functions
 */
#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

void set_file_extention(char* filename,char** dest_pointer,const char* ext);
void write_line_to_file(const char *, FILE *);
int bin_to_decimal(int binary_number);
int count_occurrences(char* str, char char_);
int is_number(char* str);
int starts_with(const char *a, const char *b);
int validate_printable_only(char *str);
unsigned int countWords(char *str);
char *trim_whitespaces(char *str);
char *remove_head(char* str,char* delimiter);
char *strndup(const char *, size_t);

#endif //PROJECT_COMMON_H
