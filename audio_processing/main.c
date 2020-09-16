#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "file_handle.h"
#include "data_handler.h"

//////////////////////////////////////////////////////////////////////

// fornece o codigo definido para os filtros
int get_filter_code(char* filter_name){
    // refina o argumento do nome do arquivo
    for (int i = strlen(filter_name)-1; i >= 0; i--){
        if (filter_name[i] == '/')
            filter_name = filter_name + i + 1;
    }

    // "swich" para as strings de nomes dos filtros e seus respectivos códigos
    if (!strcmp(filter_name, "main")){
        help_menu();
        exit(0);
    } else if (!strcmp(filter_name, "wavinfo")) {
        return WAVINFO;
    } else if (!strcmp(filter_name, "wavvol")) {
        return WAVVOL;
    } else if (!strcmp(filter_name, "wavnorm")) {
        return WAVNORM;
    } else if (!strcmp(filter_name, "wavecho")) {
        return WAVECHO;
    } else if (!strcmp(filter_name, "wavrev")) {
        return WAVREV;
    } else if (!strcmp(filter_name, "wavwide")) {
        return WAVWIDE;
    } else if (!strcmp(filter_name, "wavext")) {
        return WAVEXT;
    } else if (!strcmp(filter_name, "wavfreq")) {
        return WAVFREQ;
    } else if (!strcmp(filter_name, "wavmix")) {
        return WAVMIX;
    } else if (!strcmp(filter_name, "wavcat")) {
        return WAVCAT;
    } else {
        fprintf(stderr, "Erro no nome do executavel\n");
        exit(EXEC_NAME_ERR);
    }
}

//////////////////////////////////////////////////////////////////////

// confere o tipo de filtro quanto ao nº de inputs 
int single_input(int code){
    return code < 10;
}

//////////////////////////////////////////////////////////////////////

int main(int argc, char  **argv)
{
    fprintf(stderr, "main: %s\n", argv[0]);
    int filter_code = get_filter_code(argv[0]);

    // le os argumentos passados pelo terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);

    if (single_input(filter_code)){
        // le o arquivo passado por argumento
        wav_header_t *wav_header = NULL;
        int16_t *data = NULL;
        get_wav_data(&data, &wav_header, arg_data->input_file);

        switch (filter_code) {
            case WAVINFO:
                print_wav_info(wav_header);
                break;
            case WAVVOL:
                volume_changer(arg_data, wav_header, data);
                break;
            case WAVNORM:
                volume_normalize(arg_data, wav_header, data);
                break;
            case WAVECHO:
                echo_maker(arg_data, wav_header, data);
                break;
            case WAVREV:
                reversor(arg_data, wav_header, data);
                break;
            case WAVWIDE:
                amplified_stereo(arg_data, wav_header, data);
                break;
            case WAVEXT:
                channel_extractor(arg_data, wav_header, &data);
                break;
            case WAVFREQ:
                frequency_modifier(arg_data, wav_header, data);
                break;
        }

        // armazena o vetor no aqruivo
        if (filter_code != WAVINFO)
            store_wav_data(wav_header, arg_data, data);
        close_files(arg_data, 0);
    
        // libera a memória
        free(arg_data);
        free(wav_header);
        free(data);

    } else {
        // le os arquivos passados por argumento
        int num_arq;
        arg_data->mult_inputs = get_mult_args(argc, argv, &num_arq);

        if (num_arq > MAX_ARGS){
            fprintf(stderr, "Favor insira menos argumentos\n");
            exit(ERR_ARG_NUM);
        }

        // vetores de dados dos arquivos
        wav_header_t **wav_headers = malloc(num_arq * sizeof(wav_header_t*));
        int16_t **data = malloc(num_arq * sizeof(int16_t*));

        wav_header_t* wav_header_final;
        int16_t* data_final;

        switch (filter_code) {
            case WAVCAT:
                concatenate(arg_data, wav_headers, data, num_arq, &wav_header_final, &data_final);
                break;
            case WAVMIX:
                mixer(arg_data, wav_headers, data, num_arq, &wav_header_final, &data_final);
                break;
        }
    
        // armazena o vetor no aqruivo
        store_wav_data(wav_header_final, arg_data, data_final);
        close_files(arg_data, num_arq);

        // libera a memória
        free(arg_data);
        for (int i = 0; i < num_arq; i++){
            free(wav_headers[i]);
            free(data[i]);
        }
    }
    
    return 0;
}
