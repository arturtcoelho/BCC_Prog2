#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "file_handle.h"
#include "data_handler.h"

int main(int argc, char  **argv)
{
    // parse dos argumentos do terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);

    // fetch das informações do arquivo de entrada
    wav_header_t *wav_header = NULL;
    int16_t *data = NULL;
    get_wav_data(&data, &wav_header, arg_data->input_file);

    reversor(arg_data, wav_header, data);

    // armazena o vetor no aqruivo
    store_wav_data(wav_header, arg_data, data);

    return 0;
}
