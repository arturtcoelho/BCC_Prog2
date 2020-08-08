#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	FILE *fp;

	// abre e testa  arquivo
	fp = fopen("vestiba.csv", "r");

	if(fp == NULL){
		printf("Não foi achado o arquivo\n");
		exit(1);
	}

	// acumuladores de dados
	float *notas;
	int num = 0;
	float nota;
	float maior = 0;
	float menor = 100;
	float media = 0;
	float desvio = 0;

	// strings de apoio
	char line[1000];
	char *palavra;
	char curso[100];
	char cursomaior[100];
	char cursomenor[100];

	// Descobre quantas linhas ha no arquivo e aloca um veto
	fseek(fp, 0, SEEK_END);
	notas = (float*)malloc(ftell(fp) * sizeof(float));
    fseek(fp, 0, SEEK_SET);

    // enquanto nao chega no fim do arquivo
	while(!feof(fp)){
		// le uma linha
		fgets(line, 999, fp);
		curso[0] = 0;
		palavra = strtok(line, "  ");
		
		// le o curso
		while (!isdigit(palavra[0])){
			strcat(curso, palavra);
			strcat(curso, " ");
			palavra = strtok(NULL, "  ");
		}
		// ignora os proximos dados
		palavra = strtok(NULL, "  ");
		palavra = strtok(NULL, "  ");
		
		// transforma a string em float
		nota = atof(palavra);
		// define a maior e menor nota, assim
		// como o curso com maior e menor nota
		if (menor >= nota){
			strcpy(cursomenor, curso);
			menor = nota;
		}
		if (maior <= nota){
			strcpy(cursomaior, curso);
			maior = nota;
		}
		notas[num] = nota;
		num++;

		// printf("%s", curso);
		// printf("%f\n", notas);
	}
	// calcula a media
	for (int i = 0; i < num; ++i){
		media += notas[i];
	}
	media = media / num;
	// calcula o desvio padrao
	for (int i = 0; i < num; ++i)
	{
		desvio += (notas[i]-media)*(notas[i]-media);
	}
	desvio = desvio / num;
	desvio = sqrt(desvio);

	// imprime os dados
	printf("O curso com menor nota (%d) eh: %s\n", (int)menor, cursomenor);
	printf("O curso com maior nota (%d) eh: %s\n", (int)maior, cursomaior);
	printf("A media eh: %f\n", media);
	printf("O desvio padrao eh: %f\n", desvio);

	// libera o ponteiro e o vetor alocado
	fclose(fp);
	free(notas);

	return 0;
}