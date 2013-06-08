#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include "bmp.h"

/**
 * \brief extracts the extension from a filename
 *  
 *  The returned pointer points to a location inside filename,
 *  possibly the final '\0' if the filename has no extension.
 *
 * \param filename the name of the file
 *
 * \return a constant pointer to the extension
 */
const char *get_filename_ext(const char *filename);

/**
 * \brief checks that the version of a BMP is supported
 *
 * \param img a bmp_type with a loaded header
 *
 * \return 0 if the version is not supported, >0 otherwise
 */
int check_version(const struct bmp_type* img);

/**
 * \brief checks whether a BMP is compressed or not
 *
 * \param img a bmp_type with a loaded header
 *
 * \return 0 if the file is compressed, 1 otherwise
 */
int check_compression(const struct bmp_type* img);

/**
 * \brief retrieves a file size
 *
 * This functions saves the internal pointer of the file stream
 * and restore it at the end.
 *
 * \param file an open file stream
 *
 * \return the file size, in bytes
 */
int get_file_size(FILE* file);


#endif

