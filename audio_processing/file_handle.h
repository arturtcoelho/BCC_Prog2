#ifndef __FILE_HANDLE__
#define __FILE_HANDLE__

#include <stdio.h>

#include "header.h"

arg_data_t* get_arg_data(int argc, char **argv);
wav_header_t* read_header(void* input_file_name);
int get_wav_data(int16_t **data, wav_header_t **wav_header, arg_data_t *arg_data);
int store_wav_data(wav_header_t *wav_header, arg_data_t *arg_data, int16_t *data);

#endif