/*@<lexer.c>::*/

#include <stdio.h>
#include <ctype.h>
#include <lexer.h>

char lexeme[MAXIDLEN + 1];

/*
 * ID = [A-Za-z][A-Za-z0-9]*
 */
int isID(FILE *tape)
{
    int head;

    head = getc(tape);

    if (isalpha(head))
    {
        while (isalnum(head = getc(tape)))
            ;
        ungetc(head, tape);
        return ID;
    }

    ungetc(head, tape);
    return 0;
}

/*
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
            while ((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <= '7')
            {
                ungetc(lexeme[i], tape);
                i++;
            };

            return OCT;
        };

        ungetc(lexeme[i], tape);
        ungetc('0', tape);
    }
    else
    {
        ungetc(lexeme[0], tape);
    }
    return 0;
}

/*
 * HEX = [1-9][0-9]* | 0
 */
int isHEX(FILE *tape)
{
    int x, ch;

    ch = getc(tape);
    if (ch != '0')
    {
        ungetc(ch, tape);
        return 0;
    }

    ch = getc(tape);
    if ((ch != 'X') && (ch != 'x'))
    {
        ungetc(ch, tape);
        ungetc('0', tape);

        return 0;
    }

    x = ch;
    ch = getc(tape);
    if (!(isxdigit(ch)))
    {
        ungetc(ch, tape);
        ungetc(x, tape);
        ungetc('0', tape);

        return 0;
    }

    while (isxdigit(ch = getc(tape)))
        ;
    ungetc(ch, tape);

    return HEX;
}

/*
 * FLT =
 */
int isFLT(FILE *tape)
{
    // TODO
}

void skipspaces(FILE *tape)
{
    int head;

    while (isspace(head = getc(tape)))
        ;

    ungetc(head, tape);
}

int gettoken(FILE *source)
{
    int token;

    skipspaces(source);

    if ((token = isID(source)))
        return token;
    if ((token = isOCT(source)))
        return token;
    if ((token = isHEX(source)))
        return token;
    if ((token = isDEC(source)))
        return token;
    token = getc(source);

    return token;
}
