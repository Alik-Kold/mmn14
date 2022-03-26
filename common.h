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
#include "artifacts.h"

void dec_to_binary_array(int decimal_number, int result[]);
void handle_binary_array_of_negative(int len, int result[]);
void set_file_extention(char* filename,char** dest_pointer,const char* ext);
void reverse_array(int arr[], int size);
void write_line_to_file(const char *, FILE *);
int bin_to_decimal(int binary_number);
int count_occurrences(char* str, char char_);
int is_number(char* str);
int line_is_too_long(const char *line);
int starts_with(const char *a, const char *b);
int validate_printable_only(char *str);
unsigned int count_words(char *str);
char *extract_string(char* line, char* delim_start, char* delim_end);
char *get_str_upto(char* line, char* delim);
char *get_word(char *line, char *delim);
char *trim_whitespaces(char *str);
char *remove_head(char* str,char* delimiter);
char *strndup(const char *, size_t);
char bin_to_hex(int binary_number);


#endif //PROJECT_COMMON_H
