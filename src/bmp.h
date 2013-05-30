#ifndef BMP_TYPE_H_INCLUDED
#define BMP_TYPE_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

#define BMP_FILE_HEADER_SIZE 54
#define BMP_IMAGE_HEADER_SIZE 40
#define BMP_MAGIC_OFFSET 0x0000
#define BMP_HEADER_SIZE_OFFSET 0x000e
#define BMP_COMPRESSION_OFFSET 0x001e
#define BMP_PIXELS_MATRIX_SIZE_OFFSET 0x0022
#define BMP_PIXELS_MATRIX_ADDRESS_OFFSET 0x000a

struct bmp_type
{
    uint32_t compressed; /* at 0x001e */
    int32_t version;    /* check for magic header and header size */
    uint32_t usable_size; /* at 0x0022 */
    uint8_t header[BMP_FILE_HEADER_SIZE]; /* complete header */
    uint8_t* matrix;  /* from [0x000a] */
};


/* */
int load_img_header(FILE* in, struct bmp_type* img);

int load_img_matrix(FILE* in, struct bmp_type* img);

int write_img(FILE* out, struct bmp_type* img);

#endif

