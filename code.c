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
int get_addr_type(int opcode, int operand_type, int src){
    if (operand_type == NUMBER){
        if (((src) && ( opcode == clr_oc || opcode == jmp_oc ||
                        opcode == red_oc || opcode == prn_oc ||
                        opcode == rts_oc || opcode == stop_oc)) || (opcode != cmp_oc && opcode != prn_oc))
            return -1;
        return IMMEDIATE;
    }
    if (operand_type == LABEL){
        if (src){
            if (opcode != mov_oc && opcode != cmp_oc && opcode != add_oc && opcode != lea_oc) // list of allowed methods
                return -1;
        }
        else{
            if(opcode == rts_oc || opcode == stop_oc)
                return -1;
        }
        return DIRECT;
    }
    if (operand_type == INDEX_REGISTER){
        if (src){
            if (opcode != mov_oc && opcode != cmp_oc && opcode != add_oc && opcode != sub_oc && opcode != lea_oc) // list of allowed methods
                return -1;
        }
        else{
            if(opcode == rts_oc || opcode == stop_oc)
                return -1;
        }
        return INDEXING;
    }
    if (operand_type == REGISTER){
        if (src){
            if (opcode != mov_oc && opcode != cmp_oc && opcode != add_oc) // list of allowed methods
                return -1;
        }
        else{
            if(opcode == jmp_oc || opcode == rts_oc || opcode == stop_oc )
                return -1;
        }
        return REGISTER_DIRECT;

    }
    return -1;
}


/*
 * encode values into binary
 * addressing word - fill in all the values
 * opcode / data / string - fill in value to start, attribute, is_data flag, and zeros for the rest
 * todo: make base_addr and offset into unsigned ints instead of regular ints.
 * The issue right now is that our machine code structure is in regular ints.
 * How do we convert without making it "implementation defined" ?
 */
void encode(struct Machine_code **node, int * counter, int start, int dest_register,int src_addr_type, int src_register,
        int funct, int attribute, int is_data) {
    (*node)->position = *counter;
    (*counter)++;
    (*node)->is_data = is_data;

    dec_to_binary_array(dest_register, &(*node)->val[2]);
    dec_to_binary_array(src_addr_type, &(*node)->val[6]);
    dec_to_binary_array(src_register, &(*node)->val[8]);
    dec_to_binary_array(funct, &(*node)->val[12]);
    dec_to_binary_array(attribute, &(*node)->val[WORD_BITS + 1]);
    dec_to_binary_array(start, (*node)->val);
    if (start < 0) handle_binary_array_of_negative(WORD_BITS, (*node)->val);

    (*node)->next = (struct Machine_code * )malloc(sizeof (struct Machine_code));
    memset((*node)->next, 0, sizeof(struct Machine_code));
    *node = (*node)->next;
}


int unexpected_addressing_type_error(int addr_type) {
    printf("Unexpected addressing type - %d\n", addr_type);
    return 1;
}

void get_operand_params(int *errors, int opcode, int src, char *operand, int *addr_type, int *register_num) {
    (*addr_type) = get_addr_type(opcode, analyze_operand(operand), src);
    switch ((*addr_type)){
        case IMMEDIATE: break;
        case DIRECT: break;
        case REGISTER_DIRECT:
            operand++;
            (*register_num) = atoi(operand);
            break;
        case INDEXING:
            operand = extract_string(operand, "[", "]");
            if (!validate_registers(operand)){
                printf("invalid register for indexing - %s\n"
                       "Allowed registers: r10-r15\n", operand);
                (*errors)++;
            }
            operand++;
            (*register_num) = atoi(operand);
            break;
        default:
            (*errors) += unexpected_addressing_type_error((*addr_type));
            break;
    }
}

void first_pass_encode_addressing(struct Machine_code **node, int *errors, int *IC, int addr_type, const char *operand) {
    switch (addr_type){
        case IMMEDIATE:
            encode(node, IC, atoi(operand), 0, 0, 0, 0, ABSOLUTE_FLAG, 0);
            break;
        case DIRECT:
            encode(node, IC, 0, 0, 0, 0, 0, RELOCATABLE_FLAG, 0);
            encode(node, IC, 0, 0, 0, 0, 0, RELOCATABLE_FLAG, 0);
            break;
        case REGISTER_DIRECT:
            break;
        case INDEXING:
            encode(node, IC, 0, 0, 0, 0, 0, RELOCATABLE_FLAG, 0);
            encode(node, IC, 0, 0, 0, 0, 0, RELOCATABLE_FLAG, 0);
            break;
        default:
            (*errors) += unexpected_addressing_type_error(addr_type);
            break;
    }
}

