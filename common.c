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
    char* default_val = "";
    int l = strlen(str);

    while(l >= 1 && isspace(str[l - 1])) --l;
    while(* str && isspace(* str)) ++str, --l;
    if(l < 0)
        return default_val;
    return strndup(str, l);
}

unsigned int count_words(char *str){
    int state = 0;
    unsigned int word_count = 0;

    /* Scan all characters one by one */
    while (*str){

        /* // If next character is a separator, set the
        // state as OUT */
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = 0;

        /*
        // If next character is not a word separator and
        // state is OUT, then set the state as IN and
        // increment word count */
        else if (state == 0){
            state = 1;
            ++word_count;
        }

        /* Move to next character */
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
    int len = strlen(filename) + strlen(ext) + 1;
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
    while (decimal_number && i <= WORD_BITS){
        result[i++] = decimal_number % 2;
        decimal_number /= 2;
    }
}


void reverse_array(int arr[], int size){
    int i, temp;
    for (i = 0; i < size/2; i++){
        temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}


/*
 * Using 2's complement for negatives
 * and removing minus sign
 */
void handle_binary_array_of_negative(int len, int result[]){
    int i;
    for (i = 0; i < len; i++) if (result[i]) break;
    result[i] = 1;                          /* clearing negative */
    for (i++; i < len; i++)
        if (result[i] < 0) result[i] = 0;   /* clearing negative */
        else result[i] ^= 1;
}

/*
 * Get string and delimiter
 * return string after first occurrence of delimiter
 */
char* remove_head(char* str,char* delimiter){
    int str_len;
    char* new = "";

    str_len = strlen(str) - strlen(delimiter) + 1;
    if(str_len <= 1)
        return new;
    new = malloc(str_len);
    memset(new,0,strlen(str) - strlen(delimiter) + 1);

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


/* strndup actually exists in string.h now,
 * but is defined by the posix 2008 standard,
 * and wasn't around in the ansi c days.
 * Pasting it here for the make warning to go away*/
char *strndup(const char *s, size_t n) {
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
 * Check whether line is longer than LINE_MAX_LEN macro
 */
int line_is_too_long(const char *line) {
    size_t len_line = strlen(line);
    if (len_line > LINE_MAX_LEN + 1){
        printf("input line too long\n"
               "line: %s\n"
               "line length (or captured up to): %lu\n"
               "length allowed:                  %d\n", line, len_line, LINE_MAX_LEN);
        return 1;
    }
    return 0;
}

char* get_str_upto(char* line, char* delim){
    size_t len = strcspn(line, delim);
    if (len && len < strlen(line)) return strndup(line, len);
    return NULL;
}


char* extract_string(char* str, char* delim_start, char* delim_end){
    char *start = strstr(str, delim_start);
    if (start) return get_str_upto(++start, delim_end);
    return NULL;
}

char bin_to_hex(int binary_number){
    int num = binary_number;
    int dec_value = 0;

    int base = 1;

    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    if (dec_value < 10) return (dec_value + 48);
    else                return (dec_value + 87);
}


char *get_word(char *line, char *delim) {
    char* str = strdup(line);
    return strtok(str, delim);
}

