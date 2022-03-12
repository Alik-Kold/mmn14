#ifndef PROJECT_PRECOMPILATION_H
#define PROJECT_PRECOMPILATION_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "artifacts.h"
#include "common.h"
#include "structs.h"


int validate_macro_name(char* labelname);
int extract_macros(char * filename);


#endif //PROJECT_PRECOMPILATION_H
