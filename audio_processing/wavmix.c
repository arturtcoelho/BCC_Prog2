#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "file_handle.h"
#include "data_handler.h"

int main(int argc, char  **argv)
{
    // le os argumentos passados pelo terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);
    int num_arq;
    arg_data->mult_inputs = get_mult_args(argc, argv, &num_arq);

    if (num_arq > MAX_ARGS){
        fprintf(stderr, "Favor insira menos argumentos\n");
        exit(ERR_ARG_NUM);
    }

    // vetores de dados dos arquivos
    wav_header_t **wav_headers = malloc(num_arq * sizeof(wav_header_t*));
    int16_t **data = malloc(num_arq * sizeof(int16_t*));
    
    wav_header_t* wav_header_final;
    int16_t* data_final;

    mixer(arg_data, wav_headers, data, num_arq, &wav_header_final, &data_final);
    
    // armazena o vetor no aqruivo
    store_wav_data(wav_header_final, arg_data, data_final);

    return 0;
}
