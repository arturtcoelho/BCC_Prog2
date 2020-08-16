#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "header.h"
#include "file_handle.h"

file_data_t* get_arg_data(int argc, char **argv){

    file_data_t *file_data = malloc(sizeof(file_data_t));
    
    int opt;

    opterr = 0;
    while ((opt = getopt(argc, argv, "i:o:l:t:")) != -1){
        switch (opt) {
            case 'i':
                file_data->input_file = malloc(strlen(optarg));
                strcpy(file_data->input_file, optarg);
                if (file_data->input_file == NULL){
                    printf("opção -i, arquivo %s não encontrado\n", optarg);
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                break;
            case 'o':
                file_data->output_file = malloc(strlen(optarg));
                strcpy(file_data->output_file, optarg);
                if (file_data->output_file == NULL){
                    printf("opção -o, arquivo %s não encontrado\n", optarg);
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                break;
            case 'l':
                if (optarg == NULL){
                    printf("opção -l, argumento inválido\n");
                    exit(ERR_ARG_NAO_ENCONTRADO);
                }
                file_data->level = (float)atof(optarg);
                break;
            case 't':
                if (optarg == NULL){
                    printf("opção -t, argumento inválido\n");
                    exit(ERR_ARG_NAO_ENCONTRADO);
                }
                file_data->time = (float)atof(optarg);
                break;
            default:
                break;
        }
    }

    if (file_data->input_file == NULL){
        printf("Utilizando stdin como entrada\n");
        file_data->input_file = stdin;
    }

    if (file_data->output_file == NULL){
        printf("Utilizando stdout como saida\n");
        file_data->output_file = stdout;
    }

    return file_data;
}

wav_header_t* read_header(char *input_file_name){
    FILE* input_file = fopen(input_file_name, "r");
 
//  aloca memoria para o header
    wav_header_t* wav_header = (wav_header_t*)malloc(sizeof(wav_header_t));
    if (wav_header == NULL){
        printf("Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }

    if (input_file == NULL){
        printf("arquivo de input invalido: %p\n", input_file);
        exit(ERR_ARQ_NAO_ENCONTRADO);
    }

// le o cabeçalho
    fread(wav_header, sizeof(wav_header_t), 1, input_file);

    fclose(input_file);

    return wav_header;
}

int16_t* get_wav_data(wav_header_t *wav_header, file_data_t *file_data){
    FILE* input_file = fopen(file_data->input_file, "r");

    int16_t *data = malloc(wav_header->sub_chunk2_size);
    if (data == NULL){
        printf("Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }

    fseek(input_file, sizeof(wav_header_t), SEEK_SET);

    int size;
    size = fread(data, sizeof(int16_t), wav_header->sub_chunk2_size / sizeof(int16_t), input_file);
    
    if (!size){
        printf("erro de leitura\n");
        exit(ERR_LEITURA_DADOS);
    }

    if (size != wav_header->sub_chunk2_size / sizeof(int16_t)){
        printf("erro de leitura\nresultados inesperados podem ocorrer\n");
    }

    fclose(input_file);

    return data;
}

int store_wav_data(wav_header_t *wav_header, file_data_t *file_data, int16_t *data){

    FILE* output_file = fopen(file_data->output_file, "w+");

    if (output_file == NULL){
        printf("erro de criação de arquivo\n");
        exit(ERR_CRIACAO_ARQ);
    }

    if (!fwrite(wav_header, sizeof(wav_header_t), 1, output_file)){
        printf("erro de escrita de arquivo\n");
        exit(ERR_ESCRITA_ARQ);
    }
    if (!fwrite(data, sizeof(int16_t), wav_header->sub_chunk2_size / sizeof(int16_t), output_file)) {
        printf("erro de escrita de arquivo\n");
        exit(ERR_ESCRITA_ARQ);
    }

    fclose(output_file);
    return 1;
}