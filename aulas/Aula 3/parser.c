#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>

// Olho do compilador
int lookahead;

// E --> TR
void E(void)
{
    T();
    R();
}

// T --> FQ
void T(void)
{
    F();
    Q();
}

// F -> '(' E ')' | DEC | OCT | HEX | FLT | ID
void F(void)
{
    switch (lookahead)
    {
    case '(':
        match('(');
        E();
        match(')');
        break;

    case DEC:
        match(DEC);
        break;

    case OCT:
        match(OCT);
        break;

    case HEX:
        match(HEX);
        break;

    case FLT:
        match(FLT);
        break;

    default:
        match(ID);
    }
}

// Q -> '*' F Q | '/' F Q | epsilon
void Q(void)
{
    switch (lookahead)
    {
    case '*':
        match('*');
        F();
        Q();
        break;

    case '/':
        match('/');
        F();
        Q();
        break;

    default:;
    }
}

// R -> '+' T R | '-' T R | epsilon
void R(void)
{
    switch (lookahead)
    {
    case '+':
        match('+');
        T();
        R();
        break;

    case '-':
        match('-');
        T();
        R();
        break;

    default:;
    }
}

/* PARSER COMPONENTES */

int lookahead;

void match(int expectedToken)
{
    if (lookahead == expectedToken)
    { // O que eu tenho na fila é igual ao que eu tenho na pilha
        lookahead = gettoken(source);
    }
    else
    {
        fprintf(stderr, "Token mismatch \n");
        exit(ERRTOKEN);
    }
}