#ifndef BMP_TYPE_H_INCLUDED
#define BMP_TYPE_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

#define BMP_FILE_HEADER_SIZE 54
#define BMP_IMAGE_HEADER_SIZE 40
#define BMP_MAGIC_OFFSET 0x0000
#define BMP_HEADER_SIZE_OFFSET 0x000e
#define BMP_COMPRESSION_OFFSET 0x001e
#define BMP_PIXELS_MATRIX_SIZE_OFFSET 0x0022
#define BMP_PIXELS_MATRIX_ADDRESS_OFFSET 0x000a

/**
 * \struct bmp_type
 *
 * \brief describes a BMPv3 file
 */
struct bmp_type
{
    uint32_t compressed; /*!< indicates if the content is compressed (offset 0x001e in the file) */
    int32_t version;    /*!< indicates the version (3: BMPv3, -1: unsupported version) */
    uint32_t usable_size; /*!< indicates the size of the pixels matrix (in bytes) (offset 0x0022 in the file) */
    uint8_t header[BMP_FILE_HEADER_SIZE]; /*!< the complete header (54 first bytes) */
    uint8_t* matrix;  /*!< a pointer to the pixels matrix (from offset [0x000a] in the file) */
};


/**
 * \brief Load an BMPv3 header from a file stream into a bmp_type struct
 *
 * This function receives a correctly allocated bmp_type \a img and sets
 * its fields with the information from the first 54 bytes readable from \a in. 
 * It left the field \a img->matrix untouched (so it doesn't have to be allocated).
 *
 * \param in the file stream corresponding to an open BMPv3 file
 * \param img an allocated \a bmp_type structure
 */
int load_img_header(FILE* in, struct bmp_type* img);

/** 
 * \brief Load the matrix of pixels from a BMPv3 file into a bmp_type struct
 *
 * This functions receives a correctly allocated bmp_type \a img with an
 * allocated \a img->matrix field and loads the byte corresponding to the image
 * content from the file stream \a in to \a img.
 *
 * \param in the file stream corresponding to an open BMPv3 file 
 * \param img an allocated \a bmp_type structure with an allocated matrix field
 *
 * \warning This function doesn't make any checking on \a in. It's the respon-
 * sability of the programmer to load first the header in \a img and performs
 * the appropriate checking.
 */
int load_img_matrix(FILE* in, struct bmp_type* img);

/**
 * \brief Write down a BMP to a file
 *
 * This function receives a \a img containing both a header and a matrix of pixels
 * and write down to the file stream \a out. The output file is previously truncated.
 * 
 * \param out the output file stream which must be opened in writing mode
 * \param img a bmp_type containing both a loaded header and a loaded matrix of pixels
 */
int write_img(FILE* out, struct bmp_type* img);

#endif

