/*
 * common project generic functions
 */
#include "common.h"


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
    int l = strlen(str);

    while(isspace(str[l - 1])) --l;
    while(* str && isspace(* str)) ++str, --l;

    return strndup(str, l);
}

unsigned int count_words(char *str){
    int state = 0;
    unsigned int word_count = 0;

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
            ++word_count;
        }

        // Move to next character
        ++str;
    }

    return word_count;
}

void dec_to_binary(int n, int binaryNum[32]){
    int i = 0;
    while (n > 0) {
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
}

int is_number(char* str){
    int i=0, length;
    if (!str) return 0;
    if ((str[0] == '+') || (str[0] == '-')) str++;
    length = strlen (str);
    for (;i<length; i++) if (!isdigit(str[i])) return 0;
    return 1;
}


void set_file_extention(char* filename,char** dest_pointer,const char* ext){
    int len = strlen(filename) + strlen(ext);
    *dest_pointer = (char*)malloc(len);
    memset(*dest_pointer, 0, len);
    strcpy(*dest_pointer, filename);
    strcat(*dest_pointer, ext);
}


int bin_to_decimal(int binary_number){
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


void dec_to_binary_array(int decimal_number, int result[]){
    int i = 0;
    while (decimal_number){
        result[i++] = decimal_number % 2;
        decimal_number /= 2;
    }
}

/*
 * Get string and delimiter
 * return string after first occurrence of delimiter
 */
char* remove_head(char* str,char* delimiter){
    char* new = malloc(strlen(str) - strlen(delimiter));
    memset(new,0,strlen(str) + strlen(delimiter) + 1);
    strcpy(new,str + strlen(delimiter) + 1);
    return new;
}


/*
 * Return # of occurrences char_ in str
 */
int count_occurrences(char* str, char char_){
    int cnt;

    for (cnt=0; str[cnt]; str[cnt] == char_ ? cnt++ : *str++);
    return cnt;
}


void write_line_to_file(const char *line, FILE *am_file) {
    fprintf(am_file, "\n");
    fwrite(line, strlen(line),1,am_file);
}


char *strndup(const char *s, size_t n) {
    /* strndup actually exists in string.h now,
     * but is defined by the posix 2008 standard,
     * and wasn't around in the ansi c days.
     * Pasting it here for the make warning to go away*/
    char *p;
    size_t n1;

    for (n1 = 0; n1 < n && s[n1] != '\0'; n1++) continue;
    p = malloc(n + 1);
    if (p != NULL) {
        memcpy(p, s, n1);
        p[n1] = '\0';
    }

    return p;
}


int validate_printable_only(char *str) {
    if (!str){
        printf("Provided string is empty or NULL. This won't do.\n");
        return 0;
    }
    while(str[0] != 0) {
        if (!isprint(str[0])) {
            printf("Non-printable character encountered\n"
                   "ASCII - %d\n", str[0]);
            return 0;
        }
        str++;
    }
    return 1;
}


/*
 * Check whether line is longer than LEN_LINE macro
 */
int line_is_too_long(const char *line) {
    size_t len_line = strlen(line);
    if ( len_line > LEN_LINE + 1){
        printf("input line too long\n"
               "line: %s\n"
               "line length (or captured up to): %lu\n"
               "length allowed:                  %d\n", line, len_line, LEN_LINE);
        return 1;
    }
    return 0;
}
