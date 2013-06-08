#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../src/stegobmp_write.h"
#include "../src/crypt.h"


/*values*/
#define EXTRACT 1
#define EMBED   2
#define STEG    3
#define OUT     4
#define PASS    5
#define IN      6
#define LSB1    7
#define LSB4    8
#define LSBE    9

void print_usage() {
    printf("Estraction Usage: \n");
    printf("\t --extract                : to extract hidden message\n");
    printf("\t -p bitmapfile            : root of the image with the hidden messahe\n");
    printf("\t --out outfile            : name of the extracted hidden message\n");
    printf("\t -steg  <LSB1|LSB4|LSBE>  : mode of estraction\n");
    printf("\n");
    printf("Embedding Usage: \n");
    printf("\t --embed                  : to embed hidden message\n");
    printf("\t -p bitmapfile            : root of the image whehe the message should be embedded\n");
    printf("\t --out outfile            : name of the resulting image\n");
    printf("\t --in outfile             : message to hide\n");
    printf("\t -steg  <LSB1|LSB4|LSBE>  : mode of embedding\n");
}



int main(int argc, char **argv)
{

 	struct option {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
	};
	
	int c;
    int digit_optind = 0;
    int aopt = 0, bopt = 0;
    char *copt = 0, *dopt = 0;
    
    static struct option long_options[] = {
        {"extract", 0, 0, EXTRACT},
        {"embed", 0, 0, EMBED},
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



    enum encrypt_type encrypt_t = -1;
    enum encrypt_block_type encrypt_block_t = -1;
    char * in;
    char * out;
    char * bitmap;
    char * passwd;


    while ((c = getopt_long(argc, argv, "p:a:m:",
                 long_options, &option_index)) != -1) {
        int this_option_optind = optind ? optind : 1;
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
			}else{
				if (mode != EMBED){
					print_usage(); 
					exit(EXIT_FAILURE);
				}
			}
			break;
		case OUT: 
			if (optarg){
				out = malloc(strlen(optarg) * sizeof(char));
				strcpy(out,optarg);
			}else{
				print_usage(); 
				exit(EXIT_FAILURE);
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
				printf("You must specify and steganography method \n");
				print_usage(); 
				exit(EXIT_FAILURE);
			}
			break;
        case 'a':
        	//encrypt_type <aes128|aes192|aes256|des>
            if (optarg){
				if (strcmp("aes128",optarg) == 0){
                    encrypt_t = AES_128;
				}else if (strcmp("aes192",optarg) == 0){
					encrypt_t = AES_192;
				}else if (strcmp("aes256",optarg) == 0){
					encrypt_t = AES_256;
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
				}
            }
            break;
        case 'm':
         	//encrypt_block_type <ecb|cfb|ofb|cbc>
            if (optarg){
				if (strcmp("ecb",optarg) == 0){
                    encrypt_block_t = ECB;
				}else if (strcmp("cfb",optarg) == 0){
					encrypt_block_t = CFB;
				}else if (strcmp("ofb",optarg) == 0){
					encrypt_block_t = OFB;
				}else if (strcmp("cbc",optarg) == 0){
					encrypt_block_t = CBC;
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
				}
			}
            break;
        case PASS:
            if (encrypt_block_t && encrypt_t){
                if (optarg){
                    passwd = malloc(strlen(optarg) * sizeof(char));
                    strcpy(bitmap,optarg);
                }else{
                    printf("%s\n", "A password should be specified");
                    print_usage(); 
                    exit(EXIT_FAILURE);
                }  
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
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }
    
    FILE* msg_f;
    FILE* in_f;
    FILE* bitmap_f;
    /*----- Embedding -------*/
    if (mode == EMBED){
    	bitmap_f = fopen(bitmap,"rb");
    	in_f = fopen(in,"rb");
		msg_f = fopen(out,"wb");
        /*TODO poner la extensión correcta*/
    	if (steg == LSB1){
            if (encrypt_t != -1){
                lsb1_embed(bitmap_f, in_f,".txt",msg_f);                 
            }else{
                printf("%s\n","llamando a crypt" );
                lsb1_crypt_embed(bitmap_f, in_f,".txt",msg_f, passwd, encrypt_t, encrypt_block_t);
            }
    	}else if (steg == LSB4){
    		lsb4_embed(bitmap_f, in_f, ".txt",msg_f); 
    	}else if (steg == LSBE){
    		lsbe_embed(bitmap_f, in_f, ".txt",msg_f); 
    	}else{
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
    		lsb1_extract(bitmap_f, &msg_f, out);
    	}else if (steg == LSB4){
    		lsb4_extract(bitmap_f, &msg_f, out);
    	}else if (steg == LSBE){
    		lsbe_extract(bitmap_f, &msg_f, out);
    	}else{
    		print_usage(); 
			exit(EXIT_FAILURE);
    	}
    	fclose(bitmap_f);
    }else{
    	print_usage(); 
		exit(EXIT_FAILURE);
    }

   
    free(out);
    free(bitmap);

    return 0;
}

