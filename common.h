/*
 * Commonly used generic functions
 */
#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "artifacts.h"

void dec_to_binary_array(int decimal_number, int result[]);
void handle_binary_array_of_negative(int len, int result[]);
void print_pattern(unsigned int i, char * str);
void print_breaker();
void print_str_with_padding(size_t max_pad, const char *str);
void reverse_array(int arr[], int size);
void set_file_extention(char* filename,char** dest_pointer,const char* ext);
void write_line_to_file(const char *, FILE *);
int count_occurrences(char* str, char char_);
int is_number(char* str);
int line_is_too_long(const char *line);
int starts_with(const char *big, const char *little);
int validate_printable_only(char *str);
unsigned int count_words(char *str);
char bin_to_hex(int binary_number);
char *extract_string(char* line, char* delim_start, char* delim_end);
char *get_str_upto(char* line, char* delim);
char *get_word(char *line, char *delim);
char *remove_head(char* str,char* delimiter);
char *strndup(const char *, size_t);
char *trim_whitespaces(char *str);

#endif
