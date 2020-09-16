// Artur Temporal Coelho GRR20190471 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "header.h"
#include "file_handle.h"
///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////

// altera a amplitude de cada amostra
void volume_changer(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){

    // cuida do valor do argumento
    if ((arg_data->level <= 0) || (arg_data->level > 10)) {
        arg_data->level = DEF_VOL;
        fprintf(stderr, "wavvol: level invalido ajustado para vol padrão\n");
    }

    // para cada elemento, multiplica sua amplitude pelo valor estabelecido
    int size = wav_header->sub_chunk2_size / sizeof(int16_t);
    int volume;
    for (int i = 0; i < size; i++) {
        volume = data[i] * arg_data->level;
        data[i] = volume < MAX_16 ? volume : MAX_16;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////

// normaliza o audio para um volume próximo do máximo
void volume_normalize(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
    int size = wav_header->sub_chunk2_size / sizeof(int16_t);

    // resolução de erro em caso de argumento com valor inválido
    if (arg_data->level <= 0 || arg_data->level > 1) {
        arg_data->level = DEF_NORM;
        fprintf(stderr, "wavnorm: level invalido ajustado para nor padrão\n");
    }

    // descobre o maior valor
    float max = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] > max){
            max = data[i];
        }
    }

    // normaliza o volume do arquivo
    float norm = (MAX_16 * arg_data->level) / max;
    for (int i = 0; i < size; i++) {
        data[i] = data[i] * norm;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////

// inverte as amostras
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

///////////////////////////////////////////////////////////////////////////////////////////

// realiza o eco do audio por um valor anterior
void echo_maker(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
        
    // cuida do valor do argumento
    if ((arg_data->level <= 0) || (arg_data->level > 1)) {
        arg_data->level = DEF_ECO;
        fprintf(stderr, "wavecho: level invalido ajustado para echo padrão\n");
    }
    if (arg_data->time <= 0) {
        arg_data->time = DEF_DELAY;
        fprintf(stderr, "wavecho: time invalido ajustado para time padrão\n");
    }

    // para cada elemento, adiciona-se o eco correspondente
    int size = wav_header->sub_chunk2_size / sizeof(int16_t);
    int delay = arg_data->time * (float)wav_header->sample_rate / 1000.0;

    int echo_value;
    for (int i = 0; i < size; i++) {
        echo_value = data[i-delay >= 0 ? i-delay : i] * arg_data->level + data[i];
        data[i] = echo_value < MAX_16 ? echo_value : MAX_16;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////

// concatena os arquivos de audio
void concatenate(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final){

    int total_data = 0;
    *wav_header_final = malloc(sizeof(wav_header_t));

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
    }

    // concatenação de dados de multiplos arquivos
    memcpy(*wav_header_final, wav_headers[0], sizeof(wav_header_t));

    *data_final = malloc(total_data * sizeof(int16_t));

    int displacement = 0;
    for (int i = 0; i < num_arq; i++)
    {   
        if ((*wav_header_final)->sample_rate != wav_headers[i]->sample_rate){
            fprintf(stderr, "AVISO: Diferença entre taxa de amostra, resultados podem ser diferente do esperado\n");
        }
        memcpy((*data_final + displacement), data[i], wav_headers[i]->sub_chunk2_size);
        displacement += wav_headers[i]->sub_chunk2_size / 2;
    }

    (*wav_header_final)->sub_chunk2_size = total_data;
    (*wav_header_final)->chunk_size = total_data + DIFF_DATA_SIZE;

}

///////////////////////////////////////////////////////////////////////////////////////////

// amplifica o efeito estereo realizando um eco entre os canais
void amplified_stereo(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
    // cuida do valor do argumento
    if ((arg_data->level <= 0) || (arg_data->level > 10)) {
        arg_data->level = DEF_DIFF;
        fprintf(stderr, "wavwide: level invalido ajustado para diff padrão\n");
    }

    // faz a alteração de eco estereo
    int diff;
    for (int i = 0; i < wav_header->sub_chunk2_size / sizeof(int16_t); i++)
    {
        // fprintf(stderr, "%d, %d\n", data[i], data[i+1]);
        diff = data[i]-data[i+1];
        data[i] = data[i] + (diff * arg_data->level);
        data[i+1] = data[i+1] - (diff * arg_data->level);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

// mistura as amostras de audio
void mixer(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final){

    // leitura de dados de multiplos arquivos
    int total_data = 0;
    *wav_header_final = malloc(sizeof(wav_header_t));

    for (int i = 0; i < num_arq; i++)
    {
        if (arg_data->mult_inputs[i])
            get_wav_data(&(data[i]), &(wav_headers[i]), arg_data->mult_inputs[i]);
        else {
            fprintf(stderr, "Erro de leitura de arquivo\n");
            exit(ERR_LEITURA_DADOS);
        }
        total_data = wav_headers[i]->sub_chunk2_size > total_data ? wav_headers[i]->sub_chunk2_size : total_data;
    }

    // concatenação de dados de multiplos arquivos
    memcpy(*wav_header_final, wav_headers[0], sizeof(wav_header_t));

    (*wav_header_final)->sub_chunk2_size = total_data;
    (*wav_header_final)->chunk_size = total_data + DIFF_DATA_SIZE;

    *data_final = malloc(total_data * sizeof(int16_t));

    for (int i = 0; i < num_arq; i++)
    {  
        if ((*wav_header_final)->sample_rate != wav_headers[i]->sample_rate){
            fprintf(stderr, "AVISO: Diferença entre taxa de amostra, resultados podem ser diferente do esperado\n");
        }
        for (int j = 0; j < wav_headers[i]->sub_chunk2_size / sizeof(int16_t); j++) {
            (*data_final)[j] += data[i][j];
        }
        
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

// extrai um canal de audio de um arquivo com multiplos canais
void channel_extractor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t** data){
    int channels = wav_header->number_of_channels;
    if (channels == 1)
        return;
    wav_header->number_of_channels = 1;
    wav_header->byte_rate = wav_header->byte_rate / channels;
    wav_header->block_align = wav_header->block_align / channels;
    int size = wav_header->sub_chunk2_size;
    wav_header->sub_chunk2_size = wav_header->sub_chunk2_size / channels;
    wav_header->chunk_size = wav_header->sub_chunk2_size + DIFF_DATA_SIZE;

    int16_t *data_final = malloc(wav_header->sub_chunk2_size);
    if (data_final == NULL){
        fprintf(stderr, "Erro de alocação\n");
        exit(ERR_BAD_MALLOC);
    }

    for (int i = 0; i < size/sizeof(int16_t); i += channels){
        data_final[i/2] = (*data)[i];
    }

    free(*data);
    *data = data_final;
}

///////////////////////////////////////////////////////////////////////////////////////////

// modifica a taxa de amostragem do arquivo
void frequency_modifier(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
    if (arg_data->level <= 0 || arg_data->level >= 10){
        arg_data->level = DEF_FREQ;
        fprintf(stderr, "wavfreq: level invalido ajustado para freq padrão\n");
    }
    wav_header->sample_rate = wav_header->sample_rate * arg_data->level;
}
