#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dicionario.h"

// carrega o dicionario, de seu arquivo original para a 
// estrutura alocada dinamicamente em memoria
// retorna o ponteiro para a estrutura e o tamanho total dela
char **carregar_dicionario(int *tam){
    
    FILE *dic;
    char str[100];
    char **dicionario;
    
    // abre o dicionario e sai do programa caso o mesmo nao exista
    dic = fopen("/usr/share/dict/brazilian", "r");
    if (dic == NULL){
    	dic = fopen("./brazilian", "r");
    	if (dic == NULL){
	    	printf("O dicionario nao foi encontrado\n");
	    	exit(1);
	    }
    }

    // aloca a memoria inicial do dicionario
    dicionario = (char**)malloc(10000 * (sizeof(char*)));

    int max = 10000;
    int i = 0;
    // enquanto nao chega ao final do arquivo
    while(!feof(dic)){
    	// le uma linha do dicionario, aloca o espaco dela na estrutura do dicionario
    	fgets(str, 99, dic);
    	str[strlen(str)-1] = 0;
    	dicionario[i] = malloc((strlen(str)+1)*sizeof(char));
    	strcpy(dicionario[i], str);
    	i++;
    	// caso necessario, aloca mais espaco na 
    	if (i >= max)
    		max += 10000;
    		dicionario = realloc(dicionario, max*sizeof(char*));
    }
    *tam = i;
    fclose(dic);
    return dicionario;
}

// retorna nao nulo caso o parametro seja um caractere entre a e z
// maiusculo oou minusculo, acentuado ou nao
// caso contrario, retorna 0
int eh_letra(char caractere){
    return caractere >= 'A' && caractere <= 'Z' || caractere >= 'a' && caractere <= 'z' || caractere >= -64 && caractere <= -1;
}

// retorna nao nulo caso a palavra seja encontrada no dicionario
// retorna 0 caso contrario
// implementado em busca binaria
int palavra_valida(char *palavra, char **dicionario, int tam){
	// confere se eh apenas uma letra
	if ((strlen(palavra) <= 1)){
		if (eh_letra(palavra[0]))
			return 1;
		return 0;
	}
	int inicio = 0;
	int fim = tam;
	int meio = (inicio + fim) / 2;
	// busca binaria
	while(inicio <= fim){
		if(!strcasecmp(dicionario[meio], palavra)){
			return 1;
		}
		if(strcasecmp(dicionario[meio], palavra) < 0){
			inicio = meio + 1;
		}
		else{
			fim = meio - 1;
		}
		meio = (inicio + fim)/2;
	}
	// implementacao em busca sequencial
	// int i = 0;
	// while (i < tam && strcasecmp(dicionario[i], palavra)){
	// 	i++;
	// }
	// return i != tam;
    return 0;    
}

void finaliza_programa(char **dicionario, int tam){
	for (int i = 0; i < tam; ++i)
	{
		free(dicionario[i]);
	}
	free(dicionario);
	return;
}