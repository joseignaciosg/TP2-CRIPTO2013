#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <file.h>
#include "stegobmp.h"

/* extract the n-th bit from number x */
#define BIT(x,n) ((x >> n) & 0x01)

#define BUFFER_SIZE 256

#define WRONG_VERSION_ERR -1
#define COMPRESSED_ERR -2
#define TOO_SMALL_ERR -3

static int get_file_size(const FILE* file)
{
    int size;
    fseek(file, 0, SEEK_END);
    size = ftell(&file);
    rewind(file);
    return size;
}

static int bmp_checking(const int required_size, const struct bmp_type* img)
{
    if (!check_version(&img))
    {
	fprintf(stderr,"Wrong BMP version (stegobmp only supports BMPv3\n");
	return WRONG_VERSION_ERR;
    }

    if ((int size = check_image_size(required_size,&img)) != 0)
    {
	fprintf(stderr,"The chosen image is too small to contain the message (maximum size usable with this image: %i)\n",size);
	return TOO_SMALL_ERR;
    }

    if (!check_compression(&img))
    {
	fprintf(stderr,"stegobmp doesn't support compressed bmp\n");
	return COMPRESSED_ERR;
    }

    return 0;
}


int lsb1_embed(const FILE* image, const FILE* in, FILE* out)
{
    struct bmp_type img;
    uint8_t buffer[BUFFER_SIZE];
    int offset = 4;
    load_img_header(image, &img);
    
    int in_file_size = get_file_size(&in);
    if (!bmp_checking(in_file_size,&img))
	return -1;

    img.matrix = (uint8_t) malloc(sizeof(img.usable_size));
    load_img_matrix(image, &img);

    /* cool stuff */
    lsb1_write_bytes(&in_file_size, sizeof(int), &img, 0);

    while (size_t read_size = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, in))
    {
	lsb1_write_bytes(buffer, read_size, &img, offset);
	offset += read_size;
    }
    /* escribir extensión */

    free(img.matrix);
    return 0;
}

int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out, int start_offset)
{
    int i,j;

    for (i=0 ; i<size ; i++)
    {
	for(j=0 ; j<8 ; j++)
	{
	    if (BIT(in[i],j))
		out[start_offset] |= (uint_8) 1;
	    else
		out[start_offset] &= (uint_8) ~1;
	    start_offset++;
	}
    }
}


int lsb4_write_bytes(const void* in, const int size, struct bmp_type* out, int start_offset)
{
    int i,j,k;
    int index_out=0;

    for (i=0 ; i<size ; i++)
    {
	for(j=0 ; j<8 ; j++)
	{
	    for (k=0 ; k<4 ; k++)
	    {
		if (BIT(in[i],j))
		    out[index_out] |= (uint_8) (1 << k);
		else
		    out[index_out] &= (uint_8) ~(1 << k);
	    }
	    index_out++;
	}
    }
}

