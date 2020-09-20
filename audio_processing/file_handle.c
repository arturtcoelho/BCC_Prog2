// Artur Temporal Coelho GRR20190471 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "file_handle.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

// parseamento dos argumentos gerais
arg_data_t* get_arg_data(int argc, char **argv){

    // aloca espaço para as informações de argumento
    arg_data_t *arg_data = malloc(sizeof(arg_data_t));
    if (!arg_data) {
        fprintf(stderr, "Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }
    arg_data->input_file = NULL;
    arg_data->output_file = NULL;
    arg_data->mult_inputs = NULL;
    arg_data->level = 0;
    arg_data->time = 0;
    arg_data->safe = 0;
    int opt;

    // utiliza a função do unistd, getopt para processar as informações dos argumentos
    opterr = 0;
    while ((opt = getopt(argc, argv, "i:o:l:t:s")) != -1){
        switch (opt) {
            case 'i':
                if (!optarg) {
                    fprintf(stderr, "Favor inserir um arquivo de input com a opção -i\n");
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                arg_data->input_file = fopen(optarg, "r");
                if (!arg_data->input_file){
                    fprintf(stderr, "opção -i, arquivo %s não encontrado\n", optarg);
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                break;
            case 'o':
                if (!optarg) {
                    fprintf(stderr, "Favor inserir um arquivo de input com a opção -i\n");
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                arg_data->output_file = fopen(optarg, "w");
                if (!arg_data->output_file){
                    fprintf(stderr, "opção -o, arquivo %s não encontrado\n", optarg);
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                break;
            case 'l':
                if (optarg == NULL){
                    fprintf(stderr, "opção -l, argumento inválido\n");
                    exit(ERR_ARG_NAO_ENCONTRADO);
                }
                arg_data->level = (float)atof(optarg);
                break;
            case 't':
                if (optarg == NULL){
                    fprintf(stderr, "opção -t, argumento inválido\n");
                    exit(ERR_ARG_NAO_ENCONTRADO);
                }
                arg_data->time = (float)atof(optarg);
                break;
            case 's':
                arg_data->safe = 1;
                break;
            default:
                break;
        }
    }

    // caso os arquivos de input e output nao sejam selecionados, é utilizado stdin ou stdout
    if (arg_data->input_file == NULL){
        arg_data->input_file = stdin;
    }

    if (arg_data->output_file == NULL){
        arg_data->output_file = stdout;
    }

    return arg_data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// parseamento de multiplos arquivos
FILE** get_mult_args(int argc, char **argv, int *num_arq){
    // vetor de ponteiros para os nomes de arquivos
    FILE** files = malloc(MAX_ARGS * sizeof(FILE**));
        if (!files) {
        fprintf(stderr, "Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }

    if (argc <= 1){
        fprintf(stderr, "insira ao menos um arquivo\n");
        exit(ERR_ARQ_NAO_ENCONTRADO);
    }

    int num = 0;
    int i = 1;

    while (i < argc){ // para cada argumento
        if (!strcmp(argv[i], "-o"))
            i+=2;
        if (!strcmp(argv[i], "-s"))
            i++;
        // copia os arquivos
        files[num] = fopen(argv[i], "r");
        if (!files[num]) {
            fprintf(stderr, "Arquivo não encontrado\n");
            exit(ERR_ARQ_NAO_ENCONTRADO);
        }

        num++;
        i++;
    }

    *num_arq = num;

    return files;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wav_header_t* read_header(void* input_file){
 
//  aloca memoria para o cabeçalho
    wav_header_t* wav_header = (wav_header_t*)malloc(sizeof(wav_header_t));
    if (wav_header == NULL){
        printf("Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }

// le o cabeçalho
    int read_size = fread(wav_header, sizeof(wav_header_t), 1, input_file);
    if (read_size != 1){
        fprintf(stderr, "Erro de leitura de arquivo\n");
        exit(ERR_LEITURA_DADOS);
    }

    return wav_header;
}

///////////////////////////////////////////////////////////////////////////////////////////////////


int get_wav_data(int16_t **data, wav_header_t **wav_header, void* input_file){

// aloca cabeçalho
    *wav_header = (wav_header_t*)malloc(sizeof(wav_header_t));
    if (*wav_header == NULL){
        fprintf(stderr, "Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }

// le o cabeçalho
    int header_read_size = fread(*wav_header, sizeof(wav_header_t), 1, input_file);
    if (header_read_size != 1){
        fprintf(stderr, "Erro de leitura de arquivo\n");
        exit(ERR_LEITURA_DADOS);
    }


// aloca o espaço de dados
    *data = malloc((*wav_header)->sub_chunk2_size);
    if (*data == NULL){
        fprintf(stderr, "Bad malloc\n");
        exit(ERR_BAD_MALLOC);
    }

// le os dados
    int size = fread(*data, sizeof(int16_t), (*wav_header)->sub_chunk2_size / sizeof(int16_t), input_file);
    
// processamento de erro de leitura
    if (!size){
        fprintf(stderr, "erro de leitura\n");
        exit(ERR_LEITURA_DADOS);
    }

    if (size != (*wav_header)->sub_chunk2_size / sizeof(int16_t)){
        fprintf(stderr, "erro de leitura - resultados inesperados podem ocorrer\n");
    }

    if (input_file != stdin)
        fclose(input_file);

    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int store_wav_data(wav_header_t *wav_header, arg_data_t *arg_data, int16_t *data){

    FILE* output_file = arg_data->output_file;

    if (output_file == NULL){
        fprintf(stderr, "erro de criação de arquivo\n");
        exit(ERR_CRIACAO_ARQ);
    }

// escreve o cabeçalho
    if (!fwrite(wav_header, sizeof(wav_header_t), 1, output_file)){
        fprintf(stderr, "erro de escrita de arquivo\n");
        exit(ERR_ESCRITA_ARQ);
    }

    if (!fwrite(data, sizeof(int16_t), wav_header->sub_chunk2_size / sizeof(int16_t), output_file)) {
        fprintf(stderr, "erro de escrita de arquivo\n");
        exit(ERR_ESCRITA_ARQ);
    }

    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void close_files(arg_data_t* arg_data, int num_arq){
    fclose(arg_data->input_file);
    fclose(arg_data->output_file);
    for (int i = 0; i < num_arq; i++){
        fclose(arg_data->mult_inputs[i]);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void help_menu(){
    fprintf(stderr, "helpmenu\n");
}