#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
/*
 * common functions might be used everywhere
 */


/*
 * check if string a start with string b
 */
int starts_with(const char *a, const char *b) {
    return (!(strncmp(a, b, strlen(b))));
}


/*
 * trim leading and trailing spaces
 */
char* trim_whitespaces(char *str){
    char *end;

    while(isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    if (end[0] == '\n') end[0] = '\0';
    return str;
}

unsigned int countWords(char *str){
    int state = 0;
    unsigned int wc = 0;  // word count

    // Scan all characters one by one
    while (*str){
        // If next character is a separator, set the
        // state as OUT
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = 0;

            // If next character is not a word separator and
            // state is OUT, then set the state as IN and
            // increment word count
        else if (state == 0){
            state = 1;
            ++wc;
        }

        // Move to next character
        ++str;
    }

    return wc;
}

void decToBinary(int n)
{
    int binaryNum[32];

    int i = 0;
    while (n > 0) {

        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

}

int isNumber(char* str)
{
    int length = strlen (str);
    int i = 0;
    for (i=0;i<length; i++)
    if (!isdigit(str[i]))
    {
        return(0);
    }
    return 1;
}

void set_file_extention(char* filename,char** dest_pointer,const char* ext)
{
    int len = strlen(filename) + strlen(ext);
    *dest_pointer = (char*)malloc(len);
    memset(*dest_pointer, 0, len);
    strcpy(*dest_pointer,filename);
    strcat(*dest_pointer,ext);

}

int bin_to_decimal(int binary_number)
{
    int remainder;
    int i = 0;
    int hexadecimal_number = 0;

    while (binary_number != 0)
    {
        remainder = binary_number % 10;
        hexadecimal_number = hexadecimal_number + remainder * i;
        i = i * 2;
        binary_number = binary_number / 10;
    }
    return (hexadecimal_number);
}


char* remove_head(char* str,char* delimiter)
{
    char* new = malloc(strlen(str) - strlen(delimiter));
    memset(new,0,strlen(str) + strlen(delimiter) + 1);
    strcpy(new,str + strlen(delimiter) + 1);
    return new;
}

int count_occurrences(char* str, char del)
{
    int data_len;

    for (data_len=0; str[data_len]; str[data_len]==del ? data_len++ : *str++);
    return data_len;

}