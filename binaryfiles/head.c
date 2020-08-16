#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10000000

// escreve na saida padrao os 10 primeiro elementos
int main()
{

// inicializa ponteiros
    FILE* long_int_f;
    long int *value = malloc(10 * sizeof(long int));

// abre o arquivo
    long_int_f = fopen("./long_integers", "r");

    fread(value, sizeof(long int), 10, long_int_f);
    for (int i = 0; i < 10; i++)
    {
        printf("%ld\n", value[i]);
    }

// libera a memoria
    fclose(long_int_f);
    return 0;
}
