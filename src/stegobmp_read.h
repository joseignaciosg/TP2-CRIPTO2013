#ifndef STEGOBMP_READ_H_INCLUDED
#define STEGOBMP_READ_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Extract a file from a .bmp image using LSB1 */
int lsb1_extract(FILE* image, FILE** msg, const char* name);

int lsb4_extract(FILE* image, FILE** msg, const char* name);

int lsbe_extract(FILE* image, FILE** msg, const char* name);



#endif

