#pragma once
#define ERRTOKEN -0x10000000

extern int lookahead;
extern double acc;

extern char lexeme[];

extern int lineno;

extern FILE *source;

extern void match(int);

extern int gettoken(FILE *);

extern void program(void);
extern void block(void);
extern void idlist(void);
extern void parmlist(void);
extern void sbprogram(void);
extern void vardeclaration(void);
extern void type(void);
extern void begin_end(void);
extern void idstmt(void);
extern void ifstmt(void);
extern void repstmt(void);
extern void whilestmt(void);
extern void stmt(void);
extern void stmtlist(void);
extern void expression(void);
extern void simple_expression(void);
