#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "common.h"
/*
 * common functions might be used everywhere
 */

int StartsWith(const char *a, const char *b)
/*
 * check if string start with another string
 */
{
    if(strncmp(a, b, strlen(b)) == 0) return 1;
    return 0;
}

char* clean_empty_space(char *str)
/*
 * trim leading and traling spaces
 */
{
    char *end;

    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

unsigned int countWords(char *str)
{
    int state = 0;
    unsigned int wc = 0;  // word count

    // Scan all characters one by one
    while (*str)
    {
        // If next character is a separator, set the
        // state as OUT
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = 0;

            // If next character is not a word separator and
            // state is OUT, then set the state as IN and
            // increment word count
        else if (state == 0)
        {
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
    strcpy(new,str + strlen(delimiter));
    return new;
}
