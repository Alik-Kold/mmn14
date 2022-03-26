/*
 * module used for extracting the macros from the file
 */

#include "precompilation.h"


/*
 * Extract macros
 * @param filename - path to a file, without suffix
 * @return 1 if extraction succeeded, 0 otherwise
 */
int extract_macros(char *filename){
    struct Macro_list *macros_head, *macro_pointer;
    char *macro_name, *line_read, *as_file_name, *am_file_name, *line = NULL;
    size_t len = 0;
    int read, line_len, is_macro = 0;
    FILE *as_file, *am_file;

    macros_head = (struct Macro_list*)malloc(sizeof (struct Macro_list));
    macro_pointer = macros_head;
    set_file_extention(filename,&as_file_name,".as");
    set_file_extention(filename,&am_file_name,".am");

    line_read = (char*)malloc(LINE_MAX_LEN);

    as_file = fopen(as_file_name, "r");
    if (!as_file){
        printf("Couldn't open %s\n"
               "Please make sure file exists and has read permissions\n", as_file_name);
        return 0;
    }
    am_file = fopen(am_file_name,"w");

    /* extract macros from file and add to linked list
     * if the line is not a macro do nothing
     * if the line is a macro: save it to the linked list
     */
    while ((read = getline(&line_read, &len, as_file)) != -1) {
        line = trim_whitespaces(line_read);
        if(strlen(line) < 1)
            continue;
        line_len = 0;
        if (starts_with(line, "macro")){
            while (macro_pointer->next) macro_pointer = macro_pointer->next;
            macro_name = remove_head(line,"macro");
            macro_name = trim_whitespaces(macro_name);
            if (validate_label(macro_name)){
                printf("Error: %s is a saved word unable to create macro with the same name",macro_name);
                return 0;
            }
            macro_pointer->name = (char*) malloc(strlen(macro_name) + 1);
            strcpy(macro_pointer->name, macro_name);
            is_macro = 1;
            continue;
        }
        if (is_macro){
            do{
                line = trim_whitespaces(line);
                if (!starts_with(line, "endm")){
                    if (line_len == 0){
                        macro_pointer->lines = (char*)malloc(strlen(line) + 1);
                        line_len += strlen(line) +1;
                        memset(macro_pointer->lines,0,line_len);
                    }
                    else{
                        line_len += strlen(line) +1;
                        macro_pointer->lines = realloc(macro_pointer->lines, line_len +1);
                        strcat(macro_pointer->lines,"\n");
                    }
                    strcat(macro_pointer->lines,line);
                }
                getline(&line_read, &len, as_file);
                line = trim_whitespaces(line_read);
                if(strlen(line) < 1)
                    continue;
            } while (strcmp(line, "endm") != 0 && read != -1);
            is_macro = 0;
        }
    }

    fseek(as_file, 0, SEEK_SET);

    /*
     * create the am file using the macros saved in the linked list
     * if the line is not macro save it to the second file
     * if the line is macro search for the macro in the linked list and write it to the second file
     */
    while ((read = getline(&line_read, &len, as_file)) != -1) {
        line = trim_whitespaces(line_read);
        if (strlen(line) < 1) continue;
        if (starts_with(line, "macro")){
            while(!starts_with(line, "endm")){
                getline(&line_read, &len, as_file);
                line = trim_whitespaces(line_read);
                if(strlen(line) < 1) continue;
            }
            continue;
        }
        if (count_words(line) == 1){
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
