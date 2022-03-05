//
// Created by user on 3/5/22.
//

#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H


int StartsWith(const char *a, const char *b);
char* clean_empty_space(char *str);
unsigned int countWords(char *str);
void decToBinary(int n);
int isNumber(char* str);
void set_file_extention(char* filename,char** dest_pointer,const char* ext);
int bin_to_decimal(int binary_number);
char *remove_head(char* str,char* delimiter);


#endif //PROJECT_COMMON_H
