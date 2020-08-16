#ifndef __FILE_HANDLE__
#define __FILE_HANDLE__

#include <stdio.h>

#include "header.h"

file_data_t* get_arg_data(int argc, char **argv);
wav_header_t* read_header(char* input_file_name);
int16_t* get_wav_data(wav_header_t *wav_header, file_data_t *file_data);
int store_wav_data(wav_header_t *wav_header, file_data_t *file_data, int16_t *data);

#endif