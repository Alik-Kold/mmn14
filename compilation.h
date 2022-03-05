//
// Created by user on 3/5/22.
//

#ifndef PROJECT_COMPILATION_H
#define PROJECT_COMPILATION_H



struct numbers_struct
{
    int number;
    struct numbers_struct* next;
};

struct Symbols_table
{
    char* symbol;
    int value;
    int base_addr;
    int offset;
    int attribute[4];
    struct Symbols_table* next;
};

struct Machine_code
{
    int val[20];
    struct Machine_code* next;
};

void analyze_operand_structure(); //todo
void create_output_files(struct Symbols_table *pTable, struct Machine_code *pCode,char* filename);
int validate_label(char *labelname);
int validate_command_name(char *command_name);
int add_to_symbols_table(char *label_name, struct Symbols_table *head,int attr_type,int  base_addr,int  offset );
void update_data_symbols(struct Symbols_table *head,int ICF, int DCF);
int *get_data_values(char* line);
char* get_string_value(char* line);
int validate_registers(char* register_name);
char* get_label_name(char* line);
void compile(char* filename);
char* get_command_name(char* line);

#endif //PROJECT_COMPILATION_H
