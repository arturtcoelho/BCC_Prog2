#include <stdio.h>
#include <stdlib.h>

#include "header.h"

int main(int argc, char const *argv[])
{
    wav_header_t *wav_header = (wav_header_t*)malloc(sizeof(wav_header_t));

    FILE* wav_file = NULL;

    if (argc > 1)
        wav_file = fopen(argv[1], "r");
    if (wav_file == NULL){
        printf("Arquivo: %s não encontrado\n", argv[1]);
        exit(69);
    }

    fread(wav_header, sizeof(wav_header_t), 1, wav_file);
    
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
