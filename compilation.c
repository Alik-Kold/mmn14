#include "compilation.h"


int analyze_operand(char* str){
    str = trim_whitespaces(str);

    if (str[0] == '#') str++;
    if (regcheck_str(str, OPERAND_PATTERN[REGISTER]))       return REGISTER;
    if (regcheck_str(str, OPERAND_PATTERN[LABEL]))          return LABEL;
    if (regcheck_str(str, OPERAND_PATTERN[NUMBER]))         return NUMBER;
    if (regcheck_str(str, OPERAND_PATTERN[INDEX]))          return INDEX;
    if (regcheck_str(str, OPERAND_PATTERN[STRING]))         return STRING;
    return -1;
}


int regcheck_str(char* str, const char* pattern){
    regex_t re_pattern;
    int result;
    printf("%s\n", pattern);
    if (regcomp(&re_pattern, pattern, REG_EXTENDED)){
        printf("Failed to compile regex, please amend and recompile\n");
        exit(1);
    }
    result = !regexec(&re_pattern, str, 0, NULL, 0);
    regfree(&re_pattern);
    return result;
}


void create_output_files(struct Symbols_table *pTable, struct Machine_code *pCode,char* filename) {
    char *ext_file_name, *ent_file_name, *ob_file_name, line[80], group_name = 'A';
    set_file_extention(filename,&ext_file_name,".ext");
    set_file_extention(filename,&ent_file_name,".ent");
    set_file_extention(filename,&ob_file_name,".ob");
    struct Symbols_table *table_point = pTable;

    FILE* ext = fopen(ext_file_name,"w");
    FILE* ent = fopen(ent_file_name,"w");
    FILE* ob = fopen(ob_file_name,"w");
    int number, i =0;
    char ascii_value;
    int DC = 100;

    while(table_point){
        if (table_point->attribute[EXTERNAL]) {
            sprintf(line,"%s BASE %d\n",table_point->symbol,table_point->base_addr);
            fwrite(line, strlen(line),1,ext);
            sprintf(line,"%s OFFSET %d\n\n",table_point->symbol,table_point->offset);
            fwrite(line, strlen(line),1,ext);
        }
        if (table_point->attribute[ENTRY]) {
            sprintf(line,"%s,%d,%d\n",table_point->symbol,table_point->base_addr,table_point->offset);
            fwrite(line, strlen(line),1,ent);
        }
        if (table_point->next == NULL)
            break;
        table_point = table_point->next;
    }

    while(pCode) {
        group_name = 'A';
        for (i = 0; i < 5 ; i++){
            //i - i+4
            number = pCode->val[i * 4] * 1000 + pCode->val[i*4 + 1] * 100 + pCode->val[i*4 + 2] * 10 + pCode->val[i*4 + 3] * 1;
            ascii_value = bin_to_hex(number);
            if      (i == 0)
                sprintf(line,"0%d %c%c-",DC,   group_name, ascii_value);
            else if (i == 4)
                sprintf(line,"%s%c%c\n",  line, group_name, ascii_value);
            else
                sprintf(line,"%s%c%c-", line, group_name, ascii_value);

            group_name ++;
        }
        DC++;
        fwrite(line, strlen(line),1,ob);
        pCode = pCode->next;
    }

    fclose(ext);
    fclose(ent);
    fclose(ob);
}


int validate_label(char *labelname){
    int i;
    for(i = 0; i < LEN_COMMANDS;     i++)   if (strcmp(labelname,COMMANDS[i]) == 0)     return 0;
    for(i = 0; i < LEN_INSTRUCTIONS; i++)   if (strcmp(labelname,INSTRUCTIONS[i]) == 0) return 0;
    return 1;
}


int validate_command_name(char *command_name){
    int i;
    for(i = 0; i < LEN_COMMANDS; i++) if (strcmp(command_name,COMMANDS[i]) == 0) return 1;
    return 0;
}


int add_to_symbols_table(char *label_name, struct Symbols_table *head, int attr_type, int base_addr, int offset ) {
    int errors = 0;
    struct Symbols_table *point, *new_node;
    point = head;
    if (point->symbol != NULL){
        while(point){
            if ((point->symbol != NULL) && (!strcmp(point->symbol, label_name))){
                if (!point->attribute[attr_type]) {
                    printf("symbol already defined %s \n",label_name);
                    errors = 1;
                    break;
                }
            }
            if (point->next == NULL){
                new_node = (struct Symbols_table*)malloc(sizeof (struct Symbols_table));
                memset(new_node, 0, sizeof (struct Symbols_table));
                point->next = new_node;
                break;
            }

            point = point->next;
        }
    }

    point->symbol = (char *)malloc(strlen(label_name));
    strcpy(point->symbol, label_name);
    point->attribute[attr_type] = 1;
    point->base_addr = base_addr;
    point->offset = offset;
    point->value =  base_addr + offset;
    return errors;
}


