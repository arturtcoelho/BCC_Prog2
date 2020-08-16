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
    long_int_f = fopen("./long_integers", "w+");

    srand(clock());

// gera os numeros em memoria
    for (int i = 0; i < SIZE; i++)
        long_int_a[i] = random() * (random() & 1 ? -1 : 1);
    
// escreve no arquivo em disco
    fwrite(long_int_a, sizeof(long int), SIZE, long_int_f);

// libera a memoria
    free(long_int_a);
    fclose(long_int_f);
    return 0;
}
