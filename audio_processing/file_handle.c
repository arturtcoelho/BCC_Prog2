#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"
#include "file_handle.h"

int get_files(int argc, char **argv, FILE** input_file, FILE** output_file){
    int opt;

    opterr = 0;
    while ((opt = getopt(argc, argv, "i:o:")) != -1){
        switch (opt) {
            case 'i':
                *input_file = fopen(optarg, "r");
                if (*input_file == NULL){
                    printf("opção -i, arquivo %s não encontrado\n", optarg);
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                break;
            case 'o':
                *output_file = fopen(optarg, "r");
                if (*output_file == NULL){
                    printf("opção -o, arquivo %s não encontrado\n", optarg);
                    exit(ERR_ARQ_NAO_ENCONTRADO);
                }
                break;
            default:
                printf("argumento não aceito: %c", opt);
                break;
        }
    }

    if (*input_file == NULL){
        printf("Utilizando stdin como entrada\n");
        *input_file = stdin;
    }

    if (*output_file == NULL){
        printf("Utilizando stdout como saida\n");
        *output_file = stdout;
    }

    return 1;
}

wav_header_t* read_header(FILE* input_file){
 
//  aloca memoria para o header
    wav_header_t* wav_header = (wav_header_t*)malloc(sizeof(wav_header_t));
    if (wav_header == NULL){
        printf("Bad malloc");
        exit(ERR_BAD_MALLOC);
    }

    if (input_file == NULL){
        printf("arquivo de input invalido: %p\n", input_file);
        exit(ERR_ARQ_NAO_ENCONTRADO);
    }

// le o cabeçalho
    fread(wav_header, sizeof(wav_header_t), 1, input_file);

    return wav_header;
}