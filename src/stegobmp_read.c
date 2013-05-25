#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stegobmp_read.h"
#include "util.h"


/*********************************************************************************/
/*              LSB1                         */
/*********************************************************************************/
int lsb1_extract(FILE* image, const char* extension, FILE *secret_msg)
{
    
    /*just for testing*/
    char ch;
    while( ( ch = fgetc(image) ) != EOF ){
        fputc(ch, secret_msg);   
    }
    fclose(secret_msg);
    printf("File copied successfully.\n");
    /*printf("The extension is... %s\n", get_filename_ext(image));*/
    return 0;
    /*return lsbX_embed(image, in, extension, out, lsb4_required_size_calculator, lsb4_write_bytes);*/
}

int extract(const char* bmp_file_name)
{
    FILE *image,*secret_msg;
    image = fopen(bmp_file_name, "r");
    if( image == NULL )
    {
       printf("The file specified does not exist\n");
       return 1;
    }
    secret_msg = fopen("secret_msg", "w");
    if( secret_msg == NULL )
    {
      fclose(secret_msg);
      return 1;
    }

    //char * extension = get_filename_ext(bmp_file_name);
    /*TODO: acá hay que ver a que función llamar*/
    if (lsb1_extract(image, NULL, secret_msg)){
       printf("Error extracting secret message  \n");
       return 1;
    }
 
    printf("Secret message extracted successfully.\n");
    fclose(image);
    return 0;
}

int main(int argc, char *argv[])
{
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        printf( "usage: %s image_file_name", argv[0] );
        return 1;
    }
    else 
    {
        return extract(argv[1]);;
    }
    
}


/*********************************************************************************/
/*				LSBE						 */
/*********************************************************************************/
