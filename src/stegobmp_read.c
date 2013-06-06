#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>
#include "bmp.h"
#include "util.h"
#include "crypt.h"
#include "stegobmp.h"
#include "stegobmp_read.h"

typedef int (byte_reader)(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset);
typedef unsigned int (byte_counter)(const struct bmp_type *img, unsigned int start_offset);

int lsb1_read_bytes(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset)
{
    unsigned int i;
    int j;
    unsigned int offset = start_offset ? *start_offset : 0;

    for(i=0 ; i<size ; i++){
	for (j=7 ; j>=0 ; j--) {
	    if (BIT(in->matrix[offset],0)) 
		*(((uint8_t*)out)+i) |= (1 << j);
	    else
		*(((uint8_t*)out)+i) &= ~(1 << j);
	    offset++;
	}
    }

    if (start_offset)
	*start_offset = offset;

    return 0;
}

int lsb4_read_bytes(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset)
{
    unsigned int i;
    int j,k;
    unsigned int offset = start_offset ? *start_offset : 0;
    uint8_t* o = (uint8_t*) out;

    for(i=0 ; i<size ; i++){
	for (j=1 ; j>=0 ; j--) {
	    for (k=3 ; k>=0 ; k--) {
		if (BIT(in->matrix[offset],k)) 
		    o[i] |= (1 << (k+j*4));
		else
		    o[i] &= ~(1 << (k+j*4));
	    }
	    offset++;
	}
    }

    if (start_offset)
	*start_offset = offset;

    return 0;
}

int lsbe_read_bytes(void* out, const unsigned int size, const struct bmp_type* in, unsigned int* start_offset)
{
    unsigned int i;
    int j;
    unsigned int offset = start_offset ? *start_offset : 0;
    uint8_t* o = (uint8_t*) out;

    for(i=0 ; i<size ; i++){
	for (j=7 ; j>=0 ; j--) {
	    while (in->matrix[offset] != 254 || in->matrix[offset] != 255)
		offset++;

	    if (BIT(in->matrix[offset],0)) 
		o[i] |= (1 << j);
	    else
		o[i] &= ~(1 << j);
	    offset++;
	}
    }

    if (start_offset)
	*start_offset = offset;

    return 0;
}

unsigned int lsb1_count_bytes(const struct bmp_type* in, unsigned int start_offset)
{
    int j;
    unsigned int count = 0;
    uint8_t test = -1;

    while(test != '\0') {
	for (j=7 ; j>=0 ; j--) {
	    if (BIT(in->matrix[start_offset],0)) 
		test |= (1 << j);
	    else
		test &= ~(1 << j);
	    start_offset++;
	}
	count++;
    }

    return count;
}

unsigned int lsb4_count_bytes(const struct bmp_type* in, unsigned int start_offset)
{
    int j,k;
    unsigned int count = 0;
    uint8_t test = -1;

    while(test != '\0') {
	for (j=1 ; j>=0 ; j--) {
	    for (k=3 ; k>=0 ; k--) {
		if (BIT(in->matrix[start_offset],k)) 
		    test |= (1 << (k+j*4));
		else
		    test &= ~(1 << (k+j*4));
	    }
	    start_offset++;
	}
	count++;
    }

    return count;
}

unsigned int lsbe_count_bytes(const struct bmp_type* in, unsigned int start_offset)
{
    int j;
    unsigned int count = 0;
    uint8_t test = -1;

    while(test != '\0') {
	for (j=7 ; j>=0 ; j--) {
	    while (in->matrix[start_offset] != 254 || in->matrix[start_offset] != 255)
		start_offset++;

	    if (BIT(in->matrix[start_offset],0)) 
		test |= (1 << j);
	    else
		test &= ~(1 << j);
	    start_offset++;
	}
	count++;
    }

    return count;
}

int lsbX_extract(FILE* image, FILE** msg_f, const char* name, byte_reader* reader, byte_counter* counter)
{
    char *msg, *extension;
    struct bmp_type img;
    uint32_t offset = 0;
    uint32_t msg_size, extension_size;
    char* filename;

    /* header loading */
    load_img_header(image, &img);

    /* content loading */
    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    /*reading size*/
    (*reader)(&msg_size, sizeof(uint32_t), &img, &offset);
    msg_size = ntohl(msg_size);
    printf("size: %i\n",msg_size);

    /*reading content*/
    msg = calloc(msg_size, sizeof(uint8_t));
    (*reader)(msg, msg_size, &img, &offset);
    printf("hidden message: %s \n", msg);

    /*reading extension*/
    extension_size = (*counter)(&img, offset);
    extension = malloc(sizeof(char)*extension_size);
    (*reader)(extension, extension_size, &img, &offset);
    printf("extension: %s \n", extension);

    /* extension_size includes '\0' */
    filename = malloc(sizeof(char)*(extension_size+strlen(name)));
    strncpy(filename, name, strlen(name)+1);
    strncat(filename,extension,extension_size);
   
    *msg_f = fopen(filename, "wb");
    fwrite(msg, sizeof(uint8_t), msg_size, *msg_f);

    free(msg);
    free(filename);
    free(img.matrix);
    free(extension);

    return 0;
}

