#include "test_funcs.h"

static void print_breaker(){
    printf("--------------------------------------------\n");
}


/*
 * Print out all values in Symbol_table
 * */
void print_symbol_table(struct Symbol_table *head){
    int i;
    struct Symbol_table *node = head;

    print_breaker();
    printf("label\tvalue\tbase\toffset\tattributes\n");
    print_breaker();
    while (node != NULL) {
        printf("%s\t%d\t\t%d\t\t%d\t\t",node->symbol, node->value, node->base_addr, node->offset);
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

    printf("position\tis_data\tvalues\n");
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


static struct person *johndoe;
static char report[255];

/*
 * print any struct
 * lifted this one from
 * https://stackoverflow.com/questions/3311182/linux-c-easy-pretty-dump-printout-of-structs-like-in-gdb-from-source-co
 */
void printout_struct(void* invar, char* structname){
    /* dumpstack(void) Got this routine from http://www.whitefang.com/unix/faq_toc.html
    ** Section 6.5. Modified to redirect to file to prevent clutter
    */
    /* This needs to be changed... */
    char dbx[500];

    sprintf(dbx, "echo 'p (struct %s)*%p\n' > gdbcmds", structname, invar );
    system(dbx);

    sprintf(dbx, "echo 'where\ndetach' | gdb -batch --command=gdbcmds %s %d > struct.dump", __progname, getpid() );
    system(dbx);

    sprintf(dbx, "cat struct.dump");
    system(dbx);
}
