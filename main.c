#include <stdio.h>
#include "common.h"
#include "compilation.h"
#include "precompilation.h"


int main(int argc, char *argv[]) {
    int i = 1;
    for (; i < argc; i++){
        if (!extract_macros(argv[i])){
            printf("Precompilation error in file\n"
                   "Skipping this one\n");
            continue;
        }
        compile(argv[i]);
    }
    printf("\ndone\n");
    return EXIT_SUCCESS;
}
