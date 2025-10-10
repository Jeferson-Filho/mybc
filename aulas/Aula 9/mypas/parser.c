#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <tokens.h>
#include <parser.h>

int lookahead; // este é o olho do compilador

extern FILE *objcode;

// tabela de símbolos comom dicionário dos valores armazenados na memória virtual
#define MAXSTENTRIES 4096
char symtab[MAXSTENTRIES][MAXIDLEN+1];
int symtab_next_entry = 0; //uso: strcpy(symtab[symtab_next_entry], name)
double vmem[MAXSTENTRIES];

int address;

// E é o símbolo inicial da gramática LL(1) de expressões simplificadas
// E -> [ ominus ] T { oplus T }
// ominus = ['+''-']
// oplus = ['+''-']
void E(void)
{
	/*0*/int varname[MAXIDLEN+1];/**/
	/*1*/int isnegate = 0;/**/
	/*2*/int isotimes = 0;/**/
	/*3*/int isoplus = 0;/**/

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
		case DEC:// int32
			/**/fprintf(objcode, "\tmovl %s, %%eax\n", lexeme);/**/
			match(DEC); break;
		case FLT:
			match(FLT); break;
		default:
			// F -> ID [ := E ]
			/**/strcpy(varname, lexeme);/**/ // tem que salvar antes do match senão perde o nome
			match(ID);
			if (lookahead == ASGN) {
				match(ASGN);
				E(); //trás o resultado no acumulador (acc)
				/**/ fprintf(objcode, "\tmovl %%eax, %s\n", varname);/**/ //armazena no endereço associado à varname
			} else {
				/**/ fprintf(objcode, "\tmovl %%eax, %s\n", varname);/**/
			}
	}
	
	/*9*/
	if(isotimes) {
		if(isotimes == '*'){
			fprintf(objcode, "\timull (%%esp)\n");
			fprintf(objcode, "\taddl $4, %%esp\n");
		} else {
			fprintf(objcode, "\tmovl %%eax, %%ecx\n");
			fprintf(objcode, "\tpopl %%eax\n");
			fprintf(objcode, "\tcltq %%eax\n");
			fprintf(objcode, "\tidivl %%ecx\n");
		};
		isotimes = 0;
	}
	/**/

	/*10*/
	if(lookahead == '*' || lookahead == '/') {
		/**/isotimes = lookahead;/**/
		fprintf(objcode, "\tpushl %%eax\n");
		match(lookahead); goto _Fbegin;
	}
    // term end
	
	/*11*/
	// if(isnegate){
	// 	acc = -acc;
	// 	isnegate = 0;
	// }

	/*12*/
	// if(isoplus /* isoplus :: oplus.t */){
	// 	if(isoplus == '+'){
	// 		stack[sp] = stack[sp] + acc;
	// 	} else {
	// 		stack[sp] = stack[sp] - acc;
	// 	};
	// 	acc = stack[sp--];
	// 	isoplus = 0;
	// }

	/**/
	if(lookahead == '+' || lookahead == '-') {
		// /**/isoplus = lookahead;/**/
		// /**/stack[++sp] = acc;/**/
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
