#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "file_handle.h"

int main(int argc, char  **argv)
{
    // le os argumentos passados pelo terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);
    
    // le o arquivo passado por argumento
    wav_header_t *wav_header = NULL;
    int16_t *data = NULL;
    get_wav_data(&data, &wav_header, arg_data->input_file);
    
    // cuida do valor do argumento
    if ((arg_data->level <= 0) || (arg_data->level > 10)) {
        arg_data->level = DEF_VOL;
        fprintf(stderr, "wavvol: level invalido ajustado para vol padrão\n");
    }

    // para cada elemento, multiplica sua amplitude pelo valor estabelecido
    int size = wav_header->sub_chunk2_size / sizeof(int16_t);
    for (int i = 0; i < size; i++) {
        data[i] = data[i] * arg_data->level;
    }

    // armazena o vetor no aqruivo
    store_wav_data(wav_header, arg_data, data);

    return 0;
}
