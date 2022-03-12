/*
 * reserved words, consts and enums
 */
#ifndef PROJECT_ARTIFACTS_H
#define PROJECT_ARTIFACTS_H
#define LEN_COMMANDS 16
#define LEN_INSTRUCTIONS 4

extern const char *COMMANDS[];
extern const char *INSTRUCTIONS[];

enum opcodes{
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
    red_oc = 12,
    prn_oc = 13,
    rts_oc = 14,
    stop_oc = 15
};

enum funct{
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

enum attributes{
    EXTERNAL = 0,
    CODE = 1,
    ENTRY = 2,
    DATA = 3,
};

#endif //PROJECT_ARTIFACTS_H
