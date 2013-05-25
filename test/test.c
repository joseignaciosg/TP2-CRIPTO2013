#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "stegobmp.h"

int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out, int start_offset)
{
    int i,j;
    uint8_t* to_be_written = (uint8_t*) in;

    for (i=0 ; i<size ; i++)
    {
	for(j=0 ; j<8 ; j++)
	{
	    if (BIT(to_be_written[i],j))
		out->matrix[start_offset] |= (uint8_t) 1;
	    else
		out->matrix[start_offset] &= (uint8_t) ~1;
	    start_offset++;
	}
    }

    return 0;
}

int lsb4_write_bytes(const void* in, const int size, struct bmp_type* out, int start_offset)
{
    int i,j,k;
    uint8_t* to_be_written = (uint8_t*) in;

    for (i=0 ; i<size ; i++)
    {
	for(j=0 ; j<2 ; j++)
	{
	    for (k=0 ; k<4 ; k++)
	    {
		if (BIT(to_be_written[i],j*4+k))
		    out->matrix[start_offset] |= (uint8_t) (1 << (k));
		else
		    out->matrix[start_offset] &= (uint8_t) ~(1 << (k));
	    }
	    start_offset++;
	}
    }

    return 0;
}

int main(void)
{
    char in = 0x0F;
    struct bmp_type out;

    out.matrix = malloc(sizeof(char)*8);
    memset(out.matrix,'h',8);

    printf("out.matrix: %s\n",out.matrix);
    lsb1_write_bytes(&in, sizeof(in), &out, 0);
    printf("out.matrix: %s\n",out.matrix);


    memset(out.matrix,'d',8);

    printf("out.matrix: %s\n",out.matrix);
    lsb4_write_bytes(&in, sizeof(in), &out, 0);
    FILE* file = fopen("1","wb");
    fprintf(file,"out.matrix: %s\n",out.matrix);
    fclose(file);
    
    free(out.matrix);
    return 0;
}

