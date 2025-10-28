#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <tokens.h>
#include <parser.h>

int lookahead; // este é o olho do compilador
int lineno = 1;
int columno = 1;

// Interpretador de comando
//
// mybc -> cmd { cmdsep cmd} EOF
void mybc(void)
{
	cmd();

	while(lookahead == ';' || lookahead == '\n'){
		// cmdsep -> ';' | '\n'
		match(lookahead);

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

// tabela de símbolos comom dicionário dos valores armazenados na memória virtual
#define MAXSTENTRIES 4096
char symtab[MAXSTENTRIES][MAXIDLEN+1];
int symtab_next_entry = 0; //uso: strcpy(symtab[symtab_next_entry], name)
double vmem[MAXSTENTRIES];
int address;

double recall(char const *name) {
	// busca bottom-up a variável em name
	for (address = symtab_next_entry - 1; address > -1; address--) {
		if (strcmp(symtab[address], name) == 0) {
			return vmem[address];
		}
	}
	// variável ainda não existe
	address = symtab_next_entry++;
	strcpy(symtab[address], name);
	return 0.e+00;//só para enfatizar que é ponto flutuante
}

void store(char const *name) {
	recall(name);// vai localizar o endereço da variável na memória
	vmem[address] = acc;
}

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
		case DEC:
		/*4*/acc = atoi(lexeme);/**/
			match(DEC); break;
		case FLT:
		/*7*/acc = atof(lexeme);/**/
			match(FLT); break;
		default:
			// F -> ID [ := E ]
			/**/strcpy(varname, lexeme);/**/ // tem que salvar antes do match senão perde o nome
			match(ID);
			if (lookahead == ASGN) {
				match(ASGN);
				E(); //trás o resultado no acumulador (acc)
				/**/ store(varname);/**/ //armazena no endereço associado à varname
			} else {
				/**/ acc = recall(varname);/**/
			}
	}
	
	/*9*/
	if(isotimes) {
		if(isotimes == '*'){
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
		if(expected == '\n'){
			columno = 1;
			lineno++;
		} else {
			columno++;
		}
	} else {
		fprintf(stderr, "ERROR line %d column %d.\nToken mismatch, expected %c but received %c\n", lineno, columno, expected, lookahead);
		exit(ERRTOKEN);
	}
}
