#ifndef __FILE_HANDLE__
#define __FILE_HANDLE__

#include <stdio.h>

#include "header.h"

int get_files(int argc, char **argv, FILE** input_file, FILE** output_file);
wav_header_t* read_header(FILE* input_file);

#endif