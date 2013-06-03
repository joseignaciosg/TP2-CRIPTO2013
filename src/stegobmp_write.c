#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>
#include "bmp.h"
#include "util.h"
#include "crypt.h"
#include "stegobmp.h"
#include "stegobmp_write.h"

typedef int (lsbX_writing_bytes_function_type)(const void* in, const int size, struct bmp_type* out, int *start_offset);
typedef unsigned int (size_calculator_type)(unsigned int raw_file_size, const char* extension);
typedef unsigned int (size_calculator_crypt_type)(unsigned int raw_file_size, unsigned int block_size, const char* extension);

/*********************************************************************************/
/*				    HELPERS					 */
/*********************************************************************************/
static int bmp_checking(const struct bmp_type* img, const unsigned int required_size, const unsigned int max_size)
{
    if (!check_version(img))
    {
	fprintf(stderr,"Wrong BMP version (stegobmp only supports BMPv3\n");
	return WRONG_VERSION_ERR;
    }

    if (required_size > img->usable_size)
    {
	fprintf(stderr,"The chosen image is too small to contain the message (maximum file size usable with this image: %i bytes)\n",max_size);
	return TOO_SMALL_ERR;
    }

    if (!check_compression(img))
    {
	fprintf(stderr,"stegobmp doesn't support compressed bmp\n");
	return COMPRESSED_ERR;
    }

    return 0;
}


/*********************************************************************************/
/*				LSB Generic					 */
/*********************************************************************************/
static int lsbX_embed(FILE* image, FILE* in, const char* extension, FILE* out, size_calculator_type *actual_size_calc, size_calculator_type *max_size_calc, lsbX_writing_bytes_function_type *writer_delegate)
{
    struct bmp_type img;
    uint8_t buffer[BUFFER_SIZE];
    int offset = 0;
    size_t read_size;
    uint32_t in_file_size;

    load_img_header(image, &img);
    
    in_file_size = get_file_size(in);
    if (bmp_checking(&img,(*actual_size_calc)(in_file_size,extension),(*max_size_calc)(img.usable_size,extension)) != 0)
	return -1;

    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    /* size must be written in big endian order */
    in_file_size = htonl(in_file_size);
    (*writer_delegate)(&in_file_size, sizeof(uint32_t), &img, &offset);
    while ((read_size = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, in)) > 0)
	(*writer_delegate)(buffer, read_size, &img, &offset);
    /* writing the extension, including the \0 at the end (so strlen(extension) + 1 byte for \0) */
    (*writer_delegate)(extension, strlen(extension)+1, &img, &offset);

    /* write to FILE* out */
    write_img(out, &img);

    free(img.matrix);
    return 0;
}

static int lsbX_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
			    size_calculator_crypt_type *actual_size_calc, size_calculator_crypt_type *max_size_calc, 
			    lsbX_writing_bytes_function_type *writer_delegate,
			    const char* passwd, enum encrypt_type enc, enum encrypt_block_type blk)
{
    struct bmp_type img;
    int offset = 0;
    uint32_t in_file_size, in_file_size_big_endian;
    uint32_t out_length, out_length_big_endian;
    unsigned int block_size, required_size;
    char* in_buf;
    char* out_buf;

    load_img_header(image, &img);
    
    in_file_size = get_file_size(in);
    block_size = get_block_size_for_cipher(enc, blk);
    required_size = (*actual_size_calc)(in_file_size, blk, extension); 
    if (bmp_checking(&img, required_size, (*max_size_calc)(img.usable_size, block_size, extension)) != 0)
	return -1;

    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    in_buf = malloc(sizeof(char)*required_size);
    in_file_size_big_endian = htonl(in_file_size);
    memcpy(in_buf, &in_file_size_big_endian, SIZE_MARKER_LENGTH);
    fread(in_buf+SIZE_MARKER_LENGTH, sizeof(char), in_file_size, in);
    strcpy(in_buf+SIZE_MARKER_LENGTH+in_file_size, extension);
    out_buf = malloc(sizeof(char)*required_size);
    crypt((unsigned char*) in_buf, in_file_size, (unsigned char*) passwd, enc, blk, (unsigned char*) out_buf, &out_length);
    out_length_big_endian = htonl(out_length);

    (*writer_delegate)(&out_length_big_endian, sizeof(uint32_t), &img, &offset);
    (*writer_delegate)(out_buf, sizeof(char), &img, &offset);

    /* write to FILE* out */
    write_img(out, &img);

    free(img.matrix);
    free(in_buf);
    free(out_buf);
    return 0;
}

