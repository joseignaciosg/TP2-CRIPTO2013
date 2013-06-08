#ifndef STEGOBMP_WRITE_H_INCLUDED
#define STEGOBMP_WRITE_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Embed a file into .bmp image using LSB1 */
/**
 * \brief Embed a file in a BMPv3 image without encryption, with algorithm LSB1
 *
 * \param image a file stream to a BMPv3 file
 * \param in a file stream to the file to embed
 * \param extension the extension of the file to embed
 * \param out a file stream to the output image
 *
 * \return 0 if the file could be successfully embedded, -1 otherwise
 */
int lsb1_embed(FILE* image, FILE* in, const char* extension, FILE* out);
/**
 * \brief Embed a file in a BMPv3 image, encrypting it first, with algorithm LSB1
 *
 * \param image a file stream to a BMPv3 file
 * \param in a file stream to the file to embed
 * \param extension the extension of the file to embed
 * \param out a file stream to the output image
 * \param passwd the password used for encryption
 * \param algo the cipher algorithm used for encryption
 * \param blk_algo the cipher chaining mode of operation used for encryption 
 *
 * \return 0 if the file could be successfully embedded, -1 otherwise
 */
int lsb1_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

/**
 * \brief Embed a file in a BMPv3 image without encryption, with algorithm LSB4
 *
 * \param image a file stream to a BMPv3 file
 * \param in a file stream to the file to embed
 * \param extension the extension of the file to embed
 * \param out a file stream to the output image
 *
 * \return 0 if the file could be successfully embedded, -1 otherwise
 */
int lsb4_embed(FILE* image, FILE* in, const char* extension, FILE* out);
/**
 * \brief Embed a file in a BMPv3 image, encrypting it first, with algorithm LSB4
 *
 * \param image a file stream to a BMPv3 file
 * \param in a file stream to the file to embed
 * \param extension the extension of the file to embed
 * \param out a file stream to the output image
 * \param passwd the password used for encryption
 * \param algo the cipher algorithm used for encryption
 * \param blk_algo the cipher chaining mode of operation used for encryption 
 *
 * \return 0 if the file could be successfully embedded, -1 otherwise
 */
int lsb4_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

/**
 * \brief Embed a file in a BMPv3 image without encryption, with algorithm LSB Enhanced
 *
 * \param image a file stream to a BMPv3 file
 * \param in a file stream to the file to embed
 * \param extension the extension of the file to embed
 * \param out a file stream to the output image
 *
 * \return 0 if the file could be successfully embedded, -1 otherwise
 */
int lsbe_embed(FILE* image, FILE* in, const char* extension, FILE* out);
/**
 * \brief Embed a file in a BMPv3 image, encrypting it first, with algorithm LSB Enhanced
 *
 * \param image a file stream to a BMPv3 file
 * \param in a file stream to the file to embed
 * \param extension the extension of the file to embed
 * \param out a file stream to the output image
 * \param passwd the password used for encryption
 * \param algo the cipher algorithm used for encryption
 * \param blk_algo the cipher chaining mode of operation used for encryption 
 *
 * \return 0 if the file could be successfully embedded, -1 otherwise
 */
int lsbe_crypt_embed(FILE* image, FILE* in, const char* extension, FILE* out, const char* passwd, const enum encrypt_type algo, const enum encrypt_block_type blk_algo);


/**
 * \brief Calculate the maximum number of bytes that can be embedded into an image,
 *  without encryption, with algorithm LSB1
 *
 * \param img a file stream to the file to embed
 * \param extension the extension of the file which would be embedded
 *
 * \return the maximum length of the embeddable file content
 */
unsigned int lsb1_maximum_size_calculator(FILE* img, const char* extension);
/**
 * \brief Calculate the maximum number of bytes that can be embedded into an image,
 *  using encryption, with algorithm LSB1
 *
 * \param img a file stream to the file to embed
 * \param block_size the size of a crypted block
 * \param extension the extension of the file which would be embedded
 *
 * \return the maximum length of the embeddable file content
 */
unsigned int lsb1_crypt_maximum_size_calculator(FILE* img, unsigned int block_size, const char* extension);

/**
 * \brief Calculate the maximum number of bytes that can be embedded into an image,
 *  without encryption, with algorithm LSB4
 *
 * \param img a file stream to the file to embed
 * \param extension the extension of the file which would be embedded
 *
 * \return the maximum length of the embeddable file content
 */
unsigned int lsb4_maximum_size_calculator(FILE* img, const char* extension);
/**
 * \brief Calculate the maximum number of bytes that can be embedded into an image,
 *  using encryption, with algorithm LSB4
 *
 * \param img a file stream to the file to embed
 * \param block_size the size of a crypted block
 * \param extension the extension of the file which would be embedded
 *
 * \return the maximum length of the embeddable file content
 */
unsigned int lsb4_crypt_maximum_size_calculator(FILE* img, unsigned int block_size, const char* extension);

/**
 * \brief Calculate the maximum number of bytes that can be embedded into an image,
 *  without encryption, with algorithm LSB Enhanced
 *
 * \param img a file stream to the file to embed
 * \param extension the extension of the file which would be embedded
 *
 * \return the maximum length of the embeddable file content
 */
unsigned int lsbe_maximum_size_calculator(FILE* img, const char* extension);
/**
 * \brief Calculate the maximum number of bytes that can be embedded into an image,
 *  using encryption, with algorithm LSB Enhanced
 *
 * \param img a file stream to the file to embed
 * \param block_size the size of a crypted block
 * \param extension the extension of the file which would be embedded
 *
 * \return the maximum length of the embeddable file content
 */
unsigned int lsbe_crypt_maximum_size_calculator(FILE* img, unsigned int block_size, const char* extension);

#endif

