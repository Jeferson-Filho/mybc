/*@<lexer.c>::*/

#include <stdio.h>
#include <ctype.h>

// #define isoct(x) (x >= '0' && x <= '7')
enum
{
    ID = 1024,
    OCT,
    DEC,
    HEX,
};

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
    int head;

    head = getc(tape);

    if (isdigit(head))
    {
        if (head == '0')
        {
            return DEC;
        }
        while (isdigit(head = getc(tape)))
            ;
        ungetc(head, tape);
        return DEC;
    }
    ungetc(head, tape);
    return 0;
}

int isoct(int ch)
{
    return (ch >= '0' && ch <= '7');
}

/*
 * OCT =  0[0-7]
 */
int isOCTAL(FILE *tape)
{
    int ch = getc(tape);
    if (ch != '0')
    {
        ungetc(ch, tape);
        return 0;
    }

    ch = getc(tape);
    if (ch < '0' || ch > '7' || !(isdigit(ch)))
    {
        ungetc(ch, tape);
        ungetc('0', tape);

        return 0;
    }

    while ((ch = getc(tape)) && (ch >= '0') && (ch >= '7'))
        ;
    ungetc(ch, tape);

    return OCT;
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
    if ((token = isOCTAL(source)))
        return token;
    if ((token = isHEX(source)))
        return token;
    if ((token = isDEC(source)))
        return token;
    token = getc(source);

    return token;
}
