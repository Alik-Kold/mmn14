#include "compilation.h"

/*
 * Validate label isn't a duplicate or a reserved word
 * @param labelname - the current str to validate
 * @return 1 if bad, 0 otherwise
 */
int validate_label(char *labelname){
    int i;
    for(i = 0; i < LEN_COMMANDS;     i++)   if (strcmp(labelname,COMMANDS[i]) == 0)     return 1;
    for(i = 0; i < LEN_INSTRUCTIONS; i++)   if (strcmp(labelname,INSTRUCTIONS[i]) == 0) return 1;
    return 0;
}


int add_to_symbol_table(char *label_name, struct Symbol_table *head, int attr_type, int base_addr, int offset) {
    int errors = 0;
    struct Symbol_table *point, *new_node;
    point = head;
    if (point->symbol){
        while(point){
            if (!point->symbol && (!strcmp(point->symbol, label_name))){
                if (!point->attribute[attr_type]) {
                    printf("symbol already defined %s \n",label_name);
                    errors = 1;
                    break;
                }
            }
            if (point->next == NULL){
                new_node = (struct Symbol_table*)malloc(sizeof (struct Symbol_table));
                memset(new_node, 0, sizeof (struct Symbol_table));

                point = point->next = new_node;
                break;
            }

            point = point->next;
        }
    }

    point->symbol = (char *)malloc(strlen(label_name));
    point->symbol= strdup(label_name);
    point->attribute[attr_type] = 1;
    point->base_addr = base_addr;
    point->offset = offset;
    point->value =  base_addr + offset;
    return errors;
}


void update_data_symbols_positions(struct Symbol_table *head, int ICF){
    struct Symbol_table *point;
    point = head;
    while(point){
        if (point->attribute[DATA]){
            point->value += ICF;
            point->offset = point->value % WORD_BITS;
            point->base_addr = point->value - point->offset;
        }

        if (point->next == NULL) break;
        point = point->next;
    }
}


int update_symbol_table_attribute(struct Symbol_table *head, char *symbol, int attribute, int collision){
    struct Symbol_table *point;
    point = head;
    while (point) {
        if (!strcmp(point->symbol, symbol)){
            if (point->attribute[attribute]){
                printf("WARNING: duplicate attributes %s for symbol %s\n", ATTRIBUTES[attribute], symbol);
                return 0;
            }
            if (point->attribute[collision]){
                printf("ERROR: attempted to colliding attribute in symbol %s\n"
                       "%s - %s\n", point->symbol, ATTRIBUTES[collision], ATTRIBUTES[attribute]);
                return 1;
            }
            point->attribute[attribute] = 1;
            return 0;
        }

        if (point->next == NULL){
            printf("ERROR: symbol %s not found\n", symbol);
            return 1;
        }
        point = point->next;
    }
    printf("ERROR: no symbols found in table\n");
    return 1;
}



/* get the values of the .data label.
 * input: entire line
 * output int* of the values*/
int *get_data_values(char* line){
    char* number_str = remove_head(line, ".data");
    int* numbers;
    struct Numbers_struct *head;
    int is_minus, i = 0, list_len = 0, data_len;
    struct Numbers_struct *nums = (struct Numbers_struct*) malloc(sizeof(struct Numbers_struct));
    nums->next = NULL;

    head = nums;
    number_str = trim_whitespaces(number_str);
    data_len = count_occurrences(number_str, ',');
    if (data_len){
        number_str = strtok(number_str, ",");
        number_str = trim_whitespaces(number_str);
    }
    while(number_str){
        is_minus = 1;
        if (number_str[0] == '-'){
            number_str++;
            is_minus = -1;
        }
        if (is_number(number_str)){
            nums->number = atoi(number_str) * is_minus;
            if(data_len){
                nums->next = (struct Numbers_struct*) malloc(sizeof(struct Numbers_struct));
                nums = nums->next;
                data_len --;
            }
            list_len ++;
        }
        else {
            printf("entered value is not a number! %s\n", number_str);
            return 0;
        }
        number_str = strtok(NULL, ",");
        if (number_str) number_str = trim_whitespaces(number_str);
    }
    nums = head;
    numbers = malloc(sizeof (int) * list_len);
    while(nums != NULL){
        numbers[i] = nums->number;
        nums = nums->next;
        i++;
    }
    return (numbers);
}


/*
 * Validate register is between r10 and r15
 * */
int validate_registers(char* register_name){
    int num;
    if (register_name[0] != REGISTER_PREFIX){
        printf("index not register\n");
        return 0;
    }
    register_name++;
    if (!is_number(register_name)){
        printf("invalid register number type %s", register_name);
        return 0;
    }
    num = atoi(register_name);
    return ((num >= 10) && (num <= 15));
}


