#include <stdio.h>
#include <stdlib.h>
#include "stegobmp.h"

/* extract the n-th bit from number x */
#define BIT(x,n) ((x >> n) & 0x01)

int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out)
{
    int i,j;
    int index_out=0;

    for (i=0 ; i<size ; i++)
    {
	for(j=0 ; j<8 ; j++)
	{
	    if (BIT(in[i],j))
		out[index_out] |= (uint_8) 1;
	    else
		out[index_out] &= (uint_8) ~1;
	    index_out++;
	}
    }
}


