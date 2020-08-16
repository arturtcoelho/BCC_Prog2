#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "file_handle.h"

int main(int argc, char  **argv)
{
    file_data_t *file_data = get_arg_data(argc, argv);
    wav_header_t *wav_header = read_header(file_data->input_file);

    printf("Cabeçalho do arquivo: \n");
    printf("%17s : %.4s\n", "riff tag", wav_header->chunk_id);
    printf("%17s : %d\n", "riff size", wav_header->chunk_size);
    printf("%17s : %.4s\n", "wave tag", wav_header->format);
    printf("%17s : %.4s\n", "form tag", wav_header->sub_chunk1_id);
    printf("%17s : %d\n", "fmt size", wav_header->sub_chunk1_size);
    printf("%17s : %d\n", "audio format", wav_header->audio_format);
    printf("%17s : %d\n", "num channels", wav_header->number_of_channels);
    printf("%17s : %d\n", "sample rate", wav_header->sample_rate);
    printf("%17s : %d\n", "byte rate", wav_header->byte_rate);
    printf("%17s : %d\n", "block align", wav_header->block_align);
    printf("%17s : %d\n", "bits per sample", wav_header->bits_per_sample);
    printf("%17s : %.4s\n", "data tag", wav_header->sub_chunk2_id);
    printf("%17s : %d\n", "data size", wav_header->sub_chunk2_size);
    printf("%17s : %d\n", "samples/channel", wav_header->sub_chunk2_size / (wav_header->number_of_channels * (wav_header->bits_per_sample / 8)));

    return 0;
}