void compile(char* filename) {
    struct Machine_code *code_head, *data_head, *data_node, *code_node;
    struct Symbol_table *head,*symbol_node;
    struct Symbol_table *symbol_table_external_head;

    FILE *fd;
    char *line, *label_name, *string_value, *read_line, *am_filename;
    size_t len;
    int IC, DC, L, errors, symbol_def, ICF, DCF, offset, *values;

    /*
     * inits and memsets
     */
    code_head = (struct Machine_code *) malloc(sizeof (struct Machine_code));
    data_head = (struct Machine_code *) malloc(sizeof (struct Machine_code));
    data_node = data_head;
    memset(code_head, 0, sizeof (struct Machine_code));
    memset(data_head, 0, sizeof (struct Machine_code));
    code_node = code_head;
    IC = IC_INIT, DC = 0, errors = 0;
    read_line = NULL;

    head = (struct Symbol_table *) malloc(sizeof (struct Symbol_table));
    symbol_table_external_head = (struct Symbol_table *) malloc(sizeof (struct Symbol_table));

    memset(head, 0, sizeof (struct Symbol_table));
    memset(symbol_table_external_head, 0, sizeof (struct Symbol_table));

    set_file_extention(filename,&am_filename,".am");
    fd = fopen(am_filename, "r");

    /* 1st pass */
    while (getline(&read_line, &len, fd) != -1) {
        symbol_def = 0;
        if (line_is_too_long(read_line)) {
            errors++;
            continue;
        }
        line = trim_whitespaces(read_line);
        if(strlen(line) < 2 || line[0] == ';') continue;

        /*
         * Check whether first word is a label
         * Yes -    turn on symbol definition flag
         *          save teh label
         *          remove the label from line
         * No  -    move on
         */
        label_name = get_str_upto(line, ":");
        if (label_name) {
            errors += validate_label(label_name);
            line = trim_whitespaces(line);
            line += strlen(label_name) + 1;
            line = trim_whitespaces(line);
            symbol_def = 1;
        }

        /*
         * Handle entry/extern input
         */
        if (starts_with(line, ".entry")) continue;
        if (starts_with(line, ".extern")) {
            label_name = remove_head(line,".extern");
            errors += add_to_symbol_table(label_name, head, EXTERNAL, 0, 0);
            continue;
        }

        /*
         * Handle data input
         */
        if (starts_with(line, ".data") || starts_with(line, ".string")) {
            if (symbol_def) errors += add_to_symbol_table(label_name, head, DATA, DC, 0);

            if (starts_with(line, ".string")){
                string_value = extract_string(line, "\"", "\"");
                if (!validate_printable_only(string_value)){
                    errors++;
                    continue;
                }
                prep_string(&data_node, string_value, &DC);
            }
            else if (strstr(line, ".data")) {
                line = trim_whitespaces(line);

                if (!line) continue;

                L = count_occurrences(line, ',') + 1;
                values = get_data_values(line);
                prep_data(&data_node, values, &DC, L);
            }
            continue;
        }

        /*
         * Handle command input
         */
        else {
            offset = IC % WORD_BITS;
            if (symbol_def) errors += add_to_symbol_table(label_name, head, CODE, IC - offset, offset);
            prep_command(&code_node, head,0, &errors, line, &IC,0);

        }

        if (IC + DC > IC_MAX - IC_INIT){
            printf("Instruction set exceeds max allowed length of %d\n"
                   "Exiting\n", IC_MAX - IC_INIT);
            return;
        }
        read_line = NULL;
    }

    ICF = IC;
    DCF = DC;
    if (errors) {
        printf("%d errors found, stopping after 1st pass\n", errors);
        return;
    }

    update_data_symbols_positions(head, ICF);

    fseek(fd, 0, SEEK_SET);

    IC = IC_INIT;
    code_node = code_head;
    data_node = data_head;

    /* 2nd pass */
    read_line = NULL;
    while (getline(&read_line, &len, fd) != -1) {
        line = trim_whitespaces(read_line);
        if(strlen(line) < 2 || line[0] == ';') continue;

        label_name = get_str_upto(line, ":");
        if (label_name){
            line = trim_whitespaces(line);
            line += strlen(label_name) + 1;
            line = trim_whitespaces(line);
        }

        if (starts_with(line, ".data") || starts_with(line, ".string") || starts_with(line, ".extern"))
            continue;
        if (starts_with(line, ".entry")) {
            label_name = trim_whitespaces(remove_head(line,".entry"));
            errors += update_symbol_table_attribute(head, label_name, ENTRY, EXTERNAL);
            continue;
        }

        prep_command(&code_node, head, symbol_table_external_head,&errors, line, &IC,1);
        read_line = NULL;

    }

    code_node = code_head;
    data_node = data_head;
    while (data_node->next){
        data_node->position += ICF;
        data_node = data_node->next;
    }
    data_node = data_head;
    while (data_node->next && data_node->next->position) data_node = data_node->next;
    data_node->next = NULL;
    while (code_node->next && code_node->next->position) code_node = code_node->next;
    code_node->next = data_head;

    if(symbol_table_external_head->symbol)
    {
        symbol_node = head;
        while(symbol_node->next && symbol_node->next->symbol) symbol_node = symbol_node->next;
        symbol_node->next = symbol_table_external_head;
    }

    print_symbol_table(head);
    print_pattern(2, "\n");
    printf("printf instruction machine code:\n");
    print_machine_code(code_head);

    if (errors) return;
    create_output_files(head,code_head,filename,ICF,DCF);
}
