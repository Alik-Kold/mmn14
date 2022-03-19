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
    size_t str_len = 1; /* LABEL_MAX_LEN == 32 */
    int register_compare, i;
    char * str = trim_whitespaces(operand);

    if (!operand) return -1;

    if (str[0] == '#') str++;
    if (str[0] == '+' || str[0] == '-' || isdigit(str[0])){
        str++;
        while (str[0] != 0) {
            if (!(isdigit(str[0]))) return -1;
            str++;
        }
        return NUMBER;
    }
    if (str[0] == REGISTER_PREFIX_STR[0] && strlen(str) <= 3){
        str++;
        register_compare = atoi(str);
        for (i=0; i < NUM_OF_REGISTERS; i++) if (register_compare == i) return REGISTER;
        str--;
    }
    if (isalpha(str[0])){
        str++;
        while(str[0] != 0){
            if (str_len > LABEL_MAX_LEN - 1){
                printf("Label %s too long!\n"
                       "Label length - %lu\n"
                       "Allowed - %d", operand, strlen(operand), LABEL_MAX_LEN);
                return -1;
            }
            if (str[0] == '['){
                switch (analyze_operand(get_str_upto(++str, "]"))) {
                    case NUMBER:    return INDEX_NUMBER;
                    case LABEL:     return INDEX_LABEL;
                    case REGISTER:  return INDEX_REGISTER;
                    default:        return -1;
                }
            }
            if (isalnum(str[0])) str++, str_len++;
            else{
                printf("Invalid label character - %c\n"
                       "Labels comprise only alphanumerics\n", str[0]);
                return -1;
            }
        }
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
    *node = (*node)->next;
    return 1;
}


void encode_register_line(struct Machine_code **node, int counter, int dest_addressing_type, int dest_register,
                         int src_addressing_type, int src_register, int funct, int attribute) {
    (*node)->position = counter;
    (*node)->is_data = 0;
    dec_to_binary_array(dest_addressing_type,   (*node)->val);
    dec_to_binary_array(dest_register,          &(*node)->val[2]);
    dec_to_binary_array(src_addressing_type,    &(*node)->val[6]);
    dec_to_binary_array(src_register,           &(*node)->val[8]);
    dec_to_binary_array(funct,                  &(*node)->val[12]);
    dec_to_binary_array(attribute,              &(*node)->val[WORD_BITS + 1]);
    (*node)->next = (struct Machine_code * )malloc(sizeof (struct Machine_code));
    memset((*node)->next, 0, sizeof(struct Machine_code));
    *node = (*node)->next;
}

/*
 * todo: make base_addr and offset into unsigned ints instead of regular ints.
 * The issue right now is that our machine code structure is in regular ints.
 * How do we convert without making it "implementation defined" ?
 */
void encode_base_addr_and_offset(struct Machine_code **node, int counter, int base_addr, int offset, int attribute) {
    (*node)->position = counter;
    (*node)->is_data = 0;

    /* encode base_addr */
    dec_to_binary_array(base_addr, (*node)->val);
    dec_to_binary_array(attribute, &(*node)->val[WORD_BITS + 1]);
    (*node)->next = (struct Machine_code * )malloc(sizeof (struct Machine_code));
    memset((*node)->next, 0, sizeof(struct Machine_code));
    *node = (*node)->next;

    /* encode offset*/
    dec_to_binary_array(offset,   (*node)->val);
    dec_to_binary_array(attribute,&(*node)->val[WORD_BITS + 1]);
    (*node)->next = (struct Machine_code * )malloc(sizeof (struct Machine_code));
    memset((*node)->next, 0, sizeof(struct Machine_code));
    *node = (*node)->next;
}

int prep_command(struct Machine_code **machine_code_node, struct Symbol_table *symbol_table_head, int *errors, char *line, int IC) {
    int num_of_operands, expected_num, L = 0, operand1_type, operand2_type;
    int dest_addressing_type, dest_register, src_addressing_type, src_register;
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
        if (!(strcmp(command_name, "rts")))         return encode_opcode(machine_code_node, IC, rts_oc, RELOCATABLE_FLAG);
        else if (!(strcmp(command_name, "stop")))   return encode_opcode(machine_code_node, IC, stop_oc, RELOCATABLE_FLAG);
        else (*errors) += unexpected_instruction_error(command_name, num_of_operands);
    }

    expected_num = 1;
    if (num_of_operands == expected_num){
        operand1 = trim_whitespaces(get_operand(line));

        if (!(strcmp(command_name, "clr")));
        else if (!(strcmp(command_name, "not")));
        else if (!(strcmp(command_name, "inc"))){
            dest_addressing_type = get_addressing_method(inc_oc, analyze_operand(command_name), 0);
            switch (dest_addressing_type){
                case DIRECT:
                    encode_base_addr_and_offset(machine_code_node, IC, 0, 0, 0);
                    L += 2;
                    IC += 2;
                    break;
                case REGISTER_DIRECT:
                    operand1++;
                    dest_register = atoi(operand1);
                    src_register = 0;
                    src_addressing_type = 0;
                    encode_register_line(machine_code_node, IC, dest_addressing_type, dest_register, src_addressing_type,
                                              src_register, inc_funct, RELOCATABLE_FLAG);
                    L++;
                    IC++;
                    break;

                case INDEXING:
                    encode_register_line(machine_code_node, IC, dest_addressing_type, dest_register, src_addressing_type,
                                              src_register, inc_funct, RELOCATABLE_FLAG);
                    L++;
                    IC++;
                    encode_base_addr_and_offset(machine_code_node, IC, 0, 0, 0);
                    L += 2;
                    IC += 2;
                    break;
            }

        }
        else if (!(strcmp(command_name, "dec")));
        else if (!(strcmp(command_name, "jmp")));
        else if (!(strcmp(command_name, "bne")));
        else if (!(strcmp(command_name, "jsr")));
        else if (!(strcmp(command_name, "red")));
        else if (!(strcmp(command_name, "prn")));
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return -1;
        }
        /* todo:
         * get operand1 (remove symbol_table_head or get command or some other implementation)
         * operand1_type = analyze_operand(str)
         * todo: (alik) determine addressing according to cmd + op_type
         * todo: (vadim) once we have addressing assignment - implement encode_addressing()
         * */
    }
    expected_num = 2;
    if (num_of_operands == expected_num){
        operand1 = trim_whitespaces(get_operand(line));
        operand2 = trim_whitespaces(strstr(line, ","));
        operand2++;
        operand1_type = analyze_operand(operand1);
        operand2_type = analyze_operand(operand2);
        if (!(strcmp(command_name, "mov")));
        else if (!(strcmp(command_name, "cmp")));
        else if (!(strcmp(command_name, "add_funct")));
        else if (!(strcmp(command_name, "sub")));
        else if (!(strcmp(command_name, "lea")));
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return -1;
        }
        /* todo:
         * get operand1 (remove symbol_table_head or get command or some other implementation)
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