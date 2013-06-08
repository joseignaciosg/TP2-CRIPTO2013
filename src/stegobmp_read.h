#ifndef STEGOBMP_READ_H_INCLUDED
#define STEGOBMP_READ_H_INCLUDED

#include "crypt.h"
#include "bmp.h"
#include "stegobmp.h"

/* Extract a file from a .bmp image */
/**
 * \brief Extract a hidden file from a BMP file using LSB1 algorithm
 *
 * \param image a file stream to a BMPv3 file
 * \param msg a pointer to a file stream that is not open yet for
 *  the output
 * \param name a pointer to a string containing the base name of the output file
 *
 * \return 0 if a file could be succesfully extracted, -1 otherwise
 */
int lsb1_extract(FILE* image, FILE** msg, const char* name);
/**
 * \brief Extract a hidden file from a BMP file using LSB4 algorithm
 *
 * \param image a file stream to a BMPv3 file
 * \param msg a pointer to a file stream that is not open yet for
 *  the output
 * \param name a pointer to a string containing the base name of the output file
 *
 * \return 0 if a file could be succesfully extracted, -1 otherwise
 */
int lsb4_extract(FILE* image, FILE** msg, const char* name);
/**
 * \brief Extract a hidden file from a BMP file using LSB Enhanced algorithm
 *
 * \param image a file stream to a BMPv3 file
 * \param msg a pointer to a file stream that is not open yet for
 *  the output
 * \param name a pointer to a string containing the base name of the output file
 *
 * \return 0 if a file could be succesfully extracted, -1 otherwise
 */
int lsbe_extract(FILE* image, FILE** msg, const char* name);


/* Extract an encrypted file from a .bmp image */
/**
 * \brief Extract a hidden encrypted file from a BMP file using LSB1 algorithm
 *
 * \param image a file stream to a BMPv3 file
 * \param msg_f a pointer to a file stream that is not open yet for
 *  the output
 * \param name a pointer to a string containing the base name of the output file
 * \param passwd the password to be used to decrypt the extracted file
 * \param algo the cipher algorithm to be used to decrypt the extracted file
 * \param blk_algo the cipher chaining mode of operation to be used to decrypt
 *  the extracted file
 *
 * \return 0 if a file could be succesfully extracted, -1 otherwise
 */
int lsb1_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char* passwd,
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo);
/**
 * \brief Extract a hidden encrypted file from a BMP file using LSB4 algorithm
 *
 * \param image a file stream to a BMPv3 file
 * \param msg_f a pointer to a file stream that is not open yet for
 *  the output
 * \param name a pointer to a string containing the base name of the output file
 * \param passwd the password to be used to decrypt the extracted file
 * \param algo the cipher algorithm to be used to decrypt the extracted file
 * \param blk_algo the cipher chaining mode of operation to be used to decrypt
 *  the extracted file
 *
 * \return 0 if a file could be succesfully extracted, -1 otherwise
 */
int lsb4_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char* passwd,
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo);
/**
 * \brief Extract a hidden encrypted file from a BMP file using LSB Enhanced algorithm
 *
 * \param image a file stream to a BMPv3 file
 * \param msg_f a pointer to a file stream that is not open yet for
 *  the output
 * \param name a pointer to a string containing the base name of the output file
 * \param passwd the password to be used to decrypt the extracted file
 * \param algo the cipher algorithm to be used to decrypt the extracted file
 * \param blk_algo the cipher chaining mode of operation to be used to decrypt
 *  the extracted file
 *
 * \return 0 if a file could be succesfully extracted, -1 otherwise
 */
int lsbe_crypt_extract(FILE* image, FILE** msg_f, const char* name, const char* passwd,
	const enum encrypt_type algo, const enum encrypt_block_type blk_algo);

#endif
