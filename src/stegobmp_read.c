#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>
#include "bmp.h"
#include "util.h"
#include "crypt.h"
#include "stegobmp.h"
#include "stegobmp_read.h"


int lsb1_extract(FILE* image, FILE* msg_f)
{
    char* msg;
    struct bmp_type img;
    unsigned int offset = 0,i=0,size;
    char hidden;
    uint32_t msg_size;

    /* header loading */
    load_img_header(image, &img);
       
    /* content loading */
    img.matrix = malloc(sizeof(uint8_t)*img.usable_size);
    load_img_matrix(image, &img);

    /*reading size*/
    for(i=0; i<4*8; i++){
        hidden = img.matrix[offset];
        offset++;
        *(((char*)&msg_size)+i/8) = BIT(hidden,0);
    }

    /*reading content*/
    msg = calloc(msg_size, sizeof(char));
    for(i=0; i<msg_size*8; i++){
        hidden = img.matrix[offset];
        *(msg+i/8) = BIT(hidden,0);
        offset++;
    }
    msg[i/8]=0;
    printf("hidden message: %s \n", msg);

    /*reading extension*/
    return 0;



}


// int lsb1_read_bytes( void* msg, struct bmp_type* img, unsigned int* start_offset)
// {

//     int i,j,size=0;
//     uint8_t* to_be_written = (uint8_t*) in;
//     unsigned int offset = start_offset ? *start_offset : 0;
//     char c, hidden, *msg;

//     /*levanto el size*/
//     for(i=0; i<4*8; i++){
//         hidden = fgetc(img->matrix[offset]);
//         offset++;
//         *(((char*)&size)+i/8)|=((hidden&1)<<7-(i%8));
//     }

//     /*lavanto el contenido*/
//     msg = calloc(size+4, sizeof(char));
//     memcpy(msg, &size, 4);
//     msg+=4;
//     for(i=0; i<(size-4)*8; i++){
//         hidden = fgetc(img->matrix[offset]);
//         *(msg+i/8)|=((hidden&1)<<7-(i%8));
//         offset++;
//     }
//     msg[i/8]=0;
//     msg-4;

//     int i,j;
//     uint8_t* to_be_read = (uint8_t*) img->matrix[offset];
//     unsigned int offset = start_offset ? *start_offset : 0;

//     for (i=0 ; i<size ; i++)
//     {
//         if (BIT(to_be_read[i],j))
//             msg |= (uint8_t) 1;
//         else
//             msg &= (uint8_t) ~1;
//         offset++;

//     }
//     if (start_offset)
//     *start_offset = offset;

//     return 0;
// }


