#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>

int lookahead; // este é o olho do compilador

// E é o símbolo inicial da gramática LL(1) de expressões simplificadas
// ominus = ['+''-']
// oplus = ['+''-']
void E(void) {

	if(lookahead == '+' || lookahead == '-') {
		match(lookahead);
	}

_Tbegin:
	T();
	if(lookahead == '+' || lookahead == '-') {
			match(lookahead); goto _Tbegin;
	}

}

// T -> F { otimes F }
void T(void) { 

_Fbegin:
	F();
	if(lookahead == '*' || lookahead == '/') {
			match(lookahead); goto _Fbegin;
	}

}

// F -> '(' E ')' | DEC | OCT | HEX | FLT | ID
void F(void)
{
	switch(lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case DEC:
			match(DEC); break;
		case OCT:
			match(OCT); break;
		case HEX:
			match(HEX); break;
		case FLT:
			match(FLT); break;
		default:
			match(ID);
	}
}

//////////////////////////// parser components /////////////////////////////////
int lookahead;
void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch\n");
		exit(ERRTOKEN);
	}
}
