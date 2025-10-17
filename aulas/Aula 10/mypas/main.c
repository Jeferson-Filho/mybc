#include <stdio.h>
#include <stdlib.h>
#include <main.h>

FILE *source, *objcode;

int main(void)
{
	lookahead = gettoken(source = stdin);
	objcode = stdout;

	mypas();

	return 0;
}
