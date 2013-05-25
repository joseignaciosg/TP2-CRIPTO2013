#ifndef STEGOBMP_WRITE_H_INCLUDED
#define STEGOBMP_WRITE_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Extract a file from a .bmp */
int lsb1_extract(FILE* in, const char* base_filename);
int lsb1_extract_decrypt(FILE* in, const char* base_filename, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsb4_extract(FILE* in, const char* base_filename);
int lsb4_extract_decrypt(FILE* in, const char* base_filename, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsbe_extract(FILE* in, const char* base_filename);
int lsbe_extract_decrypt(FILE* in, const char* base_filename, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

/* Read a sequence of bytes from a file */
int lsb1_read_bytes(const struct bmp_type* in, void* out);

int lsb4_read_bytes(const struct bmp_type* in, void* out);

int lsbe_read_bytes(const struct bmp_type* in, void* out);

#endif

