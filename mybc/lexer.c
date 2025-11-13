/*
Grupo 2
- Caio Bohlhalter de Souza
- Jeferson Patrick Dietrich Filho
- Lucas Quintas Honorato
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <lexer.h>
#include <string.h>
#include <tokens.h>

//Variáveis Globais
char lexeme[MAXIDLEN + 1];
int lineno = 1;
int columno = 1;

/* Versão extendida de identificador Pascal
 * 
 * Verifica se o lexeme é um identificador (ID)
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 */
int isID(FILE *tape)
{
	if ( isalpha(lexeme[0] = getc(tape)) ) {
		int i = 1;
		while ( isalnum( lexeme[i] = getc(tape) ) ) i++;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;

		// Verifica palavras-chave reservadas
		if(strcmp(lexeme, "exit") == 0){
			return EXIT;
		}

		if(strcmp(lexeme, "quit") == 0){
			return QUIT;
		}

		// Identificador válido (não reservado)
		return ID;
	}

	//  Não é um ID válido, devolve o caractere lido
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

/*
 * Verifica se o lexeme é um número decimal
 * DEC = [1-9][0-9]* | 0
 */
int isDEC(FILE *tape)
{
	if ( isdigit(lexeme[0] = getc(tape)) ) {
		// Se for zero, é um número decimal válido
		if (lexeme[0] == '0') {
			return DEC;
		}
		int i = 1;
		while ( isdigit(lexeme[i] = getc(tape)) ) i++;
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		// Número decimal válido
		return DEC;
	}

	// Não é um número decimal válido, devolve o caractere lido
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	return 0;
}

/*
 * Função que determina se o lexeme contém uma notação exponencial (EE)
 * EE = [eE]['+''-']?[0-9][0-9]*
*/
int isEE(FILE *tape)
{
	int i = strlen(lexeme);
	// Verifica se o próximo caractere é 'E' ou 'e'
	if (toupper(lexeme[i] = getc(tape)) == 'E' )
	{
		i++;
        // Checagem de sinal opcional
        int hassign = 0;
        if ((lexeme[i] = getc(tape)) == '+' || lexeme[i] == '-') 
		{
            i++;
            hassign = i;
        } else {
			hassign = 0;
            ungetc(lexeme[i], tape);
        }

        // Checagem do digito obrigatorio subsequente
        if (isdigit(lexeme[i] = getc(tape))) 
		{
			i++;
			while ( isdigit(lexeme[i] = getc(tape))) i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			// Notação exponencial válida, é um número flutuante
			return FLT;
		}

		// Devolve o caractere que não é dígito, e devolve o sinal se existir
		ungetc(lexeme[i], tape);
		i--;
		if(hassign)
		{
			ungetc(lexeme[i], tape);
			i--;
		}
	}
	ungetc(lexeme[i], tape);
	lexeme[i] = 0;
	// Não é notação exponencial válida
	return 0;
}

/*
 * Função que determina se o lexeme é um numero
 * podendo ser inteiro, ponto flutuante ou notação exponencial
*/
int isNUM(FILE *tape)
{
	// Tenta ler um número decimal
	int token = isDEC(tape);
	if (token == DEC) {
		// Se obteve DEC, verifica se há ponto flutuante
		int i = strlen(lexeme);
		if ( (lexeme[i] = getc(tape)) == '.' ) {
			i++;
			while ( isdigit( lexeme[i] = getc(tape) ) ) i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			// É um número flutuante
			token = FLT;
		} else {
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
		
		// Após reconhecer DEC ou FLT com ponto, verifica se há notação exponencial
		if (isEE(tape)) {
			token = FLT;
		}
	} else {
		// Não começou com dígito, verifica se começa com ponto (ex: .5)
		if ( (lexeme[0] = getc(tape)) == '.' ) {
			if ( isdigit( lexeme[1] = getc(tape) ) ) {
				token = FLT;
				int i = 2;
				while ( isdigit( lexeme[i] = getc(tape) ) ) i++;
				
				// Verifica se há notação exponencial após ponto flutuante
				if (isEE(tape)) {
					token = FLT;
				}
			} else {
				// Não há dígito após ponto, não é um número
				ungetc(lexeme[1], tape);
				ungetc(lexeme[0], tape);
				lexeme[0] = 0;
				return 0;
			}
		} else {
			// Não começa com dígito nem com ponto, então não é um número
			ungetc(lexeme[0], tape);
			lexeme[0] = 0;
			return 0;
		}
	}

	return token;
}

/*
 * Verifica se o lexeme é um operador de atribuição
 * ASGN = ':='
 */
int isASGN(FILE *tape) {
	lexeme[0] = getc(tape);
	if(lexeme[0] == ':') 
	{
		lexeme[1] = getc(tape);
		if (lexeme[1] == '=') 
		{
			// É o operador de atribuição ':='
			lexeme[2] = 0;
			return ASGN;
		}
		ungetc(lexeme[1], tape);
	}
	// Não é operador de atribuição
	ungetc(lexeme[0], tape);
	return lexeme[0] = 0;
}

/*
 * Função que determina se o lexeme é um numero octal 
 * OCT = '0'[0-7]+
 */
int isOCT(FILE *tape)
{
	// Verifica se começa com '0'
	if ( (lexeme[0] = getc(tape)) == '0') 
	{
		int i = 1;
		// Verifica se o próximo caractere é um dígito octal (0-7)
		if ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7') 
		{
			i = 2;
			while ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7') i++;
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			// Número octal válido
			return OCT;
		}
		// Não é octal, devolve os caracteres lidos
		ungetc(lexeme[1], tape);
		ungetc(lexeme[0], tape);
	} else {
		// Não começa com '0', devolve o caractere lido
		ungetc(lexeme[0], tape);
	}
	// Não é um número octal válido
	return 0;
}

/*
 * HEX = '0'[Xx][0-9A-Fa-f]+
 *
 * isxdigit == [0-9A-Fa-f]
 */
int isHEX(FILE *tape)
{
	// Para ter um hexa, é necessário que venha o prefixo "0[xX]" seguido de um hexa digito
	if ( (lexeme[0] = getc(tape)) == '0' ) {

		if ( toupper(lexeme[1] = getc(tape)) == 'X' ) {
			// Verifica se há um dígito hexadecimal após o prefixo
			if ( isxdigit(lexeme[2] = getc(tape)) ) {

				int i = 3;
				while ( isxdigit(lexeme[i] = getc(tape)) ) i++;
				ungetc(lexeme[i], tape);
				lexeme[i] = 0;
				// Número hexadecimal válido
				return HEX;
			}
			// Não há dígito hex após o prefixo, devolve os caracteres
			ungetc(lexeme[2], tape);
			ungetc(lexeme[1], tape);
			ungetc(lexeme[0], tape);
			lexeme[0] = 0;
			return 0;
		}
		// Não é 'X'/'x', devolve os caracteres lidos
		ungetc(lexeme[1], tape);
		ungetc(lexeme[0], tape);
		lexeme[0] = 0;
		return 0;
	}
	// Não começa com '0', devolve o caractere lido
	ungetc(lexeme[0], tape);
	lexeme[0] = 0;
	// Não é um número hexadecimal válido
	return 0;
}

/*
 * Salta caracteres de espaçamento (espaços, tabs, quebras de linha)
 * Atualiza as variáveis globais lineno e columno para rastrear posição do erro
 */
void skipspaces(FILE *tape)
{
	int head;
	// Lê caracteres enquanto forem espaçamento
	while ( isspace(head = getc(tape)) ){
		// Modifica 'columno' para rastrear a coluna do erro
		if(head == '\t'){
			// Caso leia tab, aumenta em 2 o tamanho ao invés de 1
			columno+=8;
		}
		else{
			columno++;
		}
		// Se encontrar quebra de linha, atualiza 'lineno' e 'columno' para rastrear a posição do erro
		if(head == '\n'){
			lineno++;
			columno = 0;
			break;
		};
	};
	if(head == 0x1B){
		// Devolve o caractere lido, que não é espaçamento
		ungetc(head, tape);
		// Verifica se é seta
		skipArrow(tape);
	}
	else{
		// Devolve o caractere lido, que não é espaçamento
		ungetc(head, tape);
	}
}

// Ignora input de setas
// Arrow = '^' '[' '[' 'A' | 'B' | 'C' | 'D' 
void skipArrow(FILE *tape)
{
	while ((lexeme[0] = getc(tape)) == 0x1B) { // ESC (27)

		// Verifica se é uma sequência de seta
		if ((lexeme[1] = getc(tape)) == '[')
		{
			lexeme[2] = getc(tape);
			
			if(lexeme[2] == 'A' || lexeme[2] == 'B' || lexeme[2] == 'C' || lexeme[2] == 'D') {
				// Modifica 'columno' para rastrear a coluna do erro
				columno++;
			}else{
				// Se não for uma seta, devolve o caracter lido
				ungetc(lexeme[2], tape);
			}
		} else {
			// Não é uma seta, devolve o que foi lido
			ungetc(lexeme[1], tape);
		}
	}
	if(isspace(lexeme[0])){
		// Devolve o caractere lido que não é ESC (27)
		ungetc(lexeme[0], tape);
		lexeme[0] = 0;
		// Verifica se é espaço
		skipspaces(tape);
	}
	else{
		// Devolve o caractere lido que não é ESC (27)
		ungetc(lexeme[0], tape);
		lexeme[0] = 0;
	}
}

int gettoken(FILE *source)
{
	int token;

	skipspaces(source);
	if ( (token = isID(source)) ) return token;
	if ( (token = isNUM(source)) ) return token;
	if ( (token = isASGN(source)) ) return token;
	//	if ( (token = isHEX(source)) ) return token;
	//	if ( (token = isOCT(source)) ) return token;

	lexeme[0] = token = getc(source);
	lexeme[1] = 0;

	// return an ASCII token
	return token;
}
