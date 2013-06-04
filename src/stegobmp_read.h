#ifndef STEGOBMP_READ_H_INCLUDED
#define STEGOBMP_READ_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Extract a file from a .bmp image using LSB1 */
int lsb1_extract(FILE* image, FILE* in, const char* extension, FILE* out);
/*
int lsb1_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsb4_embed(FILE* image, FILE* in, const char* extension, FILE* out);
int lsb4_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsbe_embed(FILE* image, FILE* in, const char* extension, FILE* out);
int lsbe_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);
*/

/* Read a sequence of bytes from a file using lsb1 */
int lsb1_read_bytes(const void* in, const int size, struct bmp_type* out, unsigned int *start_offset);

/*
int lsb4_write_bytes(const void* in, const int size, struct bmp_type* out, unsigned int *start_offset);

int lsbe_write_bytes(const void* in, const int size, struct bmp_type* out, unsigned int *start_offset);
*/


/* Calculate the maximum length that can be embedded in a file */
unsigned int lsb1_maximum_size_calculator(FILE* in, const char* extension);
/*
unsigned int lsb1_crypt_maximum_size_calculator(FILE* in, unsigned int block_size, const char* extension);

unsigned int lsb4_maximum_size_calculator(FILE* in, const char* extension);
unsigned int lsb4_crypt_maximum_size_calculator(FILE* in, unsigned int block_size, const char* extension);

unsigned int lsbe_maximum_size_calculator(FILE* in, const char* extension);
unsigned int lsbe_crypt_maximum_size_calculator(FILE* in, unsigned int block_size, const char* extension);
*/


/* Verification functions */
int check_image_size(const int requested_size, const struct bmp_type* img);

int check_compression(const struct bmp_type* img);

int check_version(const struct bmp_type* img);

#endif


#endif

