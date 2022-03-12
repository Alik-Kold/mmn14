const char *COMMANDS[] ={
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "red",
        "prn",
        "rts",
        "stop"
};

const char *INSTRUCTIONS[] = {
        ".data",
        ".string",
        ".entry",
        ".extern"
};

const char *OPERAND_PATTERN[] = {
        "^[+-]?[0-9]{1,15}$",           /* NUMBER   */
        "^[ -~]+$",                     /* STRING   */
        "^[A-Za-z][0-9A-Za-z]{0,28}$",  /* LABEL    */
        "^r[0-9][0-6]?$",               /* REGISTER */
        "["                             /* INDEX    */
};