int lsbX_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char *passwd, 
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo,
	byte_reader *reader)
{
    uint8_t* encrypted_content;
    uint8_t* decrypted_content;
    struct bmp_type img;
    unsigned int offset = 0;
    uint32_t encrypted_size, msg_size, extension_size, decrypted_size;
    char* filename;

    /* header loading */
    load_img_header(image, &img);

    /* content loading */
    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    /*reading size*/
    (*reader)(&encrypted_size, sizeof(uint32_t), &img, &offset);
    encrypted_size = ntohl(encrypted_size);
    printf("size: %i\n",encrypted_size);
    
    /*reading encrypted content*/
    encrypted_content = malloc(sizeof(uint8_t)*encrypted_size);
    decrypted_content = malloc(sizeof(uint8_t)*encrypted_size);
    
    (*reader)(encrypted_content, encrypted_size, &img, &offset);
    decrypt(encrypted_content, encrypted_size, (unsigned char*) passwd, algo, blk_algo, decrypted_content, &decrypted_size);

    printf("decrypted: %s\n",decrypted_content+sizeof(uint32_t));
    memcpy(&msg_size, decrypted_content, sizeof(uint32_t));
    msg_size = ntohl(msg_size);

    /* reading extension */
    extension_size = decrypted_size - msg_size - sizeof(uint32_t);
    
    /* building the filename */
    filename = malloc(sizeof(uint8_t)*(strlen(name)+extension_size));
    strncpy(filename, name, strlen(name)+1);
    strncat(filename, (char*) &(decrypted_content[sizeof(uint32_t)+msg_size]), extension_size);

    /* writing to file */
    *msg_f = fopen(filename, "wb");
    fwrite((char*) &(decrypted_content[sizeof(uint32_t)]), sizeof(uint8_t), msg_size, *msg_f);

    free(encrypted_content);
    free(decrypted_content);
    free(filename);
    free(img.matrix);

    return 0;
}

int lsb1_extract(FILE* image, FILE** msg_f, const char* name)
{
    return lsbX_extract(image, msg_f, name, lsb1_read_bytes, lsb1_count_bytes);
}

int lsb4_extract(FILE* image, FILE** msg_f, const char* name)
{
    return lsbX_extract(image, msg_f, name, lsb4_read_bytes, lsb4_count_bytes);
}

int lsbe_extract(FILE* image, FILE** msg_f, const char* name)
{
    return lsbX_extract(image, msg_f, name, lsbe_read_bytes, lsbe_count_bytes);
}

// int lsb1_read_bytes( void* msg, struct bmp_type* img, unsigned int* start_offset)
// {

//     int i,j,size=0;
//     uint8_t* to_be_written = (uint8_t*) in;
//     unsigned int offset = start_offset ? *start_offset : 0;
//     char c, hidden, *msg;

//     /*levanto el size*/
//     for(i=0; i<4*8; i++){
//         hidden = fgetc(img->matrix[offset]);
//         offset++;
//         *(((char*)&size)+i/8)|=((hidden&1)<<7-(i%8));
//     }

//     /*lavanto el contenido*/
//     msg = calloc(size+4, sizeof(char));
//     memcpy(msg, &size, 4);
//     msg+=4;
//     for(i=0; i<(size-4)*8; i++){
//         hidden = fgetc(img->matrix[offset]);
//         *(msg+i/8)|=((hidden&1)<<7-(i%8));
//         offset++;
//     }
//     msg[i/8]=0;
//     msg-4;

//     int i,j;
//     uint8_t* to_be_read = (uint8_t*) img->matrix[offset];
//     unsigned int offset = start_offset ? *start_offset : 0;

//     for (i=0 ; i<size ; i++)
//     {
//         if (BIT(to_be_read[i],j))
//             msg |= (uint8_t) 1;
//         else
//             msg &= (uint8_t) ~1;
//         offset++;

//     }
//     if (start_offset)
//     *start_offset = offset;

//     return 0;
// }


