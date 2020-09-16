// Artur Temporal Coelho GRR20190471 
#ifndef __HEADER__
#define __HEADER__

// definição de códigos de erro 
#define ERR_ARQ_NAO_ENCONTRADO 1
#define ERR_BAD_MALLOC 2
#define ERR_ARG_NAO_ENCONTRADO 3
#define ERR_LEITURA_DADOS 4
#define ERR_CRIACAO_ARQ 5
#define ERR_ESCRITA_ARQ 6
#define ERR_ARG_NUM 7
#define ERR_SAMPLE 8
#define DATA_ERR 100
#define EXEC_NAME_ERR 200

// valor máximo de int16_t utilizado para comparação
#define MAX_16 31129.0

// valores padrão caso não haja input do usuário
#define DEF_NORM 0.95
#define DEF_VOL 1
#define DEF_ECO 0.5
#define DEF_DELAY 1000
#define DEF_DIFF 0.5
#define DEF_FREQ 1
#define DEF_VIB 1

// define valores máximos de uso
#define MAX_ARGS 64
#define DIFF_DATA_SIZE 36

// define os códigos dos filtros
#define WAVINFO 1
#define WAVVOL 2
#define WAVNORM 3
#define WAVECHO 4
#define WAVREV 5
#define WAVWIDE 6
#define WAVEXT 7 
#define WAVFREQ 8
#define WAVMIX 11
#define WAVCAT 12

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
    FILE* input_file;
    FILE** mult_inputs;
    void* output_file;
} arg_data_t;

#endif