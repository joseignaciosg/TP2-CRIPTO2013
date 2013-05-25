#ifndef CRYPT_H_INCLUDED
#define CRYPT_H_INCLUDED

enum encrypt_type
{
    AES_128,
    AES_192,
    AES_256,
    DES
};

enum encrypt_block_type
{
    ECB,
    CFB,
    OFB,
    CBC
};




#endif

