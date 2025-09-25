#include <stdio.h>
#include <stdlib.h>
#include <main.h>

FILE *source, *objcode;

int main(void)
{
	lookahead = gettoken(source = stdin);
	objcode = stdout;

	E();

	return 0;
}
