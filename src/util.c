/**
 * \file util.c
 *
 * \brief Provide utilitary functions for checking and file manipulation
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"
#include "bmp.h"


const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) 
	return (filename + strlen(filename)); /* return a pointer to the \0 */
    return dot + 1;
}

/* 
 * Fetches file size. The internal pointer of the stream is restored to its position.
 */
int get_file_size(FILE* file)
{
    int size;
    int current_position = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, current_position, SEEK_SET);
    return size;
}

/*********************************************************************************/
/*			    Checking functions					 */
/*********************************************************************************/
int check_version(const struct bmp_type* img)
{
    return img->version != -1;
}

int check_compression(const struct bmp_type* img)
{
    return img->compressed == 0;
}

