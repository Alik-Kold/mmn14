/*
 * maman14 Assembler project
 * Course number    20465
 * Semester         Fall 2022a
 * Collaborators:
 *                  - Alik Koldobsky
 *                  - Vadim Genin
 * Project instructions viewable at
 * https://sheilta.apps.openu.ac.il/pls/myopr/booklet.viewbook?p_semester=2022a&p_kurs=20465
 *
 * Project flowchart made with love, viewable at lucidchart (link below)
 * It requires opening a (free) account with them
 * https://lucid.app/lucidchart/fc51b603-1fc5-4623-9fc7-b2d83f24cffe/edit?invitationId=inv_7638c6cb-08bf-452e-965a-a5eb5dcb287f
 * we strongly encourage that you visit the page and drop us a comment :-D
 * If you're not clicking links or opening accounts, a screenshot will be provided in the project zip file
 */

#include <stdio.h>
#include "common.h"
#include "compilation.h"
#include "precompilation.h"

/*
 * Entry point for Assembler flow
 * @param argc - number of filepaths to process
 * @param argv - array of strings of the filepaths
 * @return run success
 */
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
    return EXIT_SUCCESS;
}
