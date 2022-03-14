#include "code.h"


/*
 * Encode string to machine code
 */
int encode_string(char* str){
    return 1;
}

int unexpected_instruction_error(char *instruction, int num_of_operands) {
    printf("\nUnexpected instruction %s for number of operands received %d\n", instruction, num_of_operands);
    return 1;
}


int validate_command_name(char *command_name){
    int i;
    for(i = 0; i < LEN_COMMANDS; i++) if (strcmp(command_name,COMMANDS[i]) == 0) return 1;
    return 0;
}


char* get_command_name(char* line){
    char* str = strdup(line);
    return strtok(str, " ");
}


int validate_and_encode_instruction(int *errors, char *line, int IC) {
    int num_of_operands, expected_num, L;
    char *command_name = get_command_name(line);

    if (!validate_command_name(command_name)) {
        printf("command name %s not found!\n", command_name);
        errors++;
    }

    line = trim_whitespaces(remove_head(line, command_name));
    if (*line != '\0') num_of_operands = count_occurrences(line, ',') + 1;
    else num_of_operands = 0;

    expected_num = 0;
    if (num_of_operands == expected_num){
        if (!(strcmp(command_name, "rts")));
        else if (!(strcmp(command_name, "stop")));
        else (*errors) += unexpected_instruction_error(command_name, num_of_operands);
    }

    expected_num = 1;
    if (num_of_operands == expected_num){
        if (!(strcmp(command_name, "clr")));
        else if (!(strcmp(command_name, "not")));
        else if (!(strcmp(command_name, "inc")));
        else if (!(strcmp(command_name, "dec")));
        else if (!(strcmp(command_name, "jmp")));
        else if (!(strcmp(command_name, "bne")));
        else if (!(strcmp(command_name, "jsr")));
        else if (!(strcmp(command_name, "red")));
        else if (!(strcmp(command_name, "prn")));
        else (*errors) += unexpected_instruction_error(command_name, num_of_operands);
    }
    expected_num = 2;
    if (num_of_operands == expected_num){
        if (!(strcmp(command_name, "mov")));
        else if (!(strcmp(command_name, "cmp")));
        else if (!(strcmp(command_name, "add")));
        else if (!(strcmp(command_name, "sub")));
        else if (!(strcmp(command_name, "lea")));
        else (*errors) += unexpected_instruction_error(command_name, num_of_operands);
    }

    if (num_of_operands > expected_num) (*errors) += unexpected_instruction_error(command_name, num_of_operands);

    return L;
}

