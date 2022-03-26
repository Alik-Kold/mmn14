/*
 * Project encoding library
 */
#ifndef PROJECT_CODINGS_H
#define PROJECT_CODINGS_H
#include <string.h>
#include "artifacts.h"
#include "common.h"
#include "structs.h"
#include "compilation.h"

void prep_command(struct Machine_code **node, struct Symbol_table *symbol_table_head, int *errors, char *line, int * IC,int second_pass);
void prep_string(struct Machine_code **node, char *data, int *DC);
void prep_data(struct Machine_code **node, int *data, int *DC, int L);
int analyze_operand(char *);
int validate_command_name(char *command_name);
int get_addr_type(int opcode, int operand_type, int src);
int unexpected_instruction_error(char *instruction, int num_of_operands);
void encode(struct Machine_code **node, int * counter, int start, int dest_register,
            int src_addr_type, int src_register, int funct, int attribute, int is_data);
void promote_IC_and_node(struct Machine_code **node,int *IC);

#endif
