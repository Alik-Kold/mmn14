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
int analyze_operand(char* operand){
    size_t str_len;
    int operand_type, i;
    char * str = trim_whitespaces(operand);

    if (str[0] == '#') str++;
    if (str[0] == '+' || str[0] == '-' || isdigit(str[0])){
        while (str[0] != 0) {
            if (!(isdigit(str[0]))) return -1;
            str++;
        }
        return NUMBER;
    }
    if (str[0] == REGISTER_PREFIX_STR[0] && strlen(str) <= 3){ /* REGISTER_PREFIX_STR[0] = 'r' */
        for (i=0; i < NUM_OF_REGISTERS; i++)
            if (!strcmp(str, GLUE_REGISTER(REGISTER_PREFIX_STR, i))) return REGISTER;
        return LABEL;
    }


    return -1;
}

/*
 *return addressing method based on opcode and operand_type and if src operand or dst, if invalid, return -1
 */
int get_addressing_method(int opcode, int operand_type, int src){
    if (operand_type == NUMBER){
        if ((src) && (  opcode == clr_oc ||
                        opcode == jmp_oc ||
                        opcode == red_oc ||
                        opcode == prn_oc ||
                        opcode == rts_oc ||
                        opcode == stop_oc))
            return -1;// list of allowed methods

        else if (       opcode != cmp_oc &&
                        opcode != prn_oc)
            return -1;
        return IMMEDIATE;
    }
    if (operand_type == LABEL){
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
    if (operand_type == INDEX_REGISTER)
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
    if (operand_type == REGISTER)
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
    int num_of_operands, expected_num, L, operand1_type, operand2_type;
    char *command_name = get_command_name(line), *operand1, *operand2;

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
        operand1 = get_operand(line);
        if (!(strcmp(command_name, "clr"))){
            printf("%s\n", operand1);
        }
        else if (!(strcmp(command_name, "not")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }

        else if (!(strcmp(command_name, "inc")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else if (!(strcmp(command_name, "dec")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else if (!(strcmp(command_name, "jmp")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else if (!(strcmp(command_name, "bne")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else if (!(strcmp(command_name, "jsr")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else if (!(strcmp(command_name, "red")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else if (!(strcmp(command_name, "prn")))
        {
            operand1_type = analyze_operand(operand1);
            printf("%s - %d\n", operand1, operand1_type);
        }
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return -1;
        }
        /* todo:
         * get operand1 (remove head or get command or some other implementation)
         * operand1_type = analyze_operand(str)
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
         * get operand1 (remove head or get command or some other implementation)
         * operand1_type = analyze_operand(str)
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