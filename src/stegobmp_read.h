#ifndef STEGOBMP_READ_H_INCLUDED
#define STEGOBMP_READ_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Extract a file from a .bmp image */
int lsb1_extract(FILE* image, FILE** msg, const char* name);
int lsb4_extract(FILE* image, FILE** msg, const char* name);
int lsbe_extract(FILE* image, FILE** msg, const char* name);

/* Extract an encrypted file from a .bmp image */
int lsb1_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char* passwd,
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo);
int lsb4_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char* passwd,
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo);
int lsbe_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char* passwd,
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

#endif
