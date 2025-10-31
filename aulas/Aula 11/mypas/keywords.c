#include <stdio.h>
#include <string.h>
#include <keywords.h>

char * keywords[] = {
    "begin",
    "program",
    "procedure",
    "function",
    "integer",
    "real",
    "boolean",
    "var",
    "if",
    "then",
    "else",
    "repeat",
    "until",
    "while",
    "do",
    "or",
    "div",
    "mod",
    "and",
    "true",
    "false",
    "end",
};

int isKeyWord(char const *name)
{
    int i;
    for (i = BEGIN; i <= END; i++) {
        if (strcmp(name, keywords[i - BEGIN]) == 0) {
            return i;
        }
    }

    return 0; // not a keyword
}