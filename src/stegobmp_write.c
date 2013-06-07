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

typedef int (lsbX_writing_bytes_function_type)(const void* in, const int size, struct bmp_type* out, unsigned int *start_offset);
typedef unsigned int (size_calculator_type)(FILE* f, const char* extension);
typedef unsigned int (size_calculator_crypt_type)(FILE* f, unsigned int block_size, const char* extension);

/*********************************************************************************/
/*				    HELPERS					 */
/*********************************************************************************/
inline static unsigned int lsbX_packet_size(unsigned int raw_file_size, const char* extension)
{
    return raw_file_size + SIZE_MARKER_LENGTH + strlen(extension) + 1;
}

inline static unsigned int lsbX_crypt_packet_size(unsigned int raw_file_size, unsigned int block_size, const char* extension)
{
    return ((raw_file_size + SIZE_MARKER_LENGTH*2 + strlen(extension) + 1) / block_size + 1) * block_size;
}

static int bmp_checking(const struct bmp_type* img, const unsigned int in_size, const unsigned int max_size)
{
    if (!check_version(img))
    {
	fprintf(stderr,"Wrong BMP version (stegobmp only supports BMPv3\n");
	return WRONG_VERSION_ERR;
    }

    if (in_size > max_size)
    {
	fprintf(stderr,"The chosen image is too small to contain the message (of size %i bytes whereas maximum file size usable with this image is %i bytes)\n",in_size,max_size);
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
static int lsbX_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
		      size_calculator_type *max_size_calc, lsbX_writing_bytes_function_type *writer_delegate)
{
    struct bmp_type img;
    uint8_t buffer[BUFFER_SIZE];
    unsigned int offset = 0;
    size_t read_size;
    uint32_t in_file_size;

    /* image header loading */
    load_img_header(image, &img);
    
    /* size computation */
    in_file_size = get_file_size(in);

    /* checking and image loading */
    if (bmp_checking(&img, in_file_size, (*max_size_calc)(image,extension)) != 0)
	return -1;
    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    /* packet construction on-the-fly and bit manipulation on image content
     * size must be written in big endian order to the file */
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
			    size_calculator_crypt_type *max_size_calc, 
			    lsbX_writing_bytes_function_type *writer_delegate,
			    const char* passwd, enum encrypt_type enc, enum encrypt_block_type blk)
{
    struct bmp_type img;
    unsigned int offset = 0;
    uint32_t in_file_size, in_file_size_big_endian;
    uint32_t out_length, out_length_big_endian;
    unsigned int block_size, crypt_expected_packet_size, packet_size;
    char* in_buf;
    char* out_buf;

    /* header loading */
    load_img_header(image, &img);
    
    /* sizes computation */
    in_file_size = get_file_size(in);
    block_size = get_block_size_for_cipher(enc, blk);
    crypt_expected_packet_size = lsbX_crypt_packet_size(in_file_size, block_size, extension); 

    /* checking and image loading */
    if (bmp_checking(&img, crypt_expected_packet_size, (*max_size_calc)(image, block_size, extension)) != 0)
	return -1;
    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    /* packet allocation */
    packet_size = lsbX_packet_size(in_file_size, extension);
    in_buf = malloc(sizeof(char)*packet_size);
    in_file_size_big_endian = htonl(in_file_size);

    /* packet building */
    memcpy(in_buf, &in_file_size_big_endian, SIZE_MARKER_LENGTH);
    fread(in_buf+SIZE_MARKER_LENGTH, sizeof(char), in_file_size, in);
    memcpy(in_buf+SIZE_MARKER_LENGTH+in_file_size, extension, strlen(extension)+1);
    
    /* encryption of packet */
    out_buf = malloc(sizeof(char)*crypt_expected_packet_size);
    crypt((unsigned char*) in_buf, packet_size, (unsigned char*) passwd, enc, blk, (unsigned char*) out_buf, &out_length);
    out_length_big_endian = htonl(out_length);

    /* bit manipulation on image content */
    (*writer_delegate)(&out_length_big_endian, sizeof(uint32_t), &img, &offset);
    (*writer_delegate)(out_buf, sizeof(char)*out_length, &img, &offset);

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
inline unsigned int lsb1_maximum_size_calculator(FILE* img, const char* extension)
{
    return get_file_size(img)/8 - BMP_FILE_HEADER_SIZE  - SIZE_MARKER_LENGTH - strlen(extension) - 1;
}

inline unsigned int lsb1_crypt_maximum_size_calculator(FILE* img, unsigned int block_size, const char* extension)
{
    return ((get_file_size(img)/8 - BMP_FILE_HEADER_SIZE  - SIZE_MARKER_LENGTH*2 - strlen(extension) - 1) / block_size) * block_size;
}

int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out, unsigned int* start_offset)
{
    int i,j;
    uint8_t* to_be_written = (uint8_t*) in;
    unsigned int offset = start_offset ? *start_offset : 0;

    for (i=0 ; i<size ; i++)
    {
    	for(j=7 ; j>=0 ; j--)
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
    return lsbX_embed(image, in, extension, out, lsb1_maximum_size_calculator, lsb1_write_bytes);
}

int lsb1_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
			    const char* passwd, const enum encrypt_type enc, const enum encrypt_block_type blk) 
{
    return lsbX_crypt_embed(image, in, extension, out, 
	    lsb1_crypt_maximum_size_calculator, lsb1_write_bytes,
	    passwd, enc, blk);
}

/*********************************************************************************/
/*				LSB4						 */
/*********************************************************************************/
inline unsigned int lsb4_maximum_size_calculator(FILE* img, const char* extension)
{
    return (get_file_size(img) - BMP_FILE_HEADER_SIZE  - SIZE_MARKER_LENGTH - strlen(extension) - 1) / 2;
}

inline unsigned int lsb4_crypt_maximum_size_calculator(FILE* img, unsigned int block_size, const char* extension)
{
    return (((get_file_size(img) - BMP_FILE_HEADER_SIZE - SIZE_MARKER_LENGTH*2 - strlen(extension) - 1) / 2) / block_size) * block_size;
}


int lsb4_write_bytes(const void* in, const int size, struct bmp_type* out, unsigned int* start_offset)
{
    int i,j,k;
    uint8_t* to_be_written = (uint8_t*) in;
    unsigned int offset = start_offset ? *start_offset : 0;

    for (i=0 ; i<size ; i++)
    {
    	for(j=1 ; j>=0 ; j--)
    	{
    	    for (k=3 ; k>=0 ; k--)
    	    {
    		if (BIT(to_be_written[i],j*4+k))
    		    out->matrix[offset] |= (uint8_t) (1 << (k));
    		else
    		    out->matrix[offset] &= (uint8_t) ~(1 << (k));
    	    }
    	    offset++;
    	}
    }

    if (start_offset)
	*start_offset = offset;

    return 0;
}

int lsb4_embed(FILE* image, FILE* in, const char* extension, FILE* out)
{
    return lsbX_embed(image, in, extension, out, lsb4_maximum_size_calculator, lsb4_write_bytes);
}

int lsb4_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
			    const char* passwd, const enum encrypt_type enc, const enum encrypt_block_type blk) 
{
    return lsbX_crypt_embed(image, in, extension, out, 
	    lsb4_crypt_maximum_size_calculator, lsb4_write_bytes,
	    passwd, enc, blk);
}


