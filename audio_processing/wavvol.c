#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "file_handle.h"

int main(int argc, char  **argv)
{
    file_data_t *file_data = get_arg_data(argc, argv);
    wav_header_t *wav_header = read_header(file_data->input_file);

    int16_t *data = get_wav_data(wav_header, file_data);

    for (int i = 0; i < wav_header->sub_chunk2_size / 16; i++) {
        data[i] = data[i] * file_data->level;
    }

    store_wav_data(wav_header, file_data, data);
    printf("fim\n");

    return 0;
}
