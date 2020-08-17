#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "file_handle.h"

int main(int argc, char  **argv)
{
    // parse dos argumentos do terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);

    // fetch das informações do arquivo de entrada
    wav_header_t *wav_header = NULL;
    int16_t *data = NULL;
    get_wav_data(&data, &wav_header, arg_data);

    int size = wav_header->sub_chunk2_size / sizeof(int16_t);

    // inverte o vetor, invertendo a musica
    int aux;
    for (int i = 0; i < size / 2; i++) {
        aux = data[i];
        data[i] = data[size-i];
        data[size-i] = aux;
    }

    // armazena o vetor no aqruivo
    store_wav_data(wav_header, arg_data, data);

    return 0;
}
