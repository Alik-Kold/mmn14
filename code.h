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

void prep_command(struct Machine_code **node, struct Symbol_table *symbol_table_head, struct Symbol_table *symbol_table_external_head,
        int *errors, char *line, int * IC,int second_pass);
void prep_string(struct Machine_code **node, char *data, int *DC);
void prep_data(struct Machine_code **node, int *data, int *DC, int L);
int validate_command_name(char *command_name);
int unexpected_instruction_error(char *command_name, int num_of_operands);

#endif
