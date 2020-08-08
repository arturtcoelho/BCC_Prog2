#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dicionario.h"

int main() {

    char **dicionario;

    int tam;

    dicionario = carregar_dicionario(&tam);

    char caractere;
    char palavra[100];

    // printf("%d\n", palavra_valida(palavra, dicionario, tam));
    caractere = getchar();
    
    while(!feof(stdin)){
        while(eh_letra(caractere)){
            strncat(palavra, &caractere, 1);
            caractere = getchar();
        }   
        if (eh_letra(palavra[0])){
	        if (palavra_valida(palavra, dicionario, tam)){
	            printf("%s", palavra);
	        }
	        else{
	            printf("[%s]", palavra);
	        }
        }
        palavra[0] = 0;
        if (!eh_letra(caractere))
            printf("%c", caractere);
        caractere = getchar();
    }

    finaliza_programa(dicionario, tam);

    return 0;
}