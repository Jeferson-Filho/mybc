//rodar com cc -S teste.c -o teste.s para gerar o código em assembly
#include <stdio.h>

int x = 3;
int y = 4;
int z;

int main(void) {
    x = x + y;

    printf("z = %d\n", z);

    return 0;
}