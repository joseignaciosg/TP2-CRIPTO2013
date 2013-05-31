#include <stdio.h>
#include "../src/util.h"

int main(void)
{
	printf("%s\n", get_filename_ext("test.tiff"));
	printf("%s\n", get_filename_ext("test.blah.tiff"));
	printf("%s\n", get_filename_ext("test."));
	printf("%s\n", get_filename_ext("test"));
	printf("%s\n", get_filename_ext("..."));

	return 0;
}


