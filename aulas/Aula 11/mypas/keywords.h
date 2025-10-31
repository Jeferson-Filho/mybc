// keywords.h
#pragma once

enum {
    BEGIN = 0x10000000,
    PROGRAM,
    PROCEDURE,
    FUNCTION,
    INTEGER,
    REAL,
    BOOLEAN,
    VAR,
    IF,
    THEN,
    ELSE,
    REPEAT,
    UNTIL,
    WHILE,
    DO,
    OR,
    DIV,
    MOD,
    AND,
    TRUE,
    FALSE,
    END,
};

extern int isKeyWord(char const *name);
