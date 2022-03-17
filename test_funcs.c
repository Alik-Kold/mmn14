#include "test_funcs.h"


static void print_pattern(unsigned int i, char * str){while(i--) printf("%s", str);}

static void print_breaker(){
    print_pattern(50, "-");
    printf("\n");
}

static void print_str_with_padding(size_t max_pad, const char *str) {
    int i;
    printf("%s", str);
    for (i=0; i < max_pad + 1 - strlen(str); i++) printf(" ");
}


/*
 * Print out all values in Symbol_table
 * */
void print_symbol_table(struct Symbol_table *head){
    int i;
    size_t max_len = 6;
    struct Symbol_table *node = head;

    print_breaker();
    print_breaker();
    while (node) {
        if (max_len < strlen(node->symbol)) max_len = strlen(node->symbol);
        node = node->next;
    }
    print_str_with_padding(max_len, "label");
    printf("value\tbase\toffset\tattributes\n");
    node = head;
    while (node != NULL) {
        print_str_with_padding(max_len, node->symbol);
        printf("%d\t\t%d\t\t%d\t\t", node->value, node->base_addr, node->offset);
        for (i=0; i < LEN_INSTRUCTIONS; i++) if (node->attribute[i]) printf("%s ", ATTRIBUTES[i]);
        printf("\n");
        print_breaker();
        node = node->next;
    }
    print_breaker();

}


/*
 * Print out all the encodings
 * */
void print_machine_code(struct Machine_code *head){
    int i;
    struct Machine_code *node = head;

    printf("position\tis_data\t\tvalues\n");
    while (node != NULL) {
        print_breaker();
        printf("%d\t\t%d\t\t",node->position, node->is_data);
        for (i=0; i < 1 + ARE_BITS_LEN + WORD_BITS; i++) printf("%d", node->val[i]);
        printf("\n");
        node = node->next;
    }
    print_breaker();
    print_breaker();
}
