//
// Created by user on 2/26/22.
//

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "common.h"
#include "reserverd_words.h"
#include <stdio.h>
#include "compilation.h"


enum opcdes
{
    mov_oc = 0,
    cmp_oc = 1,
    add_oc = 2,
    sub_oc = 2,
    lea_oc = 4,
    clr_oc = 5,
    not_oc = 5,
    inc_oc = 5,
    dec_oc = 5,
    jmp_oc = 9,
    bne_oc = 9,
    jsr_oc = 9,
    red_oc= 12,
    prn_oc= 13,
    rts_oc= 14,
    stop_oc = 15
};

enum funct
{
    add = 10,
    sub = 11,
    clr = 10,
    not = 11,
    inc = 12,
    dec = 13,
    jmp = 10,
    bne = 11,
    jsr = 12,
};

enum attributes
{
    EXTERNAL = 0,
    CODE = 1,
    ENTRY = 2,
    DATA = 3,
};






void create_output_files(struct Symbols_table *pTable, struct Machine_code *pCode,char* filename) {
    char *ext_file_name;
    char *ent_file_name;
    char *ob_file_name;
    set_file_extention(filename,&ext_file_name,".ext");
    set_file_extention(filename,&ent_file_name,".ent");
    set_file_extention(filename,&ob_file_name,".ob");
    struct Symbols_table *table_point = pTable;

    FILE* ext = fopen(ext_file_name,"w");
    FILE* ent = fopen(ent_file_name,"w");
    FILE* ob = fopen(ob_file_name,"w");
    char line[80];
    int i =0;
    int number = 0;

    char group_name = 'A';
    while(table_point)
    {
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
        if (table_point->next == NULL)
            break;
    table_point = table_point->next;
    }

    while(pCode) {
        group_name = 'A';
        for (; i < 6 ; i++)
        {
            //i - i+4
            number = pCode->val[i] * 1000 + pCode->val[i + 1] * 100 + pCode->val[i + 2] * 10 + pCode->val[i + 3] * 1;
            number = bin_to_decimal(number);
            if(i != 5 && i !=0)
                sprintf(line,"%s%c%d-",line,group_name,number);
            else if(i == 5)
                sprintf(line,"%s%c%d",line,group_name,number);
            else if(i == 0)
                sprintf(line,"%c%d-",group_name,number);
            group_name ++;
        }
        fwrite(line, strlen(line),1,ob);
        pCode = pCode->next;
    }

    fclose(ext);
    fclose(ent);
    fclose(ob);

    }
int validate_label(char *labelname)
{
    int i = 0;
    for(i = 0; i < LEN_COMMANDS; i++)
    {
        if (strcmp(labelname,COMMANDS[i]) == 0)
            return 0;
    }
    for(i = 0; i < LEN_INSTRUCTIONS; i++)
    {
        if (strcmp(labelname,INSTRUCTIONS[i]) == 0)
            return 0;
    }
    return 1;
}
int validate_command_name(char *command_name)
{
    int i = 0;
    for(i = 0; i < LEN_COMMANDS; i++)
    {
        if (strcmp(command_name,COMMANDS[i]) == 0)
            return 1;
    }
    return 0;
}

int add_to_symbols_table(char *label_name, struct Symbols_table *head,int attr_type,int  base_addr,int  offset ) {
    int errors = 0;
    struct Symbols_table *point;
    struct Symbols_table *new_node;
    point = head;
    if (point->symbol != NULL)
    {
        while(point)
        {
            if (strcmp(point->symbol,label_name) == 0)
            {
                if (!point->attribute[attr_type]) {
                    printf("symbol already defined %s \n",label_name);
                    errors = 1;
                    break;
                }
            }
            if (point->next == NULL)
            {
                new_node = (struct Symbols_table*)malloc(sizeof (struct Symbols_table));
                memset(new_node,0,sizeof (struct Symbols_table));
                point->next = new_node;

            }

            point = point->next;
        }
    }

    point->symbol = (char *)malloc(strlen(label_name));
    strcpy(point->symbol,label_name);
    point->attribute[attr_type] = 1;
    point->base_addr = base_addr;
    point->offset = offset;
    point->value =  base_addr + offset;
    return errors;
}

void update_data_symbols(struct Symbols_table *head,int ICF, int DCF)
{
    int errors = 0;
    struct Symbols_table *point;
    point = head;
    while(point)
    {

        if(point->attribute[DATA])
        {

            /*todo: calculate values using offset and base addr*/
            point->offset = 0;
            point->base_addr = 0;
            point->value = point->offset + point->base_addr;

        }

        if (point->next == NULL)
            break;
        point = point->next;

    }
}


