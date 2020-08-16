#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <argp.h>

// retorna o numero de argumentos com a opção -.
int main(int argc, char *argv[])
{
    int cont = 0;
    while (getopt(argc, argv, "") != -1)
        cont++;
    return cont;
}
