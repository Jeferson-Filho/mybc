#define ERRTOKEN -0x10000000

extern FILE *source;
extern int lookahead;

extern char lexeme[];

extern void match(int);

extern int gettoken(FILE *);

extern void E(void);
extern void T(void);
extern void F(void);
extern void Q(void);
extern void R(void);