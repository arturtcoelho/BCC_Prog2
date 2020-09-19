// Artur Temporal Coelho GRR20190471 
#ifndef __DATA_HANDLER__
#define __DATA_HANDLER__

#include "file_handle.h"

int16_t max_value(wav_header_t* wav_header, int16_t* data);
void print_wav_info(wav_header_t* wav_header);
void volume_changer(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void volume_normalize(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void reversor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void echo_maker(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void concatenate(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final, int total_data);
void amplified_stereo(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void mixer(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final, int total_data);
void channel_extractor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t** data);
void frequency_modifier(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);

// definição de códigos de erro
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
#define DIFF_DATA_SIZE 36

#endif