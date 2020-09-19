// Artur Temporal Coelho GRR20190471 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_handler.h"
#include "file_handle.h"

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

// define os padroes de segurança
#define ECHO_SAFE 1.25
#define WIDE_SAFE 2.5

//////////////////////////////////////////////////////////////////////////////

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
    // define qual filtro foi passado por link
    int filter_code = get_filter_code(argv[0]);

    // le os argumentos passados pelo terminal
    arg_data_t *arg_data = get_arg_data(argc, argv);
    int safe = arg_data->safe;

    int16_t max;

    if (single_input(filter_code)){
        //////////////////////////////////////////////////////////////////////////
        // filtros que aceitam apenas um input como argumento ou entrada padrão //
        //////////////////////////////////////////////////////////////////////////

        // le o arquivo de input passado por argumento ou stdin
        wav_header_t *wav_header = NULL;
        int16_t *data = NULL;
        get_wav_data(&data, &wav_header, arg_data->input_file);

        // realiza a operação de filtro
        switch (filter_code) {
            case WAVINFO: // inprime info
                print_wav_info(wav_header);
                break;

            case WAVVOL: // muda o volume
                max = max_value(wav_header, data);
                if (safe &&  max * arg_data->level > MAX_16){
                    arg_data->level = (float)max / MAX_16;
                    fprintf(stderr, "Valor de entrada assegurado\n");
                }
                volume_changer(arg_data, wav_header, data);
                break;

            case WAVNORM: // normaliza
                volume_normalize(arg_data, wav_header, data);
                break;

            case WAVECHO: // efeito eco
                max = max_value(wav_header, data);
                if (safe && max + max * arg_data->level > MAX_16){
                    arg_data->level = (float)max / (MAX_16  * ECHO_SAFE);
                    fprintf(stderr, "Valor de entrada assegurado %f\n", arg_data->level);
                }
                echo_maker(arg_data, wav_header, data);
                break;

            case WAVREV: // reverte
                reversor(arg_data, wav_header, data);
                break;

            case WAVWIDE: // estereo amplificado
                max = max_value(wav_header, data);
                if (safe && max + max * arg_data->level > MAX_16){
                    arg_data->level = (float)max / (MAX_16 * WIDE_SAFE);
                    fprintf(stderr, "Valor de entrada assegurado %f\n", arg_data->level);
                }
                amplified_stereo(arg_data, wav_header, data);
                break;

            case WAVEXT: // extrai o canal
                channel_extractor(arg_data, wav_header, &data);
                break;

            case WAVFREQ: // muda a frequencia da amostra
                frequency_modifier(arg_data, wav_header, data);
                break;
        }

        // armazena o vetor no aqruivo
        if (filter_code != WAVINFO)
            store_wav_data(wav_header, arg_data, data);
    
        // libera a memória
        close_files(arg_data, 0);
        free(wav_header);
        free(data);

    } else {

        ////////////////////////////////////////////////////////////////////////////
        // tipos de filtros que aceitam multiplos inputs como argumentos          //
        ////////////////////////////////////////////////////////////////////////////

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

        // header e vetor de somatorio final
        wav_header_t* wav_header_final = malloc(sizeof(wav_header_t));
        int16_t* data_final;

        int total_data = 0;
        int max_data = 0;

        // leitura de dados de multiplos arquivos
        for (int i = 0; i < num_arq; i++)
        {
            if (arg_data->mult_inputs[i])
                get_wav_data(&(data[i]), &(wav_headers[i]), arg_data->mult_inputs[i]);
            else {
                fprintf(stderr, "Erro de leitura de arquivo\n");
                exit(ERR_LEITURA_DADOS);
            }
            total_data += wav_headers[i]->sub_chunk2_size;
            if (max_data < wav_headers[i]->sub_chunk2_size)
                max_data = wav_headers[i]->sub_chunk2_size;
        }
        
        // define os valores padroes de cabeçalho
        memcpy(wav_header_final, wav_headers[0], sizeof(wav_header_t));

        data_final = malloc(total_data * sizeof(int16_t));

        switch (filter_code) {
            case WAVCAT: // contcatena os arquivos
                concatenate(arg_data, wav_headers, data, num_arq, &wav_header_final, &data_final, total_data);
                wav_header_final->sub_chunk2_size = total_data;
                wav_header_final->chunk_size = total_data + DIFF_DATA_SIZE;
                break;

            case WAVMIX: // mistura os arquivos
                if (safe){
                    arg_data->level = 1.0 / num_arq;
                    for (int i = 0; i < num_arq; i++){
                        volume_changer(arg_data, wav_headers[i], data[i]);
                    }
                    fprintf(stderr, "Valor de entrada assegurado %f\n", arg_data->level);
                }
                mixer(arg_data, wav_headers, data, num_arq, &wav_header_final, &data_final, max_data);
                wav_header_final->sub_chunk2_size = max_data;
                wav_header_final->chunk_size = max_data + DIFF_DATA_SIZE;
                break;
        }
    
        // armazena o header e vetor final no aqruivo
        store_wav_data(wav_header_final, arg_data, data_final);

        // libera a memória de todos os headers e vetores
        close_files(arg_data, num_arq);
        for (int i = 0; i < num_arq; i++){
            free(wav_headers[i]);
            free(data[i]);
        }
        if (!arg_data->mult_inputs)
            free(arg_data->mult_inputs);
    }

    free(arg_data);
    
    return 0;
}
