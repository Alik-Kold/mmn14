#include "compilation.h"


void create_output_files(struct Symbol_table *pTable, struct Machine_code *pCode, char* filename) {
    char *ext_file_name, *ent_file_name, *ob_file_name, line[80], group_name = 'A';
    set_file_extention(filename,&ext_file_name,".ext");
    set_file_extention(filename,&ent_file_name,".ent");
    set_file_extention(filename,&ob_file_name,".ob");
    struct Symbol_table *table_point = pTable;

    FILE* ext = fopen(ext_file_name,"w");
    FILE* ent = fopen(ent_file_name,"w");
    FILE* ob = fopen(ob_file_name,"w");
    int number, i =0;

    while(table_point){
        if (table_point->attribute[EXTERNAL]) {
            sprintf(line,"%s BASE %d",table_point->symbol,table_point->base_addr);
            fwrite(line, strlen(line),1,ext);
            sprintf(line,"%s OFFSET %d",table_point->symbol,table_point->offset);
            fwrite(line, strlen(line),1,ext);
        }
        if (table_point->attribute[ENTRY]) {
            sprintf(line,"%s,%d,%d",table_point->symbol,table_point->base_addr,table_point->offset);
            fwrite(line, strlen(line),1,ent);
        }
        if (table_point->next == NULL) break;
        table_point = table_point->next;
    }

    while(pCode) {
        group_name = 'A';
        for (; i < 6 ; i++){
            //i - i+4
            number = pCode->val[i] * 1000 + pCode->val[i + 1] * 100 + pCode->val[i + 2] * 10 + pCode->val[i + 3] * 1;
            number = bin_to_decimal(number);
            if      (i == 0)    sprintf(line,"%c%d-",   group_name, number);
            else if (i == 5)    sprintf(line,"%s%c%d",  line, group_name, number);
            else                sprintf(line,"%s%c%d-", line, group_name, number);

            group_name++;
        }
        fwrite(line, strlen(line),1,ob);
        pCode = pCode->next;
    }

    fclose(ext);
    fclose(ent);
    fclose(ob);
}

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
    int errors = 0;
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


/* get the values of the .data label.
 * input: entire line
 * output int* of the values*/
int *get_data_values(char* line){
    char* number_str = remove_head(line, ".data");
    int* numbers;
    int is_minus = 1, i = 0, list_len = 0, data_len = 0;
    struct Numbers_struct *nums = (struct Numbers_struct*) malloc(sizeof(struct Numbers_struct));
    nums->next = NULL;
    struct Numbers_struct *head = nums;

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


char* extract_string(char* line){
    char* str = remove_head(line,".string");
    size_t len;
    str = trim_whitespaces(str); // the bug is here
    str++; //todo: vadim! why is it here?
    len = strlen(str);
    str[len - 1] = '\0';
    return str;
}


/*check if register is between r10 and r15*/
int validate_registers(char* register_name){
    char register_num[2];
    int num;
    strcpy(register_num, register_name + 1);
    num = atoi(register_name);
    return ((num >= 10) && (num <= 15));
}


char* get_str_upto(char* line, char* delim){
    char* place = strstr(line, delim);
    int len;
    if (place){
        len = place - line;
        return strndup(line, len);
    }
    return NULL;
}


void compile(char* filename) {
    struct  Machine_code *code_head = (struct Machine_code *) malloc(sizeof (struct Machine_code));
    memset(code_head, 0, sizeof (struct Machine_code));
    struct Machine_code *code_node = code_head;
    int IC = IC_INIT, DC = 0, L, errors = 0, symbol_def = 0, ICF, DCF, offset, arr_len;
    int *values;
    char *line = NULL;  //= (char*) malloc(LINE_MAX_LEN + 1),
    char *label_name, *full_label_name, *string_value;
    size_t len;
    struct Symbol_table *head = (struct Symbol_table *) malloc(sizeof (struct Symbol_table));
    memset(head, 0, sizeof (struct Symbol_table));
    struct Symbol_table *point = head;


    FILE *fd = fopen(filename, "r");

    /* 1st pass */
    while (getline(&line, &len, fd) != -1) {
        symbol_def = 0;
        if (line_is_too_long(line)) {
            errors++;
            continue;
        }
        line = trim_whitespaces(line);
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
            symbol_def = 1;
            errors += validate_label(label_name);
            full_label_name = malloc(strlen(label_name) + 2);
            strcpy(full_label_name, label_name);
            strcat(full_label_name, ":");
            line = remove_head(line,full_label_name);
            line = trim_whitespaces(line);
        }

        /*
         * Handle entry/extern input
         */
        if (strstr(line, ".entry")) continue;
        if (strstr(line, ".extern")) {
            label_name = remove_head(line,".extern");
            errors += add_to_symbol_table(label_name, head, EXTERNAL, 0, 0);
            continue;
        }

        /*
         * Handle data input
         */
        if (strstr(line, ".data") || strstr(line, ".string")) {
            if (strstr(line, ".string")){
                string_value = extract_string(line);
                if (!validate_printable_only(string_value)){
                    errors++;
                    continue;
                }
                L = strlen(string_value);
                DC += L;

            }
            else if (strstr(line, ".data")) {
                L = count_occurrences(line, ',') + 1;
                values = get_data_values(line);
                DC += L;
            }
            if (symbol_def) {
                /*printf("%s\n", label_name);*/
                errors += add_to_symbol_table(label_name, head, DATA, DC, 0);
                DC++;
            }
            /*todo : data image
             * DC += L;
             */
            continue;
        }

        /*
         * Handle command input
         */
        else {
            offset = IC % WORD_BITS;
            if (symbol_def) errors += add_to_symbol_table(label_name, head, CODE, IC - offset, offset);

            /* TODO:
             * parse the operation and get the number of operands and size of operation
             * */
            IC += prep_command(&code_node, head, &errors, line, IC);

        }
        line = NULL;
    }
    ICF = IC;
    DCF = DC;
    if (errors) {
        printf("%d errors found, stopping after 1st pass\n", errors);
        return;
    }

    print_symbol_table(head);
    printf("\n\n");
    print_machine_code(code_head);

    update_data_symbols_positions(head, ICF);
    fseek(fd, 0, SEEK_SET);

    /* 2nd pass */
    line = NULL;
    while (getline(&line, &len, fd) != -1) {
        symbol_def = 0;
        line = trim_whitespaces(line);

        label_name = get_str_upto(line, ":");
        if (label_name) continue;

        if (strstr(line, ".data") || strstr(line, ".string")) {
            if (symbol_def) errors += add_to_symbol_table(label_name, head, DATA, 0, 0); //todo: offset and baseaddr?
            if (strstr(line, ".data") != NULL) {
                arr_len = count_occurrences(line, ',') + 1;
                values = get_data_values(line);
                DC += arr_len;
            } else {
                string_value = extract_string(line);
                DC += strlen(string_value);
            }
            /*todo : data image */
            continue;
        } else if (strstr(line, ".extern") != NULL) {
            continue;
        }
        if (strstr(line, ".entry") != NULL) {
            /*todo: something in the symbols table*/
            continue;
        }
        /*
         * todo: finish encoding (??????????/)
         * */
        line = NULL;

    }
    if (errors) return;
    create_output_files(head,code_head,filename);
}