/*********************************************************************************/
/*				LSB1						 */
/*********************************************************************************/
unsigned int lsb1_maximum_size_calculator(unsigned int raw_file_size, const char* extension)
{
    return (raw_file_size - SIZE_MARKER_LENGTH - strlen(extension) - 1) / 8;
}

unsigned int lsb1_crypt_maximum_size_calculator(unsigned int raw_file_size, unsigned int block_size, const char* extension)
{
    return (((raw_file_size - SIZE_MARKER_LENGTH*2 - strlen(extension) - 1) / 8) / block_size) * block_size;
}

unsigned int lsb1_required_size_calculator(unsigned int raw_file_size, const char* extension)
{
    return raw_file_size*8 + SIZE_MARKER_LENGTH + strlen(extension) + 1;
}

unsigned int lsb1_crypt_required_size_calculator(unsigned int raw_file_size, unsigned int block_size, const char* extension)
{
    return ((raw_file_size*8 + SIZE_MARKER_LENGTH*2 + strlen(extension) + 1) / block_size + 1) * block_size;
}

int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out, int* start_offset)
{
    int i,j;
    uint8_t* to_be_written = (uint8_t*) in;
    int offset = start_offset ? *start_offset : 0;

    for (i=0 ; i<size ; i++)
    {
    	for(j=0 ; j<8 ; j++)
    	{
    	    if (BIT(to_be_written[i],j))
    		out->matrix[offset] |= (uint8_t) 1;
    	    else
    		out->matrix[offset] &= (uint8_t) ~1;
    	    offset++;
    	}
    }
    if (start_offset)
	*start_offset = offset;

    return 0;
}

int lsb1_embed(FILE* image, FILE* in, const char* extension, FILE* out)
{
    return lsbX_embed(image, in, extension, out, lsb1_required_size_calculator, lsb1_maximum_size_calculator, lsb1_write_bytes);
}

int lsb1_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
			    const char* passwd, const enum encrypt_type enc, const enum encrypt_block_type blk) 
{
    return lsbX_crypt_embed(image, in, extension, out, 
	    lsb1_crypt_required_size_calculator, lsb1_crypt_maximum_size_calculator, lsb1_write_bytes,
	    passwd, enc, blk);
}

/*********************************************************************************/
/*				LSB4						 */
/*********************************************************************************/
unsigned int lsb4_maximum_size_calculator(unsigned int raw_file_size, const char* extension)
{
    return (raw_file_size - SIZE_MARKER_LENGTH - strlen(extension) - 1) / 2;
}

unsigned int lsb4_crypt_maximum_size_calculator(unsigned int raw_file_size, unsigned int block_size, const char* extension)
{
    return (((raw_file_size - SIZE_MARKER_LENGTH*2 - strlen(extension) - 1) / 2) / block_size) * block_size;
}

unsigned int lsb4_required_size_calculator(unsigned int raw_file_size, const char* extension)
{
    return raw_file_size*2 + SIZE_MARKER_LENGTH + strlen(extension) + 1;
}

unsigned int lsb4_crypt_required_size_calculator(unsigned int raw_file_size, unsigned int block_size, const char* extension)
{
    return ((raw_file_size*2 + SIZE_MARKER_LENGTH*2 + strlen(extension) + 1) / block_size + 1) * block_size;
}


int lsb4_write_bytes(const void* in, const int size, struct bmp_type* out, int* start_offset)
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
    		    out->matrix[*start_offset] |= (uint8_t) (1 << (k));
    		else
    		    out->matrix[*start_offset] &= (uint8_t) ~(1 << (k));
    	    }

    	    (*start_offset)++;
    	}
    }

    return 0;
}

int lsb4_embed(FILE* image, FILE* in, const char* extension, FILE* out)
{
    return lsbX_embed(image, in, extension, out, lsb4_required_size_calculator, lsb4_maximum_size_calculator, lsb4_write_bytes);
}

int lsb4_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
			    const char* passwd, const enum encrypt_type enc, const enum encrypt_block_type blk) 
{
    return lsbX_crypt_embed(image, in, extension, out, 
	    lsb4_crypt_required_size_calculator, lsb4_crypt_maximum_size_calculator, lsb4_write_bytes,
	    passwd, enc, blk);
}


/*********************************************************************************/
/*				LSBE						 */
/*********************************************************************************/
