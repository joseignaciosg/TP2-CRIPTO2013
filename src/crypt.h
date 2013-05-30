#ifndef CRYPT_H_INCLUDED
#define CRYPT_H_INCLUDED

#define SALT_LENGTH 8
#define ITER_NB 21

enum encrypt_type
{
    AES_128=0,
    AES_192,
    AES_256,
    DES
};

enum encrypt_block_type
{
    ECB=0,
    CFB,
    OFB,
    CBC
};


int crypt(const unsigned char* in, const int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, int* encrypted_size);

#endif

