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

void prep_command(struct Machine_code **machine_code_node, struct Symbol_table *symbol_table_head, int *errors, char *line, int * IC);
int analyze_operand(char *);
int validate_command_name(char *command_name);
int get_addressing_method(int opcode, int operand_type, int src);
int unexpected_instruction_error(char *instruction, int num_of_operands);
char* get_command_name(char* line);
void encode(struct Machine_code **node, int * counter, int start, int dest_register,
            int src_addressing_type, int src_register, int funct, int attribute, int is_data);


#endif //PROJECT_CODINGS_H
