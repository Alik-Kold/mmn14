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

int analyze_operand(char *);
int encode_string(char* str);
int prep_command(struct Machine_code **machine_code_node, struct Symbol_table *symbol_table_head, int *errors, char *line, int IC);
int validate_command_name(char *command_name);
int regcheck_str(char*, const char*);
int get_addressing_method(int opcode, int operand_type, int src);
int unexpected_instruction_error(char *instruction, int num_of_operands);
char* get_command_name(char* line);

#endif //PROJECT_CODINGS_H
