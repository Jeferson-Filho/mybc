#define ERRTOKEN -0x10000000

extern int lookahead;
extern double acc;

extern char lexeme[];

extern int lineno;

extern FILE *source;

extern void match(int);

extern int gettoken(FILE *);

extern void mybc(void);
extern void cmd(void);
extern void expression(void);
extern void simple_expression(void);
