#ifndef __HEADER__
#define __HEADER__

#define ERR_ARQ_NAO_ENCONTRADO 1
#define ERR_BAD_MALLOC 2

#include <inttypes.h>

typedef struct wav_header {
    // 
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
    // formato
    char sub_chunk1_id[4];
    uint32_t sub_chunk1_size;
    uint16_t audio_format;
    uint16_t number_of_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    // "data"
    char sub_chunk2_id[4];
    uint32_t sub_chunk2_size;
} wav_header_t;

#endif