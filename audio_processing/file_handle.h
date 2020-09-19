// Artur Temporal Coelho GRR20190471 
#ifndef __FILE_HANDLE__
#define __FILE_HANDLE__

#include <inttypes.h>
#include <stdio.h>

// struct padrão de cabeçalho de um arquivo wav
typedef struct wav_header {
    // 
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    // formato
    char sub_chunk1_id[4];
    uint32_t sub_chunk1_size;
    uint16_t audio_format;
    uint16_t number_of_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    // "data"
    char sub_chunk2_id[4];
    uint32_t sub_chunk2_size;
} wav_header_t;

// struct que define os argumentos do usuário
typedef struct arg_data {
    float level;
    float time;
    int safe;
    FILE* input_file;
    FILE** mult_inputs;
    void* output_file;
} arg_data_t;

arg_data_t* get_arg_data(int argc, char **argv);
FILE** get_mult_args(int argc, char **argv, int *num_arq);
wav_header_t* read_header(void* input_file_name);
int get_wav_data(int16_t **data, wav_header_t **wav_header, void* input_name);
int store_wav_data(wav_header_t *wav_header, arg_data_t *arg_data, int16_t *data);
void close_files(arg_data_t* arg_data, int num_arq);
void help_menu();

// definição de códigos de erro 
#define ERR_ARQ_NAO_ENCONTRADO 1
#define ERR_BAD_MALLOC 2
#define ERR_ARG_NAO_ENCONTRADO 3
#define ERR_LEITURA_DADOS 4
#define ERR_CRIACAO_ARQ 5
#define ERR_ESCRITA_ARQ 6
#define ERR_ARG_NUM 7

#define MAX_ARGS 64

#endif