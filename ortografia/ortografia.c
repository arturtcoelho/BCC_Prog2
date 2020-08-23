// GRR 20190471 Artur Temporal Coelho

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include "dicionario.h"

#define MAX_WORD 100

int main() {

    // configura o local do programa para aceitar palavras acentuadas etc
    setlocale(LC_CTYPE, "pt_BR.ISO-8859-1");

    // inicialização das variaveis
    unsigned char **dicionario;

    int tam;

    unsigned char caractere;
    unsigned char palavra[MAX_WORD];

    // carrega o dicionario em memória
    dicionario = carregar_dicionario(&tam);

    caractere = getchar(); //le o primeiro caractere de stdin    
    while(!feof(stdin)){ // enquanto houver stdin

        // concatena caracteres para formar uma palavra
        while(isalpha(caractere)){
            strncat((char *)palavra, (char *)&caractere, 1);
            caractere = getchar();
        }   

        if (isalpha(palavra[0])){
            // imprime a palavra normalmente caso esteja no dicionário
	        if (palavra_valida(palavra, dicionario, tam)){
	            printf("%s", palavra);
	        }
            // caso contrário imprime a palavra entre colchetes
	        else{
	            printf("[%s]", palavra);
	        }
        }

        // "zera" a palavra e trata de caracteres especiais
        palavra[0] = 0;
        if (!isalpha(caractere))
            printf("%c", caractere);
        caractere = getchar();
    }

    // libera o espaço de memoria alocado para o dicionario
    finaliza_programa(dicionario, tam);

    return 0;
}