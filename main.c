#include <stdio.h>
#include <unistd.h>

#include "precompilation.h"
#include "compilation.h"
#include "common.h"
#include "reserverd_words.h"
/*
 * a file holding reserverd words and command names
 */

const int LEN_COMMANDS = 16;

const char *COMMANDS[] =
        {
                "mov",
                "cmp",
                "add",
                "sub",
                "lea",
                "clr",
                "not",
                "inc",
                "dec",
                "jmp",
                "bne",
                "jsr",
                "red",
                "prn",
                "rts",
                "stop"
        };

const int LEN_INSTRUCTIONS = 4;

const char *INSTRUCTIONS[] =
        {
                ".data",
                ".string",
                ".entry",
                ".extern"
        };

int main(int argc, char *argv[]) {
    int i = 1;
    int result = 0;
    char *am_file_name;

    for (; i < argc; i++)
    {
        //result = extract_macros(argv[i]);
        //if (result == 0)
        //    break;
        set_file_extention(argv[i],&am_file_name,".am");
        compile(am_file_name);
    }

}
