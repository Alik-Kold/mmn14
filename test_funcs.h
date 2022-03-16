/*
 * Test functions module for dev
 */

#ifndef PROJECT_TEST_FUNCS_H
#define PROJECT_TEST_FUNCS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "artifacts.h"
#include "common.h"
#include "structs.h"

extern const char *__progname;

void print_symbol_table(struct Symbol_table *head);
void printout_struct(void* invar, char* structname);


#endif //PROJECT_TEST_FUNCS_H
