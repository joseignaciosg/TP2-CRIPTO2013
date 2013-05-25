#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"


const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}


int main(void)
{
	printf("%s\n", get_filename_ext("test.tiff"));
	printf("%s\n", get_filename_ext("test.blah.tiff"));
	printf("%s\n", get_filename_ext("test."));
	printf("%s\n", get_filename_ext("test"));
	printf("%s\n", get_filename_ext("..."));
}

