#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bmp.h"
#include "stegobmp.h"
#include "stegobmp_write.h"

typedef int (lsbX_writing_bytes_function_type)(const void* in, const int size, struct bmp_type* out, int *start_offset);
typedef int (required_size_calculator_type)(int raw_file_size, const char* extension);

/*********************************************************************************/
/*				    HELPERS					 */
/*********************************************************************************/
static int get_file_size(FILE* file)
{
    int size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}

static int bmp_checking(const int required_size, const struct bmp_type* img)
{
    int size;

    if (!check_version(img))
    {
	fprintf(stderr,"Wrong BMP version (stegobmp only supports BMPv3\n");
	return WRONG_VERSION_ERR;
    }

    if ((size = check_image_size(required_size,img)) != 0)
    {
	fprintf(stderr,"The chosen image is too small to contain the message (maximum size usable with this image: %i)\n",size);
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
static int lsbX_embed(FILE* image, FILE* in, const char* extension, FILE* out, required_size_calculator_type *calc, lsbX_writing_bytes_function_type *writer_delegate)
{
    struct bmp_type img;
    uint8_t buffer[BUFFER_SIZE];
    int offset = 0;
    size_t read_size;
    int in_file_size;

    load_img_header(image, &img);
    
    in_file_size = get_file_size(in);
    if (!bmp_checking((*calc)(in_file_size,extension),&img))
	return -1;

    img.matrix = (uint8_t*) malloc(sizeof(img.usable_size));
    load_img_matrix(image, &img);

    (*writer_delegate)(&in_file_size, sizeof(int), &img, &offset);

    while ((read_size = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, in)) > 0)
	(*writer_delegate)(buffer, read_size, &img, &offset);

    /* writing the extension, including the \0 at the end (so strlen(extension) + 1 byte for \0) */
    (*writer_delegate)(extension, strlen(extension)+1, &img, &offset);

    /* write to FILE* out */

    free(img.matrix);
    return 0;
}


/*********************************************************************************/
/*				LSB1						 */
/*********************************************************************************/
int lsb1_required_size_calculator(int raw_file_size, const char* extension)
{
    return raw_file_size*8 + SIZE_MARKER_LENGTH + strlen(extension) + 1;
}

int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out, int* start_offset)
{
    int i,j;
    uint8_t* to_be_written = (uint8_t*) in;

    for (i=0 ; i<size ; i++)
    {
    	for(j=0 ; j<8 ; j++)
    	{
    	    if (BIT(to_be_written[i],j))
    		out->matrix[*start_offset] |= (uint8_t) 1;
    	    else
    		out->matrix[*start_offset] &= (uint8_t) ~1;
    	    (*start_offset)++;
    	}
    }

    return 0;
}

int lsb1_embed(FILE* image, FILE* in, const char* extension, FILE* out)
{
    return lsbX_embed(image, in, extension, out, lsb1_required_size_calculator, lsb1_write_bytes);
}



/*********************************************************************************/
/*				LSB4						 */
/*********************************************************************************/
int lsb4_required_size_calculator(int raw_file_size, const char* extension)
{
    return raw_file_size*2 + SIZE_MARKER_LENGTH + strlen(extension) + 1;
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
    return lsbX_embed(image, in, extension, out, lsb4_required_size_calculator, lsb4_write_bytes);
}


/*********************************************************************************/
/*				LSBE						 */
/*********************************************************************************/
