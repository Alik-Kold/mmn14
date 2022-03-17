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


int analyze_operand(char* str){
    size_t str_len;
    int operand_type;
    char* index;
    str = trim_whitespaces(str);

    if (str[0] == '#') str++;
    if (regcheck_str(str, OPERAND_PATTERN[REGISTER]))           return REGISTER;
    if (regcheck_str(str, OPERAND_PATTERN[LABEL]))              return LABEL;
    if (regcheck_str(str, OPERAND_PATTERN[NUMBER]))             return NUMBER;
    if (regcheck_str(str, OPERAND_PATTERN[INDEX])){
        /*
        while (str) if ((str++)[0] == '[') break;
        str_len = strlen(str) - 1;
        index = trim_whitespaces(strndup(str, str_len));
        operand_type = analyze_operand(index);
        switch (operand_type){
            case REGISTER:  return INDEX_REGISTER;
            case LABEL:     return INDEX_LABEL;
            case NUMBER:    return INDEX_NUMBER;
            default:        return -1;
        }*/
        return INDEX;
    }
    if (regcheck_str(str, OPERAND_PATTERN[STRING]))             return STRING;

    return -1;
}


int regcheck_str(char* str, const char* pattern){
    regex_t re_pattern;
    int result;
    if (regcomp(&re_pattern, pattern, REG_EXTENDED)){
        printf("Failed to compile regex, please amend and recompile\n");
        exit(1);
    }
    result = !regexec(&re_pattern, str, 0, NULL, 0);
    regfree(&re_pattern);
    return result;
}



int validate_and_encode_command(struct Machine_code *node, int *errors, char *line, int IC) {
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
        else if (!(strcmp(command_name, "stop"))){
            node->position = IC;
            node->is_data = 0;
            dec_to_binary_array(stop_oc, node->val);
            node->val[18] = 1;
            node = node->next;
            return 1;
        }
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

