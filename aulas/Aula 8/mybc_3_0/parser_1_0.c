#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>

int lookahead; // este é o olho do compilador

// E é o símbolo inicial da gramática LL(1) de expressões simplificadas
// E -> [ ominus ] T { oplus T }
// ominus = ['+''-']
// oplus = ['+''-']
void E(void) {
	/**/int issignal = 0;/**/
	/**/int isoplus = 0;/**/

	if(lookahead == '+' || lookahead == '-') {
		/*1*/(lookahead == '-') && (issignal = lookahead);/**/
		match(lookahead);
	}
	T();

	/*2*/
	if (issignal) {
		fprintf(objcode, " %c", issignal);
		issignal = 0;
	}
	/**/

	// { oplus t }
	while(lookahead == '+' || lookahead == '-') {
		
		/*3*/isoplus = lookahead; /**/

		match(lookahead); 
		T();

		/*4*/
		fprintf(objcode, " %c", isoplus);
		isoplus = 0;
		/**/
	}
}

// T -> F { otimes F }
void T(void) { 
	/**/int issignal = 0;/**/
	/**/int isotimes = 0;/**/

	if(lookahead == '+' || lookahead == '-') {
		/*1*/(lookahead == '-') && (issignal = lookahead);/**/
		match(lookahead);
	}

	F();

	/*2*/
	if (issignal) {
		fprintf(objcode, " %c", issignal);
		issignal = 0;
	}
	/**/

	// {otimes F}
	while(lookahead == '*' || lookahead == '/') {
		
		/*3*/isotimes = lookahead; /**/
		
		match(lookahead); 
		F();

		/*4*/
		fprintf(objcode, " %c", isotimes);
		isotimes = 0;
		/**/
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
		/**/fprintf(objcode, " %s ", lexeme);/**/
			match(DEC); break;
		case OCT:
		/**/fprintf(objcode, " %s ", lexeme);/**/
			match(OCT); break;
		case HEX:
		/**/fprintf(objcode, " %s ", lexeme);/**/
			match(HEX); break;
		case FLT:
		/**/fprintf(objcode, " %s ", lexeme);/**/
			match(FLT); break;
		default:
		/**/fprintf(objcode, " %s ", lexeme);/**/
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
