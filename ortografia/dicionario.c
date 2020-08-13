// GRR 20190471 Artur Temporal Coelho

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include "dicionario.h"

// para utilizar o q sort é necessario fazer o casting dos 
// tipos (unsigned char *) para (void *)  
int string_cmp(const void *a, const void *b) 
{ 
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
} 

// carrega o dicionario, de seu arquivo original para a 
// estrutura alocada dinamicamente em memoria
// retorna o ponteiro para a estrutura e o tamanho total dela
unsigned char **carregar_dicionario(int *tam){
    
    FILE *dic;
    unsigned char str[100];
    unsigned char **dicionario;
    
    // abre o dicionario e sai do programa caso o mesmo nao exista
    dic = fopen("./brazilian", "r");
    if (dic == NULL){
    	dic = fopen("/usr/share/dict/brazilian", "r");
    	if (dic == NULL){
	    	printf("O dicionario nao foi encontrado\n");
	    	exit(1);
	    }
    }

    // aloca a memoria inicial do dicionario
    dicionario = (unsigned char**)malloc(10000 * (sizeof(unsigned char*)));

    int max = 10000;
    int i = 0;

    // enquanto nao chega ao final do arquivo
    while(!feof(dic)){
    	// le uma linha do dicionario, aloca o espaco dela na estrutura do dicionario
    	fgets((char *)str, 99, dic);
    	str[strlen((char *)str)-1] = 0;
    	dicionario[i] = malloc((strlen((char *)str)+1)*sizeof(unsigned char));

		// passa a palavra para letra minúscula
		for (int i = 0; str[i]; i++)
			str[i] = tolower(str[i]);
		
		// copia a palavra para o array
    	strcpy((char *)dicionario[i], (char *)str);
    	i++;

    	// caso necessario, aloca mais espaco na memória
    	if (i >= max){
    		max += 10000;
    		dicionario = realloc(dicionario, max*sizeof(unsigned char*));
		}
    }

	qsort(dicionario, i, sizeof(unsigned char *), string_cmp);
    *tam = i;
    fclose(dic);

	// retorna o dicionario ordenado
    return dicionario;
}

// retorna nao nulo caso a palavra seja encontrada no dicionario
// retorna 0 caso contrario
// implementado em busca binaria
int palavra_valida(unsigned char *palavra, unsigned char **dicionario, int tam){

	// esse confere se uma palavra consiste de apenas um caractere, 
	// caso essa palavra seja um caractere alfabetico é considerado válido
	// e portanto não é marcado entre colchetes

	// if ((strlen((char *)palavra) <= 1)){
	// 	if (isalpha(palavra[0]))
	// 		return 1;
	// 	return 0;
	// }

	// busca binaria
	int inicio = 0;
	int fim = tam;
	int meio = (inicio + fim) / 2;
	while(inicio <= fim){
		// achou a palavra
		if(!strcasecmp((char *)dicionario[meio], (char *)palavra)){
			return 1;
		}
		if(strcasecmp((char *)dicionario[meio], (char *)palavra) < 0){
			inicio = meio + 1;
		}
		else{
			fim = meio - 1;
		}
		meio = (inicio + fim)/2;
	}

	// palavra não encontrada
    return 0;    
}

// libera o espaço de memoria alocado dinamicamente
void finaliza_programa(unsigned char **dicionario, int tam){
	for (int i = 0; i < tam; ++i)
	{
		free(dicionario[i]);
	}
	free(dicionario);
	return;
}