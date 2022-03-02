/*
 * a file holding reserverd words and command names
 */

int LEN_COMMANDS = 16;

char **COMMANDS =
        {
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

int LEN_INSTRUCTIONS = 4;

char **INSTRUCTIONS =
        {
            ".data",
            ".string",
            ".entry",
            ".extern"
        };