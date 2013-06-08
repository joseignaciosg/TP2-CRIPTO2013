#ifndef CRYPT_H_INCLUDED
#define CRYPT_H_INCLUDED

#define SALT_LENGTH 8
#define ITER_NB 21
#define OP_DECRYPT 0
#define OP_CRYPT 1

/**
 * \enum encrypt_type
 *
 * \brief Each value is a supported block encryption algorithm
 *
 * \warning DES is now unsecure and should be avoided for new applications
 */
enum encrypt_type
{
    AES_128=0, /*!< AES 128 bits */
    AES_192,   /*!< AES 192 bits */
    AES_256,   /*!< AES 256 bits */
    DES,       /*!< DES */
    ENC_INVALID    /*!< pseudo-value to handle invalid cases */
};

/**
 * \enum encrypt_block_type
 *
 * \brief Each value is a chaining mode for block encryption
 *
 * \warning ECB mode is not CCA-secure and should not be used
 */
enum encrypt_block_type
{
    ECB=0,   /*!< Electronic CodeBook */
    CFB,     /*!< Cipher FeedBack */
    OFB,     /*!< Output FeedBack */
    CBC,     /*!< Cipher-Block Chaining */
    BLK_INVALID    /*!< pseudo-value to handle invalid cases */
};

/**
 * \brief Encrypts a chunk of data
 *
 * This functions receives a chunk of data and a correctly allocated buffer to 
 * put the corresponding encrypted chunk. It also returns the exact encrypted length.
 *
 * \param in the buffer containing the data to be encrypted
 * \param in_length the length (in bytes) of the input data
 * \param passwd the password from which the key and the IV must be derived
 * \param enc the cipher algorithm to be used
 * \param blk the cipher mode of operation to be used
 * \param out the buffer to which the encrypted data must be written
 * \param encrypted_size a pointer in which the encrypted size is written
 *
 * \return 0 if \a in could be successfully encrypted and -1 if an error occured
 */
int crypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* encrypted_size);

/**
 * \brief Decrypts a chunk of data
 *
 * This functions receives a chunk of data and a correctly allocated buffer to 
 * put the corresponding decrypted chunk. It also returns the exact decrypted length.
 *
 * \param in the buffer containing the data to be decrypted
 * \param in_length the length (in bytes) of the input data
 * \param passwd the password from which the key and the IV must be derived
 * \param enc the cipher algorithm to be used
 * \param blk the cipher mode of operation to be used
 * \param out the buffer to which the decrypted data must be written
 * \param decrypted_size a pointer in which the decrypted size is written
 *
 * \return 0 if \a in could be successfully decrypted and -1 if an error occured
 */
int decrypt(const unsigned char* in, const unsigned int in_length, const unsigned char* passwd, enum encrypt_type enc, enum encrypt_block_type blk, unsigned char* out, unsigned int* decrypted_size);

/**
 * \brief Give the block size (in bytes) used for a given cipher
 *
 * \param enc the cipher algorithm
 * \param blk the cipher mode of operation
 *
 * \return the block size or -1 if the cipher is not supported
 */
int get_block_size_for_cipher(enum encrypt_type enc, enum encrypt_block_type blk);
#endif

