#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <argp.h>

// recebe um argumento -f (file) e retorna 0 para nao encontrado e 1 para encontrado
// caso não haja argumentos le de stdin
int main(int argc, char *argv[])
{
    FILE* file = NULL;
    int opt;
    char file_path[100];

    while ((opt = getopt(argc, argv, "f:")) != -1){
        switch (opt) {
            case 'f':
                printf("opção -f\n");
                file = fopen(optarg, "r");
                // printf("%d\n", file == NULL);
                return file == NULL;
            default:
                break;
        }
    }
    scanf("%s", file_path);
    file = fopen(file_path, "r");
    // printf("%d\n", file == NULL);

    return file == NULL;
}
