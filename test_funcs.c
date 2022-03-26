#include "test_funcs.h"


/*
 * Print out contents of Symbol_table
 * @params head - ptr to the symbol table head
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
        printf("%d\t%d\t%d\t", node->value, node->base_addr, node->offset);
        for (i=0; i < LEN_INSTRUCTIONS; i++) if (node->attribute[i]) printf("%s ", ATTRIBUTES[i]);
        printf("\n");
        print_breaker();
        node = node->next;
    }
    print_breaker();
    print_pattern(2, "\n");
}


/*
 * Print out contents of machine_code
 * @params head - ptr to the command machine code table head
 * */
void print_machine_code(struct Machine_code *head){
    int i;
    struct Machine_code *node = head;

    print_breaker();
    print_breaker();
    printf("position\tis_data\tvalues\n");
    while (node != NULL) {
        print_breaker();
        printf("%d\t\t%d\t",node->position, node->is_data);
        for (i=ARE_BITS_LEN + WORD_BITS; i >= 0 ; i--) printf("%d", node->val[i]);
        printf("\n");
        node = node->next;
    }
    print_breaker();
    print_pattern(2, "\n");
}
