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

static int init_key_iv_salt(unsigned char* k, unsigned char* iv, unsigned char* salt, const unsigned char* passwd, const EVP_CIPHER* cipher)
{
    /* For now, don't use a real randomized salt */
    memcpy(salt,"gJxe@b4Z",8);
    /* doesn't count the final \0 in passwd buffer */
    return !EVP_BytesToKey(cipher, EVP_sha1(), salt, passwd, strlen((char*)passwd), ITER_NB, k, iv);
}

static int crypt_decrypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* encrypted_size, int mode)
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
    init_key_iv_salt(k,iv,salt,passwd,cipher);

    /*encrypt*/
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, cipher, NULL, k, iv, mode);
    EVP_CipherUpdate(&ctx, out, &out_length, in, in_length);
    EVP_CipherFinal_ex(&ctx, out+out_length, &out_final_length);

    *encrypted_size = out_length + out_final_length;

    memset(k,0,EVP_CIPHER_key_length(cipher));
    free(k);
    free(iv);
    free(salt);

    EVP_CIPHER_CTX_cleanup(&ctx);

    return 0;
}

int get_block_size_for_cipher(enum encrypt_type enc, enum encrypt_block_type blk)
{
    return EVP_CIPHER_block_size(derive_cipher(enc, blk));
}

int decrypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* out_length)
{
    return crypt_decrypt(in, in_length, passwd, enc, blk, out, out_length, 0);
}
	
int crypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* encrypted_size)
{
    return crypt_decrypt(in, in_length, passwd, enc, blk, out, encrypted_size, OP_CRYPT);
}

