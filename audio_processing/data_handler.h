// Artur Temporal Coelho GRR20190471 
#ifndef __DATA_HANDLER__
#define __DATA_HANDLER__

#include "header.h"

void print_wav_info(wav_header_t* wav_header);
void volume_changer(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void volume_normalize(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void reversor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void echo_maker(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void concatenate(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final);
void amplified_stereo(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void mixer(arg_data_t* arg_data, wav_header_t** wav_headers, int16_t** data, int num_arq, wav_header_t** wav_header_final, int16_t** data_final);
void channel_extractor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t** data);
void frequency_modifier(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);

#endif