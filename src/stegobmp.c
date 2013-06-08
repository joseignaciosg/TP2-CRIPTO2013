/**
 * \file stegobmp.c
 *
 * \brief Entry point of the Stegobmp program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "stegobmp.h"
#include "stegobmp_write.h"
#include "stegobmp_read.h"
#include "crypt.h"

/**
 * \brief Print a quick help to use the program
 */
void print_usage() {
    printf("Extraction Usage: \n");
    printf("\t --extract                : to extract hidden message\n");
    printf("\t -p bitmapfile            : root of the image with the hidden message\n");
    printf("\t --out outfile            : name of the extracted hidden message\n");
    printf("\t --steg  <LSB1|LSB4|LSBE>  : mode of extraction\n");
    printf("\n");
    printf("Embedding Usage: \n");
    printf("\t --embed                  : to embed hidden message\n");
    printf("\t -p bitmapfile            : root of the image whehe the message should be embedded\n");
    printf("\t --out outfile            : name of the resulting image\n");
    printf("\t --in outfile             : message to hide\n");
    printf("\t --steg  <LSB1|LSB4|LSBE>  : mode of embedding\n");
    printf("\n");
    printf(" Optional Parameters for encryption/decryption\n");
    printf("\t -a <aes128|aes192|aes256|des> : encryption mode\n");
    printf("\t -m <ecb|cfb|ofb|cbc>          : encryption block mode\n");
    printf("\t --pass password                : password\n");

}

/**
 * \brief entry point
 *
 * Parses the arguments and calls the appropriate high-level function.
 *
 * \param argc the number of arguments
 * \param argv the arguments
 *
 * \return the high-level called function returned value, i.e. 0 if ut was successfull,
 * !=0 if an error occured
 */
