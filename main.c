#include <stdio.h>
#include <unistd.h>

#include "precompilation.h"
//#include "compilation.c"
#include "common.h"

int main(int argc, char *argv[]) {
    int i = 1;
    int result = 0;
    char *am_file_name;

    for (; i < argc; i++)
    {
        result = extract_macros(argv[i]);
        if (result == 0)
            break;
        //set_file_extention(argv[i],am_file_name,".am");
        //compile(am_file_name);
    }

}
