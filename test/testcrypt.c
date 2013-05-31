#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "../src/crypt.h"

int main(void)
{
    char* in = "¡hola, soy yo, el testcrypt!";
    int in_length = strlen(in) + 1;
    char* passwd = "passwd";
    enum encrypt_type enc = AES_256;
    enum encrypt_block_type blk = CBC;

    char out[1024];
    int out_length;

    char rep[1024];
    int rep_length;

    crypt((unsigned char*) in, in_length, (unsigned char*) passwd, enc, blk, (unsigned char*) out, &out_length);
    decrypt((unsigned char*) out, out_length, (unsigned char*) passwd, enc, blk, (unsigned char*) rep, &rep_length);

    printf("rep: %s (%i)\n",rep,rep_length);

    return 0;
}

