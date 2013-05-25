#ifndef BMP_TYPE_H_INCLUDED
#define BMP_TYPE_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

struct bmp_type
{
    uint32_t compressed; /* at 0x001e */
    uint32_t version;    /* check for magic header and header size */
    uint32_t usable_size; /* at 0x0022 */
    uint8_t* matrix;  /* from [0x000a] */
};


/* */
int load_img_header(FILE* in, struct bmp_type* img);

int load_img_matrix(FILE* in, struct bmp_type* img);

#endif

