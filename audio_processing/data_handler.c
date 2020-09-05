#include <stdio.h>
#include <stdlib.h>

#include "header.h"

// impressão de dados do cabeçalho
void print_wav_info(wav_header_t* wav_header){
    if (!wav_header) {
        fprintf(stderr, "Erro com o cabeçalho do arquivo\n");
        exit(DATA_ERR);
    }

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

}

void volume_changer(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){

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

}

void volume_normalize(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
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

}

void reversor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
    int size = wav_header->sub_chunk2_size / sizeof(int16_t);

    // inverte o vetor, invertendo a musica
    int aux;
    for (int i = 0; i < size / 2; i++) {
        aux = data[i];
        data[i] = data[size-i];
        data[size-i] = aux;
    }
}