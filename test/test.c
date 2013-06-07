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

void print_usage() {
    printf("Usage: \n");
}


int main(int argc, char **argv)
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
 *     free(out.matrix);  

*/
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
        {"extract", 0, 0, EMBED},
        {"out"    , 1, 0, OUT},
        {"steg"   , 1, 0, STEG},
        {"pass"   , 1, 0, PASS},
        {"in"     , 1, 0, IN},
        {NULL     , 0, NULL, 0}
    };

    int option_index = 0;

    /*MODE*/
    int mode = -1;
    char * in;
    char * out;
    char * bitmap;

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
			}
			break;
		case OUT: 
			if (optarg){
				out = malloc(strlen(optarg) * sizeof(char));
				strcpy(out,optarg);
			}
			break;
        case STEG:
       		printf("steg!\n");
			if (optarg)
				if (strcmp("LSB1",optarg) == 0){
					printf("LSB1\n");
				}else if (strcmp("LSB4",optarg) == 0){
					printf("LSB4");
				}else if (strcmp("LSBE",optarg) == 0){
					printf("LSBE");
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
				}
			break;
		case PASS:
			if (optarg)
				printf (" with arg %s", optarg);
			break;
        case 'a':
        	//<aes128|aes192|aes256|des>
            if (optarg)
				if (strcmp("aes128",optarg) == 0){
					printf("aes128\n");
				}else if (strcmp("aes192",optarg) == 0){
					printf("aes192");
				}else if (strcmp("aes256",optarg) == 0){
					printf("aes256");
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
				}
            break;
        case 'm':
         	//<ecb|cfb|ofb|cbc>
            if (optarg)
				if (strcmp("ecb",optarg) == 0){
					printf("ecb\n");
				}else if (strcmp("cfb",optarg) == 0){
					printf("cfb");
				}else if (strcmp("ofb",optarg) == 0){
					printf("ofb");
				}else if (strcmp("cbc",optarg) == 0){
					printf("cbc");
				}else{
					print_usage(); 
					exit(EXIT_FAILURE);
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

    if (mode == EMBED){
    	//llamada a funcion
    }else if (mode == EXTRACT){
    	//llamda a funcion
    	printf("%s --- %s --- %s \n", in, out, bitmap);
    }else{
    	print_usage(); 
		exit(EXIT_FAILURE);
    }


    free(in);
    free(out);
    free(bitmap);
    return 0;


//     FILE* img = fopen("../files/example/sherlock1.bmp","rb");
// /*     FILE* in = fopen("1","rb");
//  *     FILE* out = fopen("out.bmp","wb"); */
//     FILE* recovered_msg = NULL;

//      lsb1_embed(img, in, ".txt",out); |+,"passwd",AES_128,CBC);+| 

// /*     out = freopen("out.bmp","rb",out); */
//     lsb1_extract(img, &recovered_msg, "out");


//     fclose(img);
// /*     fclose(in);
//  *     fclose(out); */
//     fclose(recovered_msg);

//     return 0;
    return 0;
}

