#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include "bmp.h"
#include "stegobmp_write.h"


int load_img_header(FILE* in, struct bmp_type* img)
{
    uint32_t header_size;
    
    rewind(in);
    if (fread(img->header, sizeof(uint8_t), BMP_FILE_HEADER_SIZE, in) != BMP_FILE_HEADER_SIZE) {
	perror("IO error when retrieving the header");
	return -1;
    }
	    

    /* version is "3" if the magic number is "DM"
     * and the header size is equal to 40 */
    memcpy(&header_size, &(img->header[BMP_HEADER_SIZE_OFFSET]), sizeof(uint32_t));
    if (img->header[BMP_MAGIC_OFFSET]   == 'B' && 
	img->header[BMP_MAGIC_OFFSET+1] == 'M' && 
	header_size == BMP_IMAGE_HEADER_SIZE) {
	    img->version = 3;
    } else {
	    img->version = -1;
    }

    /* BMP file is uncompressed if [0x00e1] == 0 */
    memcpy(&(img->compressed), &(img->header[BMP_COMPRESSION_OFFSET]), sizeof(uint32_t));

    /* BMP pixel array size is at 0x0022 */
    memcpy(&(img->usable_size), &(img->header[BMP_PIXELS_MATRIX_SIZE_OFFSET]), sizeof(uint32_t));

    return 0;
}

int load_img_matrix(FILE* in, struct bmp_type* img)
{
    uint32_t offset;
    
    /* pixel array starts at offset [0x000a] */
    memcpy(&offset, &(img->header[BMP_PIXELS_MATRIX_ADDRESS_OFFSET]), sizeof(uint32_t));

    fseek(in, offset, SEEK_SET); /* fseek expects a long int, conversion from uint32_t is safe */
    if (fread(img->matrix, sizeof(uint8_t), img->usable_size, in) != img->usable_size)
	    return -1;

    return 0;
}

int write_img(FILE* out, struct bmp_type* img)
{
    rewind(out);

    /* we've got the header, we've got the pixels matrix, pretty easy, uh? */
    if (fwrite(img->header,sizeof(uint8_t),BMP_FILE_HEADER_SIZE,out) != BMP_FILE_HEADER_SIZE)
	return -1;
    if (fwrite(img->matrix,sizeof(uint8_t),img->usable_size,out) != img->usable_size)
	return -1;

    return 0;
}