int main(int argc, char **argv)
{

    int c;


    const struct option long_options[] = {
	{"extract", 0, 0, EXTRACT},
	{"embed"  , 0, 0, EMBED},
	{"out"    , 1, 0, OUT},
	{"steg"   , 1, 0, STEG},
	{"pass"   , 1, 0, PASS},
	{"in"     , 1, 0, IN},
	{NULL     , 0, NULL, 0}
    };

    int option_index = 0;

    /*MODES*/
    int mode = -1;
    int steg = -1;

    enum encrypt_type encrypt_t = ENC_INVALID;
    enum encrypt_block_type encrypt_block_t = BLK_INVALID;
    char * in = NULL;
    char * out = NULL;
    char * bitmap = NULL;
    char * passwd = NULL;
    int crypt_flag = 0;


    while ((c = getopt_long(argc, argv, "p:a:m:", long_options, &option_index)) != -1) {
        switch (c) {
        case EXTRACT:
    		mode = EXTRACT;
    		break;
    	case EMBED:
    		mode = EMBED;
    		break;		
		case IN: 
			if (optarg){
				in = malloc(strlen(optarg) * sizeof(char));
				strcpy(in,optarg);
			}
			break;
		case OUT: 
			if (optarg){
				out = malloc(strlen(optarg) * sizeof(char));
				strcpy(out,optarg);
			}
			break;
		case STEG:
			if (optarg){
				if (strcmp("LSB1",optarg) == 0){
					steg = LSB1;
				}else if (strcmp("LSB4",optarg) == 0){
					steg = LSB4;
				}else if (strcmp("LSBE",optarg) == 0){
					steg = LSBE;
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
				}
			}
			else{
				printf("You must specify a steganography method \n");
				print_usage(); 
				exit(EXIT_FAILURE);
			}
			break;
        case 'a':
        	/*encrypt_type <aes128|aes192|aes256|des>*/
            if (optarg){
                crypt_flag = 1;
				if (strcmp("aes128",optarg) == 0){
				    encrypt_t = AES_128;
				}else if (strcmp("aes192",optarg) == 0){
					encrypt_t = AES_192;
				}else if (strcmp("aes256",optarg) == 0){
					encrypt_t = AES_256;
				}else if (strcmp("des",optarg) == 0){
					encrypt_t = DES;
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
				}
            }
            break; 
        case 'm':
         	/*encrypt_block_type <ecb|cfb|ofb|cbc>*/
            if (optarg){
                crypt_flag = 1;
				if (strcmp("ecb",optarg) == 0){
				    encrypt_block_t = ECB;
				}else if (strcmp("cfb",optarg) == 0){
					encrypt_block_t = CFB;
				}else if (strcmp("ofb",optarg) == 0){
					encrypt_block_t = OFB;
				}else if (strcmp("cbc",optarg) == 0){
					encrypt_block_t = CBC;
				}else{
					printf("ERROR: Invalid encryption block algorithm\n");
					print_usage(); 
					exit(EXIT_FAILURE);
				}
			}
            break;
        case PASS:
            if (optarg){
                crypt_flag = 1;
                passwd = malloc(strlen(optarg) * sizeof(char));
                strcpy(passwd,optarg);
            }
            break;
        case 'p':
        	if (optarg){
				bitmap = malloc(strlen(optarg) * sizeof(char));
				strcpy(bitmap,optarg);
			}else{
				print_usage(); 
				exit(EXIT_FAILURE);
			}
			break;
        case '?':
            break;
       	default: print_usage(); 
                 exit(EXIT_FAILURE);
        }
    }

    /*params validations*/
    if (bitmap == NULL){
	printf("ERROR: A bitmap file should be specified < -p bitmapfile > \n");
	print_usage(); 
	exit(EXIT_FAILURE);
    }

    if (in == NULL && mode == EMBED){
	printf("ERROR: In emded mode a message to hide should be specified (--in msgToHide)\n");
	print_usage(); 
	exit(EXIT_FAILURE);
    }

    if ( out == NULL ){
	printf("ERROR: A --out file should be specified \n");
	print_usage(); 
	exit(EXIT_FAILURE);
    }

    FILE* msg_f;
    FILE* in_f;
    FILE* bitmap_f;
    if ( crypt_flag  ) {
	if (encrypt_t == ENC_INVALID){
	    printf("INFO: Using default encryption type AES128 \n");
	    encrypt_t = AES_128;
	}
	if (encrypt_block_t  == BLK_INVALID){
	    printf("INFO: Using default encryption block type CBC \n");
	    encrypt_block_t = CBC;
	}
	if (passwd == NULL){
	    printf("ERROR: A password should be specified\n");
	    print_usage();
	    exit(EXIT_FAILURE);
	} 
    }


    char * extension = NULL;
    char * aux = NULL;
    /*----- Embedding -------*/
    if (mode == EMBED){
	aux = strrchr(in, '.');
	bitmap_f = fopen(bitmap,"rb");
	in_f = fopen(in,"rb");
	msg_f = fopen(out,"wb");
	if ( aux == NULL){
	    extension = malloc(strlen(".txt")* sizeof(char));
	    strcpy(extension,".txt"); 
	}else{
	    extension = malloc(strlen(aux)* sizeof(char));
	    strcpy(extension,aux); 
	}
	printf("extension: %s\n", extension);

	if (steg == LSB1){
	    if (crypt_flag){      
		printf("Embedding encrypted file with password: %s\n",passwd);
		lsb1_crypt_embed(bitmap_f, in_f, extension ,msg_f, passwd, encrypt_t, encrypt_block_t);          
	    }else{
		printf("Embedding unencrypted file\n" );
		lsb1_embed(bitmap_f, in_f, extension ,msg_f); 
	    }
	}else if (steg == LSB4){
	    if (crypt_flag){      
		printf("Embedding encrypted file with password: %s\n",passwd);
		lsb4_crypt_embed(bitmap_f, in_f, extension ,msg_f, passwd, encrypt_t, encrypt_block_t);          
	    }else{
		printf("Embedding unencrypted file\n" );
		lsb4_embed(bitmap_f, in_f,  extension ,msg_f); 
	    }		
	}else if (steg == LSBE){
	    if (crypt_flag){      
		printf("Embedding encrypted file with password: %s\n",passwd);
		lsbe_crypt_embed(bitmap_f, in_f, extension ,msg_f, passwd, encrypt_t, encrypt_block_t);          
	    }else{
		printf("%s\n","Embedding unencrypted file" );
		lsbe_embed(bitmap_f, in_f,  extension ,msg_f); 
	    }   
	}else{
	    printf("ERROR: A steganography mode should be specified <LSB1|LSB4|LSBE>\n");
	    print_usage(); 
	    exit(EXIT_FAILURE);
	}
	fclose(msg_f);
	fclose(bitmap_f);
	fclose(in_f);
	/*----- Extraction -------*/
    }else if (mode == EXTRACT){
	msg_f = NULL;
	bitmap_f = fopen(bitmap,"rb");
	if (steg == LSB1){
	    if (crypt_flag){      
		printf("Extracting encrypted with password: %s\n",passwd);
		lsb1_crypt_extract(bitmap_f, &msg_f, out, passwd, encrypt_t, encrypt_block_t);  
	    }else{
		printf("Extracting non cyphered message\n" );
		lsb1_extract(bitmap_f, &msg_f, out);
	    }
	}else if (steg == LSB4){
	    if (crypt_flag){      
		printf("Extracting encrypted with password: %s\n",passwd);
		lsb4_crypt_extract(bitmap_f, &msg_f, out, passwd, encrypt_t, encrypt_block_t);  
	    }else{
		printf("Extracting non cyphered message\n" );
		lsb4_extract(bitmap_f, &msg_f, out);
	    }
	}else if (steg == LSBE){
	    if (crypt_flag){      
		printf("Extracting encrypted with password: %s\n",passwd);
		lsbe_crypt_extract(bitmap_f, &msg_f, out, passwd, encrypt_t, encrypt_block_t);  
	    }else{
		printf("Extracting non cyphered message\n" );
		lsbe_extract(bitmap_f, &msg_f, out);
	    }
	}else{
	    printf("ERROR: A steganography mode should be specified <LSB1|LSB4|LSBE>\n");
	    print_usage(); 
	    exit(EXIT_FAILURE);
	}
	fclose(bitmap_f);
    }else{
	printf("ERROR: A mode should be specified < --extract | --embed >\n");
	print_usage(); 
	exit(EXIT_FAILURE);
    }


    free(out);
    free(bitmap);

    return 0;
}

