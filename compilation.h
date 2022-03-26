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
#include "test_funcs.h"

void compile(char* filename);
void create_output_files(struct Symbol_table *pTable, struct Machine_code *pCode,char* filename,int ICF, int DCF);
void update_data_symbols_positions(struct Symbol_table *head, int ICF);
int add_to_symbol_table(char *label_name, struct Symbol_table *head, int attr_type, int  base_addr, int  offset );
int validate_command_name(char *command_name);
int validate_label(char *labelname);
int validate_registers(char* register_name);
int update_symbol_table_attribute(struct Symbol_table *head, char *symbol, int attribute, int collision);
int *get_data_values(char* line);
char *get_word(char *line, char *delim);

#endif