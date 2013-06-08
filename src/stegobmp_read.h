#ifndef STEGOBMP_READ_H_INCLUDED
#define STEGOBMP_READ_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"



int lsb1_extract(FILE* image, FILE** msg, const char* name);

int lsb4_extract(FILE* image, FILE** msg, const char* name);

int lsbe_extract(FILE* image, FILE** msg, const char* name);

int lsb1_read_bytes(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset);

int lsb4_read_bytes(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset);

int lsbe_read_bytes(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset);


#endif

