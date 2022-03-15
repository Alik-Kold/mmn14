/*
 * Bigass compilation file, including 1st and 2nd passes
 */
#ifndef PROJECT_COMPILATION_H
#define PROJECT_COMPILATION_H
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "artifacts.h"
#include "code.h"
#include "common.h"
#include "structs.h"

void compile(char* filename);
void create_output_files(struct Symbols_table *pTable, struct Machine_code *pCode,char* filename);
void update_data_symbols_positions(struct Symbols_table *head, int ICF);
int analyze_operand(char *); //todo
int add_to_symbols_table(char *label_name, struct Symbols_table *head,int attr_type,int  base_addr,int  offset );
int regcheck_str(char*, const char*);
int validate_command_name(char *command_name);
int validate_label(char *labelname);
int validate_registers(char* register_name);
int *get_data_values(char* line);
char* get_command_name(char* line);
char* get_label_name(char* line);
char* extract_string(char* line);

#endif //PROJECT_COMPILATION_H
