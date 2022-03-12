/*
 * module used for extracting the macros from the file
 */

<<<<<<< HEAD
#include "precompilation.h"


int validate_macro_name(char* labelname){
    int i;
    for (i = 0; i < LEN_COMMANDS; i++)      if(!strcmp(COMMANDS[i],labelname))     return 0;
    for (i = 0; i < LEN_INSTRUCTIONS; i++)  if(!strcmp(INSTRUCTIONS[i],labelname)) return 0;
=======
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "reserverd_words.h"
#include "common.c"

struct Macro_list
{
    char* name;
    char *lines;
    struct Macro_list* next;
};

int validate_macro_name(char* labelname)
{
    int i;
    for (i = 0; i < LEN_COMMANDS; i++)
    {
        if(strcmp(COMMANDS[i],labelname) == 0)
            return 0;
    }
    for (i = 0; i < LEN_INSTRUCTIONS; i++)
    {
        if(strcmp(INSTRUCTIONS[i],labelname) == 0)
            return 0;
    }
>>>>>>> origin
    return 1;
}


<<<<<<< HEAD
int extract_macros(char * filename){
    struct Macro_list *macros_head;
    macros_head = (struct Macro_list*)malloc(sizeof (struct Macro_list));
    struct Macro_list *macro_pointer = macros_head;
    char *as_file_name, *am_file_name, *line = NULL;
    size_t len = 0;
    int read, line_len, is_macro = 0;
    char* macro_name;

    set_file_extention(filename,&as_file_name,".as");
    set_file_extention(filename,&am_file_name,".am");

    FILE* as_file = fopen(as_file_name, "r");
    if (as_file == NULL){
        printf("Couldn't open %s\n"
               "Please make sure file exists and has read permissions\n", as_file_name);
=======
int extract_macros(char * filename)
{
    struct Macro_list *macros_head;
    struct Macro_list *macro_pointer;
    char *asm_file_name = (char*)malloc(strlen(filename) + 4);
    char *am_file_name = (char*)malloc(strlen(filename) + 3);
    char * line = NULL;
    int len = 0;
    int read = 0 ;
    char* macro_name;
    int is_macro = 0;
    memset(asm_file_name, NULL, strlen(filename) + 4);

    memset(am_file_name, NULL, strlen(filename) + 3);

    strcat(asm_file_name,".asm");
    strcat(am_file_name,".am");

    FILE* asm_file = fopen(asm_file_name,"r");
    if (asm_file == NULL)
    {
        printf("file %s does not exist!",filename);
>>>>>>> origin
        return 0;
    }
    FILE* am_file = fopen(am_file_name,"w");

    /*extract macros from file and add to linked list */
<<<<<<< HEAD
    while ((read = getline(&line, &len, as_file)) != -1) {
=======
    while ((read = getline(&line, &len, asm_file)) != -1) {
>>>>>>> origin
        /*
         * if the line is not a macro do nothing
         * if the line is a macro: save it to the linked list
         */
<<<<<<< HEAD
        line = trim_whitespaces(line);
        line_len = 0;
        if (starts_with(line, "macro")){
            while (macro_pointer->next) macro_pointer = macro_pointer->next;
            macro_name = remove_head(line,"macro");
            macro_name = trim_whitespaces(macro_name);
            if (!validate_macro_name(macro_name)){
                printf("Error: %s is a saved word unable to create macro with the same name",macro_name);
                return 0;
            }
            macro_pointer->name = (char*) malloc(strlen(macro_name));
            strcpy(macro_pointer->name, macro_name);
            is_macro = 1;
            continue;
        }
        if (is_macro){
            do{
                line = trim_whitespaces(line);
                if (!starts_with(line, "endm")){
                    if (line_len == 0){
                        macro_pointer->lines = (char*)malloc(strlen(line));
                        line_len += strlen(line);
                        memset(macro_pointer->lines,0,line_len);
                    }
                    else{
                        line_len += strlen(line);
                        macro_pointer->lines = realloc(macro_pointer->lines, line_len);
                        strcat(macro_pointer->lines,"\n");
                    }
                    strcat(macro_pointer->lines,line);
                }
                getline(&line, &len, as_file);
                line = trim_whitespaces(line);
            } while (strcmp(line, "endm") != 0 && read != -1);
=======
        line = clean_empty_space(line);
        if (StartsWith(line,"macro"))
        {
            while (macro_pointer->next)
            {
                macro_pointer = macro_pointer->next;
            }
            macro_name = strtok(line,"macro");
            macro_name = clean_empty_space(macro_name);
            if (!validate_macro_name(macro_name))
            {
                printf("error: %s is a saved word unable to create macro with the same name");
                return 0;
            }
            macro_pointer->name = (char*) malloc(strlen(macro_name));
            strcpy(macro_pointer->name,macro_name);
            is_macro = 1;
            continue;
        }
        if(is_macro)
        {
            while (strcmp(line,"endm") != 0 && read != -1)
            {

                getline(&line, &len, asm_file);
                line = clean_empty_space(line);
                if (!StartsWith(line,"endm"))
                {
                    macro_pointer->lines = realloc(macro_pointer->lines, strlen(macro_pointer->lines) + strlen(line));
                    strcat(macro_pointer->lines,"\n");
                    strcat(macro_pointer->lines,line);
                }
            }
>>>>>>> origin
            is_macro = 0;
        }
    }

<<<<<<< HEAD
    fseek(as_file, 0, SEEK_SET);
    /*create the am file using the macros saved in the linked list */
    while ((read = getline(&line, &len, as_file)) != -1) {
        /*
         * if the line is not macro save it to the second file
         * if the line is macro search for the macro in the linked list and write it to the second file
         */
        line = trim_whitespaces(line);
        if (starts_with(line, "macro")){
            while(!starts_with(line, "endm")){
                getline(&line, &len, as_file);
                line = trim_whitespaces(line);
            }
            continue;
        }
        if (countWords(line) == 1){
            if(starts_with(line, "rts") || starts_with(line, "stop")) write_line_to_file(line, am_file);
            else{
                macro_pointer = macros_head;
                while(strcmp(macro_pointer->name,line) != 0) macro_pointer = macro_pointer->next;
                write_line_to_file(macro_pointer->lines, am_file);
            }
        }
        else write_line_to_file(line, am_file);
    }
    fclose(am_file);
    fclose(as_file);
    return 1;
}
=======
    fseek(asm_file,0,SEEK_SET);
    /*create the am file using the macros saved in the linked list */
    while ((read = getline(&line, &len, asm_file)) != -1) {
        /*
         * if the line is not macro save it to the second file
         * if the line is macro search for the macro in the linked list and print it to the second file
         */
        line = clean_empty_space(line);
        if (countWords(line) == 1)
        {
            if (StartsWith(line,"macro"))
            {
                while(!StartsWith(line,"endm"))
                {
                    getline(&line, &len, asm_file);
                    line = clean_empty_space(line);
                }
            }
            else if(StartsWith(line,"rts") || StartsWith(line,"stop"))
            {
                fwrite(line, line,1,am_file);
            }
            else
            {
                macro_pointer = macros_head;
                while(strcmp(macro_pointer->name,line) != 0)
                {
                    macro_pointer = macro_pointer->next;
                }
                fwrite(macro_pointer->lines, strlen(macro_pointer->lines),1,am_file);
            }
        }
        else
        {
            fwrite(line, line,1,am_file);
        }


    }
    return 1;
}
>>>>>>> origin
