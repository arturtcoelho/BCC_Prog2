// GRR 20190471 Artur Temporal Coelho
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dicionario.h"

int main() {

    // inicialização das variaveis
    char **dicionario;

    int tam;

    char caractere;
    char palavra[100];

    // carrega o dicionario em memória
    dicionario = carregar_dicionario(&tam);


    caractere = getchar(); //le o primeiro caractere de stdin    
    while(!feof(stdin)){ // enquanto houver stdin

        // concatena caracteres para formar uma palavra
        while(eh_letra(caractere)){
            strncat(palavra, &caractere, 1);
            caractere = getchar();
        }   

        if (eh_letra(palavra[0])){
            // imprime a palavra normalmente caso esteja no dicionário
	        if (palavra_valida(palavra, dicionario, tam)){
	            printf("%s", palavra);
	        }
            // caso contrário imprime a palavra entre colchetes
	        else{
	            printf("[%s]", palavra);
	        }
        }

        // "zera" a palavra e trata de caracteres especiais (e.g. ' " , . : etc)
        palavra[0] = 0;
        if (!eh_letra(caractere))
            printf("%c", caractere);
        caractere = getchar();
    }

    // libera o espaço de memoria alocado para o dicionario
    finaliza_programa(dicionario, tam);

    return 0;
}