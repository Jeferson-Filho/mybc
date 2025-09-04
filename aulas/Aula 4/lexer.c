/*@<lexer.c>::*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <lexer.h>

//Variáveis Globais
char lexeme[MAXIDLEN + 1];

/*
 * Verifica se o lexeme é um identificador (ID)
 * ID = [A-Za-z][A-Za-z0-9]*
 */
int isID(FILE *tape)
{
    int head;

    head = getc(tape);

    if (isalpha(head))
    {
        while (isalpha(head = getc(tape)))
            ;
        ungetc(head, tape);
        return ID;
    }

    ungetc(head, tape);
    return 0;
}

/*
 * Verifica se o lexeme é um número decimal
 * DEC = [1-9][0-9]* | 0
 */
int isDEC(FILE *tape)
{
    if (isdigit(lexeme[0] = getc(tape)))
    {
        if (lexeme[0] == '0')
        {
            return DEC;
        }
        int i = 1;
        while (isdigit(lexeme[i] = getc(tape)))
            i++;
        ;
        ungetc(lexeme[i], tape);
        return DEC;
    }
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
    if (toupper(lexeme[i] = getc(tape)) == 'E' )
    {
        i++;

        // Checagem de sinal opcional
        int hassign = 0;
        if (lexeme[i] = getc(tape) == '+' || lexeme[i] == '-') {
            i++;
            hassign = i;
        } else {
            ungetc(lexeme[i], tape);
        }

        // Checagem do digito obrigatorio subsequente
        if (isdigit(lexeme[i] = getc(tape))) {
            i++;
            while ( isdigit(lexeme[i] = getc(tape))) i++;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return FLT;
        }

        ungetc(lexeme[i], tape);
        i--;
        if(hassign){
            ungetc(lexeme[i], tape);
            i--;
        }
    }
    ungetc(lexeme[i], tape);
    lexeme[i] = 0;
    return 0;
}

/*
 * Função que determina se o lexeme é um numero, podendo ser inteiro ou ponto flutuante 
*/
int isNUM(FILE *tape)
{
    int token = isDEC(tape);
    if (token == DEC)
    {
        int i = strlen(lexeme);
        if (lexeme[i] = getc(tape) == '.')
        {
            i++;
            while ( isdigit(lexeme[i] = getc(tape))) i++;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            token = FLT;
        } else {
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
        }
    } else {
        if (lexeme[0] = getc(tape) == '.')
        {
            if (isdigit(lexeme[1] = getc(tape))) {
                token = FLT;
                int i = 2;
                while ( isdigit(lexeme[i] = getc(tape))) i++;
            } else {
                ungetc(lexeme[1], tape);
                ungetc(lexeme[0], tape);
                lexeme[0] = 0;
                return 0; // Not a Number
            }
        } else {
            ungetc(lexeme[0], tape);
            lexeme[0] = 0;
            return 0; // Not a Number
        }
    }

    if (isEE(tape))
    {
        token = FLT;
    }

    return token;
}

// #define isoct(x) (x >= '0' && x <= '7')
int isoct(int ch)
{
    return (ch >= '0' && ch <= '7');
}

/**
 * start --> 0 --> 0-7 --> OK
 *   |
 *   |
 * exit
 *
 *
 *
 *
 */

/*
 * OCT =  0[0-7]
 */
int isOCT(FILE *tape)
{
    lexeme[0] = getc(tape);

    if (lexeme[0] == '0')
    {
        int i = 1;
        if ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7')
        {
            i = 2;
            while ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7') i++;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;

            return OCT;
        };

        ungetc(lexeme[1], tape);
        ungetc(lexeme[0], tape);
    }
    else
    {
        ungetc(lexeme[0], tape);
    }
    return 0;
}

/*
 * Função para determinar se o lexeme é hexadecimal
 * HEX = '0'[Xx][1-9][0-9]* | 0
 */
int isHEX(FILE *tape)
{
    if( (lexeme[0] = getc(tape)) == '0' )
    {
        if( toupper(lexeme[1] = getc(tape)) == 'X' )
        {
            if( isxdigit(lexeme[2] = getc(tape)) )
            {
                int i = 3;
                while( isxdigit(lexeme[i] = getc(tape)) ) i++;
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return HEX;
            }

            ungetc(lexeme[2], tape);
            ungetc(lexeme[1], tape);
            ungetc(lexeme[0], tape);
            lexeme[0] = 0;
            return 0;
        }
        ungetc(lexeme[1], tape);
        ungetc(lexeme[0], tape);
        lexeme[0] = 0;
        return 0;
    }
    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

/*
 * fpoint = DEC\.[0-9]* | \.[0-9][0-9]*
 * flt = fpoint EE? | DEC EE
 */
int isFLT(FILE *tape)
{
    
}

/*
 * Função que ignora espaços em branco
*/
void skipspaces(FILE *tape)
{
    int head;

    while (isspace(head = getc(tape)))
        ;

    ungetc(head, tape);
}

/*
 * Função que determina o próximo token a ser lido
*/
int gettoken(FILE *source)
{
    int token;

    skipspaces(source);

    if ((token = isID(source))) return token;
    if ((token = isNUM(source))) return token;
    if ((token = isDEC(source))) return token;
    if ((token = isOCT(source))) return token;
    if ((token = isHEX(source))) return token;

    lexeme[0] = token = getc(source);
    lexeme[1] = 0;

    return token;
}
