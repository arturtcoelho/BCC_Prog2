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
    get_wav_data(&data, &wav_header, arg_data);
    
    // cuida do valor do argumento
    if ((arg_data->level <= 0) || (arg_data->level > 1)) {
        arg_data->level = DEF_ECO;
        fprintf(stderr, "wavecho: level invalido ajustado para echo padrão\n");
    }
    if (arg_data->time <= 0) {
        arg_data->time = DEF_DELAY;
        fprintf(stderr, "wavecho: time invalido ajustado para time padrão\n");
    }

    // para cada elemento, adiciona-se o eco correspondente
    int size = wav_header->sub_chunk2_size / sizeof(int16_t);
    int delay = arg_data->time * (float)wav_header->sample_rate / 1000.0;

    int echo_value;
    for (int i = 0; i < size; i++) {
        echo_value = data[i-delay >= 0 ? i-delay : i] * arg_data->level + data[i];
        data[i] = echo_value < MAX_16 ? echo_value : MAX_16;
    }

    // armazena o vetor no aqruivo
    store_wav_data(wav_header, arg_data, data);

    return 0;
}
