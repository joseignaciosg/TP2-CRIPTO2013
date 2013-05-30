#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../src/stegobmp_write.h"


int main(void)
{
/*     char in = 0x0F;
 *     struct bmp_type out;

 *     out.matrix = malloc(sizeof(char)*8);
 *     memset(out.matrix,'h',8);

 *     printf("out.matrix: %s\n",out.matrix);
 *     lsb1_write_bytes(&in, sizeof(in), &out, 0);
 *     printf("out.matrix: %s\n",out.matrix);


 *     memset(out.matrix,'d',8);

 *     printf("out.matrix: %s\n",out.matrix);
 *     lsb4_write_bytes(&in, sizeof(in), &out, 0);
 *     FILE* file = fopen("1","wb");
 *     fprintf(file,"out.matrix: %s\n",out.matrix);
 *     fclose(file);
 *     
 *     free(out.matrix);                            */


    FILE* img = fopen("test.bmp","rb");
    FILE* in = fopen("1","rb");
    FILE* out = fopen("out.bmp","wb");

    lsb1_embed(img, in, ".txt",out);

    fclose(img);
    fclose(in);
    fclose(out);

    return 0;
}

