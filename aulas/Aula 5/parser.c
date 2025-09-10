#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>

int lookahead; // este é o olho do compilador

// E é o símbolo inicial da gramática LL(1) de expressões simplificadas
// ominus = ⊖ =  ['+''-']
// oplus = ⊕ =['+''-']
void E(void)
{
	/**/int isnegate = 0;/**/
	/**/int isotimes = 0;/**/
	/**/int isoplus = 0;/**/

	if(lookahead == '+' || lookahead == '-') {
		if (lookahead == '-') {
			isnegate = lookahead;
		}
		match(lookahead);
	}

	_Tbegin:

	_Fbegin:

	switch(lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case DEC:
			/**/printf(" %s ", lexeme);/**/
			match(DEC); break;
		case OCT:
			/**/printf(" %s ", lexeme);/**/
			match(OCT); break;
		case HEX:
			/**/printf(" %s ", lexeme);/**/
			match(HEX); break;
		case FLT:
			/**/printf(" %s ", lexeme);/**/
			match(FLT); break;
		default:
			/**/printf(" %s ", lexeme);/**/
			match(ID);
	}
	// factor end
	/**/ // isso representa uma acao semantica
	/**/
	if(isotimes) {
		printf(" %c ", isotimes);
		isotimes = 0;
	}
	/**/

	if(lookahead == '*' || lookahead == '/') {
		/**/isotimes = lookahead;/**/
		match(lookahead); goto _Fbegin;
	}
    // term end
    /**/
    if (isnegate) {
		printf(" negate ");
		isnegate = 0;
	}
	/**/

	/**/
	if(isoplus) {
		printf(" %c ", isoplus);
		isoplus = 0;
	}
	/**/

	if(lookahead == '+' || lookahead == '-') {
		/**/isoplus = lookahead;/**/
		match(lookahead); goto _Tbegin;
	}
	// expression end

}

//////////////////////////// parser components /////////////////////////////////
int lookahead;
void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch at line %d\n", lineno);
		exit(ERRTOKEN);
	}
}