int *get_data_values(char* line)
/*get the values of the .data label.
 * input: entire line
 * output int* of the values*/
{
    char* number_str = remove_head(line, ".data");
    number_str = clean_empty_space(number_str);
    int* numbers;
    struct numbers_struct *nums = (struct numbers_struct*) malloc(sizeof(struct numbers_struct));
    nums->next = NULL;
    struct numbers_struct *head = nums;
    int is_minus = 1;
    int i = 0;
    int list_len = 0;
    int data_len = 0;

    data_len = count_occurneces(number_str,',');
    if (data_len)
    {
        number_str = strtok(number_str, ",");
        number_str = clean_empty_space(number_str);

    }
    while(number_str != NULL)
    {
        is_minus = 1;
        if (number_str[0] == '-')
        {
            number_str++;
            is_minus = -1;
        }
        if (isNumber (number_str))
        {
            nums->number = atoi(number_str) * is_minus;
            if(data_len)
            {
                nums->next = (struct numbers_struct*) malloc(sizeof(struct numbers_struct));
                nums = nums->next;
                data_len --;
            }
            list_len ++;
        }
        else
        {
            printf("entered value is not a number! %s",number_str);
            return 0;
        }
        number_str = strtok(NULL, ",");
        if (number_str)
            number_str = clean_empty_space(number_str);
    }
    nums = head;
    numbers = malloc(sizeof (int) * list_len);
    while(nums != NULL)
    {
        numbers[i] = nums->number;
        nums = nums->next;
        i++;
    }
    return (numbers);
}

char* get_string_value(char* line)
{
    char* str = remove_head(line,".string");
    str = clean_empty_space(str);
    return str;

}


int validate_registers(char* register_name)
/*check if register is beween r10 and r15*/
{
    char register_num[2];
    int num;
    strcpy(register_num,register_name + 1);
    num = atoi(register_name);
    if (num < 10 || num > 15)
        return 0;
    return 1;

}


char* get_label_name(char* line)
{
    char* token;
    char* str = (char*)malloc(strlen(line));
    strcpy(str,line);
    if (strstr(line,":"))
    {
        token = strtok(str, ":");
        return token;

    }
    return NULL;
}
char* get_command_name(char* line)
{
    char* token;
    char* str = (char*)malloc(strlen(line));
    strcpy(str,line);
    token = strtok(str, " ");
    return token;
}
void compile(char* filename) {
    struct  Machine_code *code_head;
    struct  Machine_code *code_pointer;
    int IC = 100;
    int DC = 0;
    int ICF;
    int DCF;
    int read;
    char *line = (char*) malloc(80);
    size_t len;
    int errors = 0;
    char *label_name;
    char* full_label_name;
    int symbol_def = 0;
    struct Symbols_table *head = (struct Symbols_table *) malloc(sizeof (struct Symbols_table));
    memset(head,0,sizeof (struct Symbols_table));
    struct Symbols_table *point = head;
    int *values;
    char *string_value;
    char *command_name;
    FILE *f = fopen(filename, "r");
    while ((read = getline(&line, &len, f)) != -1) {
        symbol_def = 0;
        line = clean_empty_space(line);
        if(strlen(line) == 0 || line[0] == ';')
            continue;
        label_name = get_label_name(line);
        if (label_name != NULL) {
            errors = validate_label(label_name);
            symbol_def = 1;
            full_label_name = malloc(strlen(label_name) + 1);
            strcpy(full_label_name,label_name);
            strcat(full_label_name,":");
            line = remove_head(line,full_label_name);
        }
        if (strstr(line, ".data") != NULL || strstr(line, ".string") != NULL) {
            if (symbol_def) {
                errors = add_to_symbols_table(label_name, head, DATA, 0, 0); //todo: offset and baseaddr?
            }
            if (strstr(line, ".data") != NULL) {
                values = get_data_values(line); //todo: should I increase the DC in a case in which there is not symbol defenition?
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
            if (symbol_def) {
                errors = add_to_symbols_table(label_name, head, CODE, 0, 0);
            }
            command_name = line;
            command_name = get_command_name(command_name);
            if (!validate_command_name(command_name)) {
                printf("command name %s not found! ", command_name);
                errors = 0;
            }
            /* TODO:
             * parse the operation and get the number of operands and size of operation
             *
             * build binary code of first word

                IC += L;

             * */
        }

    }

    ICF = IC;
    DCF = DC;
    if (errors)
        return;


    update_data_symbols(head, ICF, DCF);
    fseek(f, 0, SEEK_SET);
    while ((read = getline(&line, &len, f)) != -1) {
        symbol_def = 0;
        line = clean_empty_space(line);
        label_name = get_label_name(line);
        if (label_name != NULL) {
            continue;
        }
        if (strstr(line, ".data") != NULL || strstr(line, ".string") != NULL) {
            if (symbol_def) {
                errors = add_to_symbols_table(label_name, head, DATA, 0, 0); //todo: offset and baseaddr?
            }
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
    if (errors)
        return;
    create_output_files(head,code_head,filename);
}