/*
void prep_string(struct Machine_code **node, int *errors, char *data, int * DC) {
 *  encode(node, IC, dest_addr_type, 0, 0, 0, 0, ABSOLUTE_FLAG, 0);

    }
 */

void prep_command(struct Machine_code **node, struct Symbol_table *symbol_table_head, int *errors, char *line, int * IC) {
    int num_of_operands, opcode, funct, dest_addr_type, dest_register, src_addr_type = 0, src_register = 0;
    char *command_name = get_command_name(line), *dest_operand, *src_operand;

    if (!validate_command_name(command_name)) {
        printf("command name %s not found!\n", command_name);
        errors++;
    }

    line = trim_whitespaces(remove_head(line, command_name));
    if (*line != '\0') num_of_operands = count_occurrences(line, ',') + 1;
    else num_of_operands = 0;


    if (num_of_operands > 2) {
        (*errors) += unexpected_instruction_error(command_name, num_of_operands);
        return;
    }

    if (!num_of_operands){
        if (!(strcmp(command_name, "rts")))
            encode(node, IC, rts_oc, 0, 0, 0, 0, RELOCATABLE_FLAG, 0);
        else if (!(strcmp(command_name, "stop")))
            encode(node, IC, stop_oc, 0, 0, 0, 0, RELOCATABLE_FLAG, 0);
        else (*errors) += unexpected_instruction_error(command_name, num_of_operands);
        return;
    }

    if (num_of_operands == 1){
        dest_operand = trim_whitespaces(get_operand(line));
        if (!(strcmp(command_name, "clr")))      opcode = clr_oc, funct = clr_funct;
        else if (!(strcmp(command_name, "not"))) opcode = not_oc, funct = not_funct;
        else if (!(strcmp(command_name, "inc"))) opcode = inc_oc, funct = inc_funct;
        else if (!(strcmp(command_name, "dec"))) opcode = dec_oc, funct = dec_funct;
        else if (!(strcmp(command_name, "jmp"))) opcode = jmp_oc, funct = jmp_funct;
        else if (!(strcmp(command_name, "bne"))) opcode = bne_oc, funct = bne_funct;
        else if (!(strcmp(command_name, "jsr"))) opcode = jsr_oc, funct = jsr_funct;
        else if (!(strcmp(command_name, "red"))) opcode = red_oc, funct = red_funct;
        else if (!(strcmp(command_name, "prn"))) opcode = prn_oc, funct = prn_funct;
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return;
        }
    }

    if (num_of_operands == 2){
        src_operand = trim_whitespaces(get_operand(line));
        dest_operand = strstr(line, ",");
        dest_operand++; /* ugly hack to get rid of the comma*/
        dest_operand = trim_whitespaces(dest_operand);

        if (!(strcmp(command_name, "mov")))         opcode = mov_oc, funct = mov_funct;
        else if (!(strcmp(command_name, "cmp")))    opcode = cmp_oc, funct = cmp_funct;
        else if (!(strcmp(command_name, "add")))    opcode = add_oc, funct = add_funct;
        else if (!(strcmp(command_name, "sub")))    opcode = sub_oc, funct = sub_funct;
        else if (!(strcmp(command_name, "lea")))    opcode = lea_oc, funct = lea_funct;
        else {
            (*errors) += unexpected_instruction_error(command_name, num_of_operands);
            return;
        }
        get_operand_params(errors, opcode, 1, src_operand, &src_addr_type, &src_register);
    }
    get_operand_params(errors, opcode, 0, dest_operand, &dest_addr_type, &dest_register);

    /* encode opcode and addressing*/
    encode(node, IC, opcode, 0, 0, 0, 0, ABSOLUTE_FLAG, 0);
    encode(node, IC, dest_addr_type, dest_register, src_addr_type, src_register, funct, ABSOLUTE_FLAG, 0);

    if (num_of_operands == 2)
        first_pass_encode_addressing(node, errors, IC, src_addr_type, src_operand);
    first_pass_encode_addressing(node, errors, IC, dest_addr_type, dest_operand);
}
/*
int prep_data(struct Machine_code **node, int *errors, char *line, int DC) {
    int num_of_operands, expected_num, L;

}
 */