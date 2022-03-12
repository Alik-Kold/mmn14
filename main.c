#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "compilation.h"
#include "precompilation.h"


int main(int argc, char *argv[]) {
    int i = 1;
    char *am_file_name;

    for (; i < argc; i++){
        if (!extract_macros(argv[i])){
            printf("Precompilation error in file\n"
                   "Skipping this one\n");
            continue;
        }
        set_file_extention(argv[i],&am_file_name,".am");
        compile(am_file_name);
    }
    printf("\ndone\n");
}
