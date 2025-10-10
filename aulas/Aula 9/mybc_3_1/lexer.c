#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <lexer.h>
#include <tokens.h>

char lexeme[MAXIDLEN + 1];

/* Versão extendida de identificador Pascal
 * ID = [A-Za-z][A-Za-z0-9]*
 */
int isID(FILE *tape)
{
	if ( isalpha(lexeme[0] = getc(tape)) ) {
		int i = 1;
		while ( isalnum( lexeme[i] = getc(tape) ) ) i++;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		if(strcmp(lexeme, "exit") == 0){
			return EXIT;
		}
		if(strcmp(lexeme, "quit") == 0){
			return QUIT;
		}
		return ID;
	}

	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

/*
 * DEC = [1-9][0-9]* | '0'
 *                           ------------------------------------------
 *                          |                      digit               |
 *                          |                    --------              |
 *                          |                   |        |             |
 *               digit      |     not zero      V        |  epsilon    V
 * -->(is DEC)--------->(is ZERO)---------->(isdigit)-------------->((DEC))
 *       |
 *       | epsilon
 *       |
 *       V
 *     ((0))
 */
int isDEC(FILE *tape)
{
	if ( isdigit(lexeme[0] = getc(tape)) ) {
		if (lexeme[0] == '0') {
			return DEC;
		}
		int i = 1;
		while ( isdigit(lexeme[i] = getc(tape)) ) i++;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return DEC;
	}

	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

// fpoint = DEC\.[0-9]* | \.[0-9][0-9]*
// flt = fpoint EE? | DEC EE
// EE = [eE]['+''-']?[0-9][0-9]*
// test input: 3e+
//             012

int isEE(FILE *tape)
{
	int i = strlen(lexeme);

	if ( toupper(lexeme[i] = getc(tape)) == 'E' ) {
		i++;
		
		// check optional signal
		int hassign;
		if ( (lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-' ) {
			hassign = i++;
		} else {
			hassign = 0;
			ungetc(lexeme[i], tape);
		}
		
		// check required digit following
		if ( isdigit(lexeme[i] = getc(tape)) ) {
			i++;
			while( isdigit(lexeme[i] = getc(tape)) ) i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return FLT;
		}
		ungetc(lexeme[i], tape);
		i--;
		if (hassign) {
			ungetc(lexeme[i], tape);
			i--;
		}
	}

	ungetc(lexeme[i], tape);
	lexeme[i] = 0;	
	return 0;
}

int isNUM(FILE *tape)
{
	int token = isDEC(tape);
	if (token == DEC) {
		int i = strlen(lexeme);
		if ( (lexeme[i] = getc(tape)) == '.' ) {
			i++;
			while ( isdigit( lexeme[i] = getc(tape) ) ) i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			token = FLT;
		} else {
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
	} else {
		if ( (lexeme[0] = getc(tape)) == '.' ) {
			if ( isdigit( lexeme[1] = getc(tape) ) ) {
				token = FLT;
				int i = 2;
				while ( isdigit( lexeme[i] = getc(tape) ) ) i++;
			} else {
				ungetc(lexeme[1], tape);
				ungetc(lexeme[0], tape);
				lexeme[0] = 0;
				return 0; // not a number
			}
		} else {
			ungetc(lexeme[0], tape);
			lexeme[0] = 0;
			return 0; // not a number
		}
	}
	
	if (isEE(tape)) {
		token = FLT;
	}

	return token;
}


int lineno = 1;

int isASGN(FILE *tape) {
	lexeme[0] = getc(tape);
	if(lexeme[0] == ':') {
		lexeme[1] = getc(tape);
		if (lexeme[1] == '=') {
			lexeme[2] = 0;
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	ungetc(lexeme[0], tape);
	return lexeme[0] = 0;
}

// Skip spaces
void skipspaces(FILE *tape)
{
	int head;
	while ( isspace(head = getc(tape)) ){
		if(head == '\n'){
			lineno++;
			break;
		};
	};
	ungetc(head, tape);
}

int gettoken(FILE *source)
{
	int token;

	skipspaces(source);

	if ( (token = isID(source)) ) return token;
	if ( (token = isNUM(source)) ) return token;
	if ( (token = isASGN(source)) ) return token;

	lexeme[0] = token = getc(source);
	lexeme[1] = 0;

	// return an ASCII token
	return token;
}
