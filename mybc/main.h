#pragma once
// Declarações:
extern int lookahead; // a ser definido no parser
extern int gettoken(FILE *); // a ser definido no lexer

extern double acc;
extern void mybc(void);