#include <stdio.h>
#include "bmp.h"

int load_img_header(FILE* in, struct bmp_type* img)
{
    char magic[2];
    uint32_t header_size;
    
    /* version is "3" if the magic number is "DM"
     * and the header size is equal to 40 */
    fseek(in, 0x0000, SEEK_SET);
    fread(magic, sizeof(char), 2, in);
    fseek(in, 0x000e, SEEK_SET);
    fread(&header_size, sizeof(uint32_t), 1, in);
    if (magic[0] == 'B' && magic[1] == 'M' && header_size == 40)
	img->version = 3;
    else
	img->version = -1;

    /* BMP file is uncompressed if [0x00e1] == 0 */
    fseek(in, 0x001e, SEEK_SET);
    fread(&(img->compressed), sizeof(uint32_t), 1, in);

    /* BMP pixel array size is at 0x0022 */
    fseek(in, 0x0022, SEEK_SET);
    fread(&(img->usable_size), sizeof(uint32_t), 1, in);

    return 0;
}

int load_img_matrix(FILE* in, struct bmp_type* img)
{
    uint32_t offset;
    
    /* pixel array starts at offset [0x000a] */
    fseek(in, 0x000a, SEEK_SET);
    fread(&offset, sizeof(uint32_t), 1, in);

    fseek(in, offset, SEEK_SET);
    fread(&(img->matrix), sizeof(uint8_t), img->usable_size, in);

    return 0;
}

