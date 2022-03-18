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

char* get_operand(char* line){
    char* str = strdup(line);
    return strtok(str, ",");
}


/*
 * receive operand string str
 * return operand type enum
 */
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

/*
 *return addressing method based on opcode and optype and if src operand or dst, if invalid, return -1
 */
int get_addressing_method(int opcode, int optype,int src)
{
    if (optype == NUMBER)
    {
        if (src)
        {
            if (opcode == clr_oc || opcode == jmp_oc ||
                opcode == red_oc || opcode == prn_oc || opcode == rts_oc || opcode == stop_oc) // list of allowed methods
                return -1;
        }
        else
        {
            if (opcode != cmp_oc && opcode != prn_oc)
                return -1;
        }
        return IMMEDIATE;
    }
    if (optype == LABEL)
    {
        if (src)
        {
            if (opcode != mov_oc && opcode != cmp_oc && opcode != add_oc && opcode != lea_oc) // list of allowed methods
                return -1;
        }
        else
        {
            if(opcode == rts_oc || opcode == stop_oc)
                return -1;
        }
        return DIRECT;
    }
    if (optype == INDEX_REGISTER)
    {
        if (src)
        {
            if (opcode != mov_oc && opcode != cmp_oc && opcode != add_oc && opcode != sub_oc) // list of allowed methods
                return -1;
        }
        else
        {
            if(opcode == jmp_oc || opcode == rts_oc || opcode == stop_oc)
                return -1;
        }
        return INDEXING;
    }
    if (optype == REGISTER)
    {
        if (src)
        {
            if (opcode != mov_oc && opcode != cmp_oc && opcode == add_oc) // list of allowed methods
                return -1;

        }
        else
        {
            if(opcode == jmp_oc || opcode == rts_oc || opcode == stop_oc )
                return -1;
        }
        return REGISTER_DIRECT;

    }
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


int encode_opcode(struct Machine_code **node, int counter, int opcode, int attribute) {
    (*node)->position = counter;
    (*node)->is_data = 0;
    dec_to_binary_array(opcode, (*node)->val);
    dec_to_binary_array(attribute, &(*node)->val[WORD_BITS + 1]);
    (*node)->next = (struct Machine_code * )malloc(sizeof (struct Machine_code));
    memset((*node)->next, 0, sizeof(struct Machine_code));
    node = (struct Machine_code **) (*node)->next; /* todo: need to pass this one instead of 1 */
    return 1;
}

int prep_command(struct Machine_code **node, int *errors, char *line, int IC) {
    int num_of_operands, expected_num, L, operand_type;
    char *command_name = get_command_name(line), *operand;

    if (!validate_command_name(command_name)) {
        printf("command name %s not found!\n", command_name);
        errors++;
    }

    line = trim_whitespaces(remove_head(line, command_name));
    if (*line != '\0') num_of_operands = count_occurrences(line, ',') + 1;
    else num_of_operands = 0;

    expected_num = 0;
    if (num_of_operands == expected_num){
        if (!(strcmp(command_name, "rts")))         return encode_opcode(node, IC, rts_oc, RELOCATABLE_FLAG);
        else if (!(strcmp(command_name, "stop")))   return encode_opcode(node, IC, stop_oc, RELOCATABLE_FLAG);
        else (*errors) += unexpected_instruction_error(command_name, num_of_operands);
    }

    expected_num = 1;
    if (num_of_operands == expected_num){
        if (!(strcmp(command_name, "clr"))){
            operand = get_operand(line);
            printf("%s\n", operand);
        }
        else if (!(strcmp(command_name, "not")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }

        else if (!(strcmp(command_name, "inc")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else if (!(strcmp(command_name, "dec")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else if (!(strcmp(command_name, "jmp")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else if (!(strcmp(command_name, "bne")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else if (!(strcmp(command_name, "jsr")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else if (!(strcmp(command_name, "red")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else if (!(strcmp(command_name, "prn")))
        {
            operand = get_operand(line);
            operand_type = analyze_operand(operand);
            printf("%s - %d\n", operand, operand_type);
        }
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return -1;
        }
        /* todo:
         * get operand (remove head or get command or some other implementation)
         * operand_type = analyze_operand(str)
         * todo: (alik) determine addressing according to cmd + op_type
         * todo: (vadim) once we have addressing assignment - implement encode_addressing()
         * */
    }
    expected_num = 2;
    if (num_of_operands == expected_num){
        if (!(strcmp(command_name, "mov")));
        else if (!(strcmp(command_name, "cmp")));
        else if (!(strcmp(command_name, "add")));
        else if (!(strcmp(command_name, "sub")));
        else if (!(strcmp(command_name, "lea")));
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return -1;
        }
        /* todo:
         * get operand (remove head or get command or some other implementation)
         * operand_type = analyze_operand(str)
         * todo: (alik) determine addressing according to cmd + op_type for second cmd type (2 operands)
         * todo: (vadim) once we have addressing assignment - implement encode_addressing()
         * */

    }

    if (num_of_operands > expected_num) (*errors) += unexpected_instruction_error(command_name, num_of_operands);

    return L;
}

int prep_data(struct Machine_code **node, int *errors, char *line, int DC) {
    int num_of_operands, expected_num, L;

}