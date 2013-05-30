#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "crypt.h"

static const EVP_CIPHER* derive_cipher(enum encrypt_type enc, enum encrypt_block_type blk)
{
    char cipher_name[12];
    OpenSSL_add_all_ciphers();

    switch (enc) {
    case AES_128:
	strncpy(cipher_name,"aes-128-",9);
	break;
    case AES_192:
	strncpy(cipher_name,"aes-128-",9);
	break;
    case AES_256:
	strncpy(cipher_name,"aes-128-",9);
	break;
    case DES:
	strncpy(cipher_name,"des-",5);
	break;
    default:
	return NULL;
    }

    switch (blk) {
    case ECB:
	strncat(cipher_name,"ecb",3);
	break;
    case CFB:
	strncat(cipher_name,"cfb",3);
	break;
    case OFB:
	strncat(cipher_name,"ofb",3);
	break;
    case CBC:
	strncat(cipher_name,"cbc",3);
	break;
    default:
	return NULL;
    }

    return EVP_get_cipherbyname(cipher_name);
}

static int allocate_key_iv_salt(unsigned char** k, unsigned char** iv, unsigned char** salt, const EVP_CIPHER* cipher)
{
    *k = malloc(sizeof(unsigned char)*EVP_CIPHER_key_length(cipher));
    *iv = malloc(sizeof(unsigned char)*EVP_CIPHER_iv_length(cipher));
    *salt = malloc(sizeof(unsigned char)*SALT_LENGTH);
    return 0;
}

static int init_key_iv_salt(unsigned char** k, unsigned char** iv, unsigned char** salt, const unsigned char* passwd, const EVP_CIPHER* cipher)
{
    FILE* f = fopen("/dev/urandom","r");
    fread(*salt,sizeof(unsigned char),SALT_LENGTH,f);
    fclose(f);

    /* doesn't count the final \0 in passwd buffer */
    return !EVP_BytesToKey(cipher, EVP_sha1(), *salt, passwd, strlen((char*)passwd), ITER_NB, *k, *iv);
}

int crypt(const unsigned char* in, const int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, int* encrypted_size)
{
    unsigned char* k;
    unsigned char* iv;
    unsigned char* salt;
    int out_length, out_final_length;
    const EVP_CIPHER* cipher =  derive_cipher(enc, blk);
    EVP_CIPHER_CTX ctx;

    if (cipher == NULL) {
	fprintf(stderr,"Unsupported cipherblock type");
	return -1;
    }

    allocate_key_iv_salt(&k,&iv,&salt,cipher);
    init_key_iv_salt(&k,&iv,&salt,passwd,cipher);

    /*encrypt*/
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, cipher, NULL, k, iv);
    EVP_EncryptUpdate(&ctx, out, &out_length, in, in_length);
    EVP_EncryptFinal_ex(&ctx, out+out_length, &out_final_length);

    *encrypted_size = out_length + out_final_length;

    memset(k,0,EVP_CIPHER_key_length(cipher));
    free(k);
    memset(iv,0,EVP_CIPHER_iv_length(cipher));
    free(iv);
    free(salt);

    EVP_CIPHER_CTX_cleanup(&ctx);

    return 0;
}

