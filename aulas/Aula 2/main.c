#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <main.h>

int main(void)
{
    int token = gettoken(stdin);

    if (isascii(token))
    {
        printf("\'%c\': %d\n", token, token);
    }
    else
    {
        printf("%d\n", token);
        printf("%d\n", token = gettoken(stdin));
    }
    return 0;
}