void update_data_symbols(struct Symbols_table *head,int ICF, int DCF){
    int errors = 0;
    struct Symbols_table *point;
    point = head;
    while(point){
        if(point->attribute[DATA]){
            /*todo: calculate values using offset and base addr*/
            point->offset = 0;
            point->base_addr = 0;
            point->value = point->offset + point->base_addr;
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
    struct numbers_struct *nums = (struct numbers_struct*) malloc(sizeof(struct numbers_struct));
    nums->next = NULL;
    struct numbers_struct *head = nums;

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
        if (isNumber (number_str)){
            nums->number = atoi(number_str) * is_minus;
            if(data_len){
                nums->next = (struct numbers_struct*) malloc(sizeof(struct numbers_struct));
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


char* get_string_value(char* line){
    char* str = remove_head(line,".string");
    str = trim_whitespaces(str);
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


char* get_label_name(char* line){
    char* place = strstr(line, ":");
    int len;
    if (place){
        len = place - line;
        return strndup(line, len);
    }
    return NULL;
}


char* get_command_name(char* line){
    char* str = strdup(line);
    return strtok(str, " ");
}


void compile(char* filename) {
    struct  Machine_code *code_head, *code_pointer;
    int IC = 100, DC = 0, errors = 0, symbol_def = 0, ICF, DCF, *values, line_num = 0, num_of_operands, operand_type;
    char *line = (char*) malloc(80), *token, *label_name, *full_label_name, *command_name, *string_value;
    size_t len;
    struct Symbols_table *head = (struct Symbols_table *) malloc(sizeof (struct Symbols_table));
    memset(head, 0, sizeof (struct Symbols_table));
    struct Symbols_table *point = head;
    FILE *fd = fopen(filename, "r");

    /* 1st pass */
    while (getline(&line, &len, fd) != -1) {
        symbol_def = 0;
        line = trim_whitespaces(line);
        line_num++;
        if(strlen(line) == 0 || line[0] == ';') continue;

        label_name = get_label_name(line);

        if (label_name != NULL) {
            errors = validate_label(label_name);
            symbol_def = 1;
            full_label_name = malloc(strlen(label_name) + 1);
            strcpy(full_label_name, label_name);
            strcat(full_label_name, ":");
            line = remove_head(line,full_label_name);
            line = trim_whitespaces(line);
        }
        if (strstr(line, ".data") != NULL || strstr(line, ".string") != NULL) {
            if (symbol_def) {
                errors = add_to_symbols_table(label_name, head, DATA, 0, 0); //todo: offset and baseaddr?
            }
            if (strstr(line, ".data") != NULL) {
                values = get_data_values(line); //todo: should I increase the DC when there is not symbol definition?
                DC += sizeof(values) / sizeof(int);
            } else {
                string_value = get_string_value(line);
                DC += strlen(string_value);
            }
            /*todo : data image */
            continue;
        } else if (strstr(line, ".extern") != NULL) {
            label_name = remove_head(line,".extern");
            errors = add_to_symbols_table(label_name, head, EXTERNAL, 0, 0);
            continue;
        }
        if (strstr(line, ".entry") != NULL) {
            continue;
        }
        else {
            if (symbol_def) errors = add_to_symbols_table(label_name, head, CODE, 0, 0);
            command_name = line;
            command_name = get_command_name(command_name);
            if (!validate_command_name(command_name)) {
                printf("command name %s not found!\n", command_name);
                errors = 0;
            }
            /* TODO:
             * parse the operation and get the number of operands and size of operation

                IC += L;

            line = remove_head(line,command_name);
            line = trim_whitespaces(line);
            if (line) num_of_operands = count_occurrences(line, ',') + 1;
            token = strtok(line, ",");
            while (token){
                token = trim_whitespaces(token);
                operand_type = analyze_operand(token);
                if (operand_type == -1){
                    printf("Invalid operand - %s", token);
                    return;
                }
                token = strtok(NULL, ",");
            }
             * */
        }

    }

    ICF = IC;
    DCF = DC;
    if (errors) return;
    update_data_symbols(head, ICF, DCF);
    fseek(fd, 0, SEEK_SET);

    /* 2nd pass */
    while (getline(&line, &len, fd) != -1) {
        symbol_def = 0;
        line = trim_whitespaces(line);

        label_name = get_label_name(line);
        if (label_name) continue;

        if (strstr(line, ".data") != NULL || strstr(line, ".string") != NULL) {
            if (symbol_def) errors = add_to_symbols_table(label_name, head, DATA, 0, 0); //todo: offset and baseaddr?
            if (strstr(line, ".data") != NULL) {
                values = get_data_values(line);
                DC += sizeof(values) / sizeof(int);
            } else {
                string_value = get_string_value(line);
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
    }
    if (errors) return;
    create_output_files(head,code_head,filename);
}


