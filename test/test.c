#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../src/stegobmp_write.h"
#include "../src/crypt.h"


<<<<<<< HEAD

int main(int argc, char **argv)
=======
int main(int argc, char** argv)
>>>>>>> 0e11b0666ae4d52acb4d83c53fe484a98625e04a
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
        {"add", 1, 0, 0},
        {"append", 0, 0, 0},
        {"delete", 1, 0, 0},
        {"verbose", 0, 0, 0},
        {"create", 1, 0, 'c'},
        {"file", 1, 0, 0},
        {NULL, 0, NULL, 0}
    };
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "abc:d:012",
                 long_options, &option_index)) != -1) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
        case 0:
            printf ("option %s", long_options[option_index].name);
            if (optarg)
                printf (" with arg %s", optarg);
            printf ("\n");
            break;
        case '0':
        case '1':
        case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
              printf ("digits occur in two different argv-elements.\n");
            digit_optind = this_option_optind;
            printf ("option %c\n", c);
            break;
        case 'a':
            printf ("option a\n");
            aopt = 1;
            break;
        case 'b':
            printf ("option b\n");
            bopt = 1;
            break;
        case 'c':
            printf ("option c with value '%s'\n", optarg);
            copt = optarg;
            break;
        case 'd':
            printf ("option d with value '%s'\n", optarg);
            dopt = optarg;
            break;
        case '?':
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }

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

