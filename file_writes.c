#include "file_writes.h"

void create_output_files(struct Symbol_table *pTable, struct Machine_code *pCode,char* filename,int ICF, int DCF) {
    char *ext_file_name, *ent_file_name, *ob_file_name, line[80], group_name ;
    struct Symbol_table *table_point;
    FILE* ext, *ent,*ob;
    char head_title[80];
    int number, i;
    char ascii_value;
    int DC;

    set_file_extention(filename,&ext_file_name,".ext");
    set_file_extention(filename,&ent_file_name,".ent");
    set_file_extention(filename,&ob_file_name,".ob");

    ext = fopen(ext_file_name,"w");
    ent = fopen(ent_file_name,"w");
    ob = fopen(ob_file_name,"w");


    table_point = pTable;
    sprintf(head_title,"%d %d\n",ICF - IC_INIT , DCF);

    fwrite(head_title,strlen(head_title),1,ob);

    DC = 100;
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
        reverse_array(pCode->val, 20);
        for (i = 0; i < 5 ; i++){

            number = pCode->val[i * 4] * 1000 + pCode->val[i*4 + 1] * 100 + pCode->val[i*4 + 2] * 10 + pCode->val[i*4 + 3] * 1;
            ascii_value = bin_to_hex(number);
            if      (i == 0)
                sprintf(line,"0%d %c%c-",DC,   group_name, ascii_value);
            else if (i == 4)
                sprintf(line,"%s%c%c\n",  line, group_name, ascii_value);
            else
                sprintf(line,"%s%c%c-", line, group_name, ascii_value);

            group_name++;
        }
        DC++;
        fwrite(line, strlen(line),1,ob);
        pCode = pCode->next;
    }

    fclose(ext);
    fclose(ent);
    fclose(ob);
}
