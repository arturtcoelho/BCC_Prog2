#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "header.h"
#include "file_handle.h"

float vibrato_sen(int i){
    return sin(i/20) / 2 + 1;
}

// int16_t* upscale_samples(wav_header_t* wav_header_src,wav_header_t* wav_header_dest, int16_t *data){
//     int m_factor = wav_header_dest->sample_rate / wav_header_src->sample_rate;
//     fprintf(stderr, "%d\n", m_factor);
//     int i;
//     int16_t *data_final = malloc(wav_header_dest->sub_chunk2_size * sizeof(int16_t));
//     for (i = 0; i < wav_header_dest->sub_chunk2_size / sizeof(int16_t); i+=(m_factor+1)){
//         data_final[i] = data[i];
//         for(int j = 0; j < m_factor; j++){
//             data_final[i+j] = data[i];
//         }
//     }
//     return data_final;
// }


// int16_t* downscale_samples(wav_header_t* wav_header_src,wav_header_t* wav_header_dest, int16_t *data, int *m_factor){
//     *m_factor = wav_header_src->sample_rate / wav_header_dest->sample_rate;
//     int16_t *data_final = malloc(wav_header_src->sub_chunk2_size);

//     for (int i = 0; i < wav_header_src->sub_chunk2_size / 8; i++){
//         for (int j = 0; j < *m_factor; j++){
//             data_final[i+j] = data[i * 4];
//         }
//     }
//     fprintf(stderr, "aloco chegou aqui\n");

//     return data_final;
// }

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

///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////

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

void concatenate(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final){

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
        total_data += wav_headers[i]->sub_chunk2_size;
    }

    // concatenação de dados de multiplos arquivos
    memcpy(*wav_header_final, wav_headers[0], sizeof(wav_header_t));

    *data_final = malloc(total_data * sizeof(int16_t));

    int displacement = 0;
    for (int i = 0; i < num_arq; i++)
    {   
        // if ((*wav_header_final)->sample_rate != wav_headers[i]->sample_rate){
        //     int m_factor;
        //     data[i] = channel_extractor(wav_headers[i], data[i]);
        //     data[i] = downscale_samples(wav_headers[i], *wav_header_final, data[i], &m_factor);
        //     wav_headers[i]->sample_rate = (*wav_header_final)->sample_rate;
        //     // total_data -= wav_headers[i]->sub_chunk2_size - wav_headers[i]->sub_chunk2_size / m_factor;
        //     wav_headers[i]->sub_chunk2_size = wav_headers[i]->sub_chunk2_size / m_factor;
        // }
        memcpy((*data_final + displacement), data[i], wav_headers[i]->sub_chunk2_size);
        displacement += wav_headers[i]->sub_chunk2_size / 2;
    }

    (*wav_header_final)->sub_chunk2_size = total_data;
    (*wav_header_final)->chunk_size = total_data + DIFF_DATA_SIZE;

}

///////////////////////////////////////////////////////////////////////////////////////////

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
        for (int j = 0; j < wav_headers[i]->sub_chunk2_size / sizeof(int16_t); j++) {
            (*data_final)[j] += data[i][j];
        }
        
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

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

void frequency_modifiew(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){
    if (arg_data->level <= 0 || arg_data->level >= 10){
        arg_data->level = DEF_FREQ;
        fprintf(stderr, "wavfreq: level invalido ajustado para freq padrão\n");
    }
    wav_header->sample_rate = wav_header->sample_rate * arg_data->level;
}

void vibrato(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data){

    if (arg_data->level <= 0 || arg_data->level >= 10){
        arg_data->level = DEF_VIB;
        fprintf(stderr, "wavvib: level invalido ajustado para vib padrão\n");
    }

    double ms = 1000.0/(float)wav_header->sample_rate;
    // fprintf(stderr, "%f ", ms);
    for (int i = 0; i < wav_header->sub_chunk2_size/sizeof(int16_t); i++){
        // fprintf(stderr, "%f ", vibrato_sen(i * ms));
        data[i] = data[i] * vibrato_sen(i) * arg_data->level;
    }

    fprintf(stderr, "aqui2\n");
    
}