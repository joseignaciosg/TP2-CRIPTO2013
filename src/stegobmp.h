#ifndef STEGOBMP_H_INCLUDED
#define STEGOBMP_H_INCLUDED

#include "crypt.h"
#include "bmp.h"

/* extract the n-th bit from number x */
#define BIT(x,n) (((x) >> (n)) & 1)

#define BUFFER_SIZE 256
#define SIZE_MARKER_LENGTH sizeof(int)

#define WRONG_VERSION_ERR 1
#define COMPRESSED_ERR 2
#define TOO_SMALL_ERR 3

/*values*/
#define EXTRACT 1
#define EMBED   2
#define STEG    3
#define OUT     4
#define PASS    5
#define IN      6
#define LSB1    7
#define LSB4    8
#define LSBE    9

#endif

