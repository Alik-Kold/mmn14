/*
 * Test functions module for dev testing
 */

#ifndef PROJECT_TEST_FUNCS_H
#define PROJECT_TEST_FUNCS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "artifacts.h"
#include "common.h"
#include "structs.h"

void print_machine_code(struct Machine_code *head);
void print_symbol_table(struct Symbol_table *head);

#endif
