#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10000000

// escreve 10M longints em um arquivo
int main()
{

// inicializa ponteiros
    FILE* long_int_f;
    long int *long_int_a = (long int*)malloc(SIZE * sizeof(long int));

// abre o arquivo
    long_int_f = fopen("./long_integers", "r");

    fread(long_int_a, sizeof(long int), SIZE, long_int_f);

    for (int i = 0; i < SIZE; i++)
    {
        printf("%ld\n", long_int_a[i]);
    }

// libera a memoria
    free(long_int_a);
    fclose(long_int_f);
    return 0;
}
