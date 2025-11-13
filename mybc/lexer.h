#pragma once
#define MAXIDLEN 256

extern char lexeme[];

extern int lineno;
extern int columno;

extern void skipArrow(FILE *);
extern void skipspaces(FILE *);
