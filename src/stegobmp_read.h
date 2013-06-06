#ifndef STEGOBMP_READ_H_INCLUDED
#define STEGOBMP_READ_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Extract a file from a .bmp image using LSB1 */
int lsb1_extract(FILE* image, FILE* msg);



#endif

