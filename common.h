#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int starts_with(const char *a, const char *b);
char* trim_whitespaces(char *str);
unsigned int countWords(char *str);
void decToBinary(int n);
int isNumber(char* str);
void set_file_extention(char* filename,char** dest_pointer,const char* ext);
int bin_to_decimal(int binary_number);
char *remove_head(char* str,char* delimiter);
int count_occurrences(char* str, char char_);
void write_line_to_file(const char *, FILE *);

#endif //PROJECT_COMMON_H
