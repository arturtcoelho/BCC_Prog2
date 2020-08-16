#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10000000

// void quick_sort(long int values[], int began, int end)
// {
// 	int i, j, pivo, aux;
// 	i = began;
// 	j = end-1;
// 	pivo = values[(began + end) / 2];
// 	while(i <= j)
// 	{
// 		while(values[i] < pivo && i < end) i++;
// 		while(values[j] > pivo && j > began) j--;
// 		if(i <= j)
// 		{
// 			aux = values[i];
// 			values[i] = values[j];
// 			values[j] = aux;
// 			i++;
// 			j--;
// 		}
// 	}
// 	if(j > began)
// 		quick_sort(values, began, j+1);
// 	if(i < end)
// 		quick_sort(values, i, end);
// }

void quick_sort(long int* arr, int beg, int end){
    int i = beg, j = end - 1, mid = beg + end / 2;
    int aux;

    while(i <= j) {
        while(arr[i] < mid && i < end) i++;
        while(arr[j] > mid && j > beg) j--;
        if (i <= j){
            aux = arr[i];
            arr[i] = arr[j];
            arr[j] = aux;
            i++;
            j--;
        }
    }
    if(j > beg)
        quick_sort(arr, beg, j+1);
    if(i < end)
        quick_sort(arr, i, end);
}

// escreve 10M longints em um arquivo
int main()
{

// inicializa ponteiros
    FILE* long_int_f;
    long int *long_int_a = (long int*)malloc(SIZE * sizeof(long int));

// abre e le o arquivo em long_int_a
    long_int_f = fopen("./long_integers", "r");
    fread(long_int_a, sizeof(long int), SIZE, long_int_f);
    
    quick_sort(long_int_a, 0, SIZE);

// escreve no arquivo em disco
    fwrite(long_int_a, sizeof(long int), SIZE, long_int_f);

// libera a memoria
    free(long_int_a);
    fclose(long_int_f);
    return 0;
}
