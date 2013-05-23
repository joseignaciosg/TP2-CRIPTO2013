#ifndef STEGOBMP_H_INCLUDED
#define STEGOBMP_H_INCLUDED

/* Embed a file into .bmp image using LSB1 */
int lsb1_embed(const FILE* image, const FILE* in, FILE* out);
int lsb1_embed_crypt(const FILE* image, const FILE* in, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsb4_embed();
int lsb4_embed_crypt();

int lsbe_embed();
int lsbe_embed_crypt();


/* Extract a file from a .bmp */
int lsb1_extract(const FILE* in, const char* base_filename);
int lsb1_extract_decrypt(const FILE* in, const char* base_filename, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsb4_extract(const FILE* in, const char* base_filename);
int lsb1_extract_decrypt(const FILE* in, const char* base_filename, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

int lsbe_extract(const FILE* in, const char* base_filename);
int lsb1_extract_decrypt(const FILE* in, const char* base_filename, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);


/* Write a sequence of bytes into a file using lsb1 */
int lsb1_write_bytes(const void* in, const int size, struct bmp_type* out);

int lsb4_write_bytes();

int lsbe_write_bytes();


/* Read a sequence of bytes from a file */
int lsb1_read_bytes(const struct bmp_type* in, void* in);

int lsb4_read_bytes(const struct bmp_type* in, void* in);

int lsbe_read_bytes(const struct bmp_type* in, void* in);

/* Verification functions */
int check_image_size(const int requested_size, const struct bmp_type* img);

int check_compression(const struct bmp_type* img);

#endif