/*********************************************************************************/
/*				LSBE						 */
/*********************************************************************************/
unsigned int lsbe_maximum_size_calculator(FILE* img, const char* extension)
{
    unsigned int usable_bytes = 0;
    uint8_t c;
    fseek(img, BMP_FILE_HEADER_SIZE, SEEK_SET);
    while (!feof(img))	
    {
	fread(&c, sizeof(uint8_t), 1, img);
	if (c >= 254)
	    usable_bytes++;
    }

    return usable_bytes/8 - SIZE_MARKER_LENGTH - strlen(extension) - 1;
}

inline unsigned int lsbe_crypt_maximum_size_calculator(FILE* img, unsigned int block_size, const char* extension)
{
    unsigned int usable_bytes_stripped = lsbe_maximum_size_calculator(img, extension);
    return ((usable_bytes_stripped - SIZE_MARKER_LENGTH) / block_size) * block_size;
}

int lsbe_write_bytes(const void* in, const int size, struct bmp_type* out, unsigned int* start_offset)
{
    int i,j;
    uint8_t* to_be_written = (uint8_t*) in;
    unsigned int offset = start_offset ? *start_offset : 0;

    for (i=0 ; i<size ; i++)
    {
    	for(j=7 ; j>=0 ; j--)
    	{
	    while (out->matrix[offset] != 254 && out->matrix[offset] != 255)
		offset++;

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

int lsbe_embed(FILE* image, FILE* in, const char* extension, FILE* out)
{
    return lsbX_embed(image, in, extension, out, lsbe_maximum_size_calculator, lsbe_write_bytes);
}

int lsbe_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, 
			    const char* passwd, const enum encrypt_type enc, const enum encrypt_block_type blk) 
{
    return lsbX_crypt_embed(image, in, extension, out, 
	    lsbe_crypt_maximum_size_calculator, lsbe_write_bytes,
	    passwd, enc, blk);
}
