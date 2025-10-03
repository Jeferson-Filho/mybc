#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>

int lookahead; // este é o olho do compilador

// Interpretador de comando
//
// mybc -> cmd { cmdsep cmd} EOF
void mybc(void)
{
	cmd();

	while(lookahead != EOF){

		// cmdsep -> ';' | '\n'
		if(lookahead == ';' || lookahead == '\n'){
			match(lookahead)
		}

		cmd();

	}

	match(EOF);
}

// cmd -> E | exit | quit | <epsilon>
void cmd(void)
{
	switch(lookahead) {
		case EXIT:
		case QUIT:
			exit(0);

		// FIRST(E):
		case '+':
		case '-':
		case '(':
		case DEC:
		case FLT:
		case HEX:
		case OCT:
		case ID:
			E();
			printf("%lg\n", acc);
			break;
		default:
			;
	}
}

extern FILE *objcode;

double acc; //acumulador
#define STACKSIZE 1024
double stack[STACKSIZE];
int sp = -1;

// E é o símbolo inicial da gramática LL(1) de expressões simplificadas
// E -> [ ominus ] T { oplus T }
// ominus = ['+''-']
// oplus = ['+''-']
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
		/*4*/acc = atoi(lexeme);/**/
			match(DEC); break;
		case OCT:
		/*5*/fprintf(objcode, " %s ", lexeme);/**/
		//Pesquisar função de octal para decimal
			match(OCT); break;
		case HEX:
		/*6*/fprintf(objcode, " %s ", lexeme);/**/
		//Pesquisar função de hexa para decimal
			match(HEX); break;
		case FLT:
		/*7*/acc = atof(lexeme);/**/
			match(FLT); break;
		default:
		/*8*/fprintf(objcode, " %s ", lexeme);/**/
			match(ID);
	}
	
	/*9*/
	if(isotimes) {
		if(isotime == '*'){
			stack[sp] = stack[sp] * acc;
		} else {
			stack[sp] = stack[sp] / acc;
		};
		acc = stack[sp--];
		isotimes = 0;
	}

	/*10*/
	if(lookahead == '*' || lookahead == '/') {
		/*10*/isotimes = lookahead;/**/
		/*10a*/stack[++sp] = acc;/**/
		match(lookahead); goto _Fbegin;
	}
    // term end
	
	/*11*/
	if(isnegate){
		acc = -acc;
		isnegate = 0;
	}

	/*12*/
	if(isoplus /* isoplus :: oplus.t */){
		if(isoplus == '+'){
			stack[sp] = stack[sp] + acc;
		} else {
			stack[sp] = stack[sp] - acc;
		};
		acc = stack[sp--];
		isoplus = 0;
	}

	/**/
	if(lookahead == '+' || lookahead == '-') {
		/**/isoplus = lookahead;/**/
		/**/stack[++sp] = acc;/**/
		match(lookahead); goto _Tbegin;
	}
	// expression end
}

//////////////////////////// parser components /////////////////////////////////
//TODO: corrigir mensagem de erro, adicionar linha e coluna
//TODO: implementar signalhandler para comandos
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
