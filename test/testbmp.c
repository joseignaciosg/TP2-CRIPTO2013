#include <stdio.h>
#include "../src/bmp.h"

int main(void)
{
    struct bmp_type img;
    FILE* in = fopen("test.bmp","rb");

    load_img_header(in, &img);

    printf("version: %i, compressed: %i, usable_size: %i\n",img.version,img.compressed,img.usable_size);

    return 0;
}

