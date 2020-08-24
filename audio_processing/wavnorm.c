#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

    // descobre o maior valor
    float max = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] > max){
            max = data[i];
        }
    }

    // resolução de erro em caso de argumento com valor inválido
    if (arg_data->level <= 0 || arg_data->level > 1) {
        arg_data->level = DEF_NORM;
        fprintf(stderr, "wavnorm: level invalido ajustado para nor padrão\n");
    }

    // normaliza o volume do arquivo
    float norm = (MAX_16 * arg_data->level) / max;
    for (int i = 0; i < size; i++) {
        data[i] = data[i] * norm;
    }

    store_wav_data(wav_header, arg_data, data);

    return 0;
}
