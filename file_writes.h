#ifndef MMN14_FILE_WRITES_H
#define MMN14_FILE_WRITES_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "artifacts.h"
#include "code.h"
#include "common.h"
#include "structs.h"
#include "test_funcs.h"

void create_output_files(struct Symbol_table *pTable, struct Machine_code *pCode,char* filename,int ICF, int DCF);


#endif //MMN14_FILE_WRITES_H
