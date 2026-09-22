#include "../headers/blockcipher.h"

int block_encrypt(unsigned char *plaintext, int plaintext_len, unsigned
char *key, unsigned char *iv, unsigned char *ciphertext)
{
    /* Declare cipher context */
    EVP_CIPHER_CTX *ctx;

    /* Create and initialize the context */
    ctx = EVP_CIPHER_CTX_new();

    /* Initialize the decryption operation. */
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb8(), NULL, key, iv);

    /* Provide the message to be encrypted, and obtain the encrypted
       output. EVP_EncryptUpdate can be called multiple times if necessary */
    int written = 0;
    EVP_EncryptUpdate(ctx, ciphertext, &written, plaintext, plaintext_len);

    int written_final = 0;
    EVP_EncryptFinal_ex(ctx, ciphertext, &written_final);

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    return written + written_final;
}

int block_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned
char *key, unsigned char *iv, unsigned char *plaintext)
{
 /* Declare cipher context */
    EVP_CIPHER_CTX *ctx;

    int written = 0;

//  /* Create and initialize the context */
    ctx = EVP_CIPHER_CTX_new();

//  /* Initialize the decryption operation. */
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb8(), NULL, key, iv);

    /* Provide the message to be decrypted, and obtain the plaintext
    output. EVP_DecryptUpdate can be called multiple times if necessary. */
    EVP_DecryptUpdate(ctx, plaintext, &written, ciphertext, ciphertext_len);

    /* Finalize the decryption. Further plaintext bytes may be written at
    this stage. */
    int written_final = 0;
    EVP_DecryptFinal_ex(ctx, plaintext, &written_final);

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    return written + written_final;
}