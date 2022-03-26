/*
 * Project reserved words, consts and enums
 */
#ifndef PROJECT_ARTIFACTS_H
#define PROJECT_ARTIFACTS_H
#define LEN_COMMANDS 16
#define LEN_INSTRUCTIONS 4
#define WORD_BITS 16
#define ARE_BITS_LEN 3
#define LINE_MAX_LEN 80
#define LABEL_MAX_LEN 32
#define IC_INIT 100
#define IC_MAX 8191
#define NUM_OF_REGISTERS 16
#define REGISTER_PREFIX 'r'

/*
 * Multiples of two,
 * in order to only toggle the appropriate bit during encoding
 */
enum are_flags{
    EXTERNAL_FLAG    = 1,
    RELOCATABLE_FLAG = 2,
    ABSOLUTE_FLAG    = 4
};

extern const char *COMMANDS[];
enum opcodes{
    mov_oc =  1,        /* 0  */
    cmp_oc =  2,        /* 1  */
    add_oc =  4,        /* 2  */
    sub_oc =  4,        /* 2  */
    lea_oc =  16,       /* 4  */
    clr_oc =  32,       /* 5  */
    not_oc =  32,       /* 5  */
    inc_oc =  32,       /* 5  */
    dec_oc =  32,       /* 5  */
    jmp_oc =  512,      /* 9  */
    bne_oc =  512,      /* 9  */
    jsr_oc =  512,      /* 9  */
    red_oc =  4096,     /* 12 */
    prn_oc =  8192,     /* 13 */
    rts_oc =  16384,    /* 14 */
    stop_oc = 32768     /* 15 */
};

/*
 * Enumerate existing functs
 * Extrapolate missing functs for completeness
 */
enum funct{
    red_funct =  0,
    prn_funct =  0,
    mov_funct =  0,
    cmp_funct =  0,
    lea_funct =  0,
    rts_funct =  0,
    stop_funct = 0,
    add_funct =  10,
    sub_funct =  11,
    clr_funct =  10,
    not_funct =  11,
    inc_funct =  12,
    dec_funct =  13,
    jmp_funct =  10,
    bne_funct =  11,
    jsr_funct =  12
};

enum operand_types{
    NUMBER          = 0,
    LABEL           = 2,
    REGISTER        = 3,
    INDEX_NUMBER    = 5,
    INDEX_LABEL     = 6,
    INDEX_REGISTER  = 7
};

enum addressing{
    IMMEDIATE       = 0,
    DIRECT          = 1,
    INDEXING        = 2,
    REGISTER_DIRECT = 3
};

extern const char *INSTRUCTIONS[];
extern const char *ATTRIBUTES[];
enum attributes{
    EXTERNAL = 0,
    CODE     = 1,
    ENTRY    = 2,
    DATA     = 3
};

#endif
