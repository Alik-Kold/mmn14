/*
 * Project data structures
 */
#ifndef PROJECT_STRUCTS_H
#define PROJECT_STRUCTS_H

struct Numbers_struct{
    int number;
    struct Numbers_struct *next;
};

struct Symbol_table{
    char *symbol;
    int value;
    int base_addr;
    int offset;
    int attribute[LEN_INSTRUCTIONS];
    struct Symbol_table *next;
};

struct Machine_code{
    int position;
    int is_data;
    int val[1 + ARE_BITS_LEN + WORD_BITS]; /* should add up to 20 */
    struct Machine_code *next;
};

struct Macro_list{
    char *name;
    char *lines;
    struct Macro_list *next;
};

#endif
