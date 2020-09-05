#ifndef __DATA_HANDLER__
#define __DATA_HANDLER__

#include "header.h"

void print_wav_info(wav_header_t* wav_header);
void volume_changer(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void volume_normalize(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);
void reversor(arg_data_t* arg_data, wav_header_t* wav_header, int16_t* data);

#endif