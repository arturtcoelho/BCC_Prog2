#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "file_handle.h"
#include "data_handler.h"

int main(int argc, char  **argv)
{
    arg_data_t *arg_data = get_arg_data(argc, argv);
    wav_header_t *wav_header = read_header(arg_data->input_file_real);

    print_wav_info(wav_header);

    close_files(arg_data);

    return 0;
}
