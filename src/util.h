#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include "bmp.h"

const char *get_filename_ext(const char *filename);
int check_version(const struct bmp_type* img);
int check_compression(const struct bmp_type* img);
int get_file_size(FILE* file);


#endif

