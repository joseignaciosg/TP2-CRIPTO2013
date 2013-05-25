#ifndef STEGOBMP_H_INCLUDED
#define STEGOBMP_H_INCLUDED

#include "crypt.h"
#include "bmp.h"

/* extract the n-th bit from number x */
#define BIT(x,n) (((x) >> (n)) & (0x01))

#define BUFFER_SIZE 256
#define SIZE_MARKER_LENGTH sizeof(int)

#define WRONG_VERSION_ERR -1
#define COMPRESSED_ERR -2
#define TOO_SMALL_ERR -3

#endif

