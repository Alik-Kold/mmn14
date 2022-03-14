/*
 * Project data structures
 */
#ifndef PROJECT_STRUCTS_H
#define PROJECT_STRUCTS_H

struct numbers_struct{
    int number;
    struct numbers_struct* next;
};

struct Symbols_table{
    char* symbol;
    int value;
    int base_addr;
    int offset;
    int attribute[4];
    struct Symbols_table* next;
};

struct Machine_code{
    int val[20];
    struct Machine_code* next;
};

struct Macro_list{
    char* name;
    char *lines;
    struct Macro_list* next;
};

#endif //PROJECT_STRUCTS_H
