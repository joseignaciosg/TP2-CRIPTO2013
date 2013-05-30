#include <stdlib.h>
#include <stdio.h>
#include "../src/bmp.h"
#include "../src/stegobmp_write.h"

int testsimplewrite();
void testcompletewrite();

int main(void)
{
    testcompletewrite();
    return 0;
}

void testcompletewrite()
{
    FILE* img = fopen("test.bmp","r");
    FILE* out = fopen("test2.bmp","w");
    FILE* in = fopen("test.txt","r");

    lsb1_embed(img,in,".txt",out);

    fclose(in);
    fclose(out);
    fclose(img);
}

int testsimplewrite()
{
    struct bmp_type img;
    FILE* in = fopen("test.bmp","r");
    FILE* out = fopen("test2.bmp","w");
    if (out == NULL)
	return -2;


    if (load_img_header(in, &img))
	return -1;
    
    printf("version: %i, compressed: %i, usable_size: %i\n",img.version,img.compressed,img.usable_size);
    if (img.version == 3 && img.compressed == 0)
    {
	img.matrix = malloc(img.usable_size * sizeof(uint8_t));
	load_img_matrix(in,&img);
	

	lsb1_write_bytes("coucou",7,&img,NULL);

	write_img(out,&img);


	free(img.matrix);
    }
    printf("end\n");

    fclose(out);
    fclose(in);

    return 0;
}

