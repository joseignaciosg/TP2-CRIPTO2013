#ifndef BMP_TYPE_H_INCLUDED
#define BMP_TYPE_H_INCLUDED

typedef struct bmp_type
{
    short compressed;
    short version;
    short usable_size;
    uint8_t[] matrix;
};


/* */
int load_img_header(FILE* in, bmp_type* img);

int load_img_matrix(FILE* in, bmp_type* img);

#endif

