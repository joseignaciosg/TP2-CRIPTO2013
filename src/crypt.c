#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "crypt.h"

static const EVP_CIPHER* derive_cipher(enum encrypt_type enc, enum encrypt_block_type blk)
{
    char cipher_name[13];
    OpenSSL_add_all_ciphers();

    switch (enc) {
    case AES_128:
	strncpy(cipher_name,"aes-128-",9);
	break;
    case AES_192:
	strncpy(cipher_name,"aes-192-",9);
	break;
    case AES_256:
	strncpy(cipher_name,"aes-256-",9);
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
	strncat(cipher_name,"cfb8",3);
	break;
    case OFB:
	strncat(cipher_name,"ofb8",3);
	break;
    case CBC:
	strncat(cipher_name,"cbc",3);
	break;
    default:
	return NULL;
    }

    return EVP_get_cipherbyname(cipher_name) ;
}

static int allocate_key_iv(unsigned char** k, unsigned char** iv, const EVP_CIPHER* cipher)
{
    *k = malloc(sizeof(unsigned char)*EVP_CIPHER_key_length(cipher));
    *iv = malloc(sizeof(unsigned char)*EVP_CIPHER_iv_length(cipher));
    return 0;
}

static int init_key_iv(unsigned char* k, unsigned char* iv, const unsigned char* passwd, const EVP_CIPHER* cipher)
{
    /* doesn't count the final \0 in passwd buffer */
    return !EVP_BytesToKey(cipher, EVP_md5(), NULL, passwd, strlen((char*)passwd), 1, k, iv);
}

static int crypt_decrypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* out_length, int mode)
{
    unsigned char* k;
    unsigned char* iv;
    int out_partial_length, out_final_length;
    EVP_CIPHER_CTX ctx;
    const EVP_CIPHER* cipher =  derive_cipher(enc, blk);

    if (cipher == NULL) {
	fprintf(stderr,"Unsupported cipherblock type\n");
	return -1;
    }

    allocate_key_iv(&k,&iv,cipher);
    init_key_iv(k,iv,passwd,cipher);

    /* encrypt or decrypt */
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, cipher, NULL, k, iv, mode);
    EVP_CipherUpdate(&ctx, out, &out_partial_length, in, in_length);
    EVP_CipherFinal_ex(&ctx, out+out_partial_length, &out_final_length);

    *out_length = out_partial_length + out_final_length;

    /* wiping key from memory (paranoid mode enabled) 
     * and cleaning up 
     * (TODO: this function should replace key bytes with random bytes
     * instead of a known sequence) */
    memcpy(k,out,EVP_CIPHER_key_length(cipher));
    free(k);
    free(iv);

    EVP_CIPHER_CTX_cleanup(&ctx);

    return 0;
}

/********************************************************************************/
/*                          Interface functions                                 */
/********************************************************************************/
int get_block_size_for_cipher(enum encrypt_type enc, enum encrypt_block_type blk)
{
    const EVP_CIPHER* cipher = derive_cipher(enc, blk);
    return cipher ? EVP_CIPHER_block_size(cipher) : -1 ;
}

int decrypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* out_length)
{
    return crypt_decrypt(in, in_length, passwd, enc, blk, out, out_length, 0);
}
	
int crypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* encrypted_size)
{
    return crypt_decrypt(in, in_length, passwd, enc, blk, out, encrypted_size, OP_CRYPT);
}

