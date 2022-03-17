/*
 * Project encoding library
 */
#ifndef PROJECT_CODINGS_H
#define PROJECT_CODINGS_H
#include "artifacts.h"
#include "common.h"
#include "structs.h"

int analyze_operand(char *);
int encode_string(char* str);
int validate_and_encode_command(int *errors, char *line, int IC);
int validate_command_name(char *command_name);
int regcheck_str(char*, const char*);
int unexpected_instruction_error(char *instruction, int num_of_operands);
char* get_command_name(char* line);

#endif //PROJECT_CODINGS_H
