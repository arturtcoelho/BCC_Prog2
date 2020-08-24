#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "file_handle.h"

int main(int argc, char  **argv)
{
    // le os argumentos passados pelo terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);
    int num_arq;
    arg_data->mult_inputs = get_mult_args(argc, argv, &num_arq);
    
    // le o arquivo passado por argumento
    wav_header_t **wav_headers = malloc(num_arq * sizeof(wav_header_t*));
    int16_t **data = malloc(num_arq * sizeof(int16_t*));

    int total_size = 0, data_size = 0;
    for (int i = 0; i < num_arq; i++)
    {    
        get_wav_data(&data[i], &wav_headers[i], arg_data->mult_inputs[i]);
        data_size += wav_headers[i]->sub_chunk2_size;
        total_size += wav_headers[i]->chunk_size;
        fprintf(stderr, "%d\n", wav_headers[i]->sub_chunk2_size);
    }

    wav_header_t *result_header = wav_headers[0];

    int16_t *result = NULL;
    result = malloc(data_size);

    unsigned long end = 0;
    for (int i = 0; i < num_arq; i++)
    {
        for (int j = 0; j < wav_headers[i]->sub_chunk2_size; j++)
        {
            // fprintf(stderr, "%ld ", end + j);
            result[end + j] = data[i][j];
        }
        fprintf(stderr, "%d\n", wav_headers[i]->sub_chunk2_size);
        end += wav_headers[i]->sub_chunk2_size-11000;
    }

    result_header->chunk_size = total_size;
    result_header->sub_chunk2_size = data_size;
    
    // armazena o vetor no aqruivo
    store_wav_data(result_header, arg_data, result);

    return 0;
}
