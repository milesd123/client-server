#include "headers.h"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

int stream_encrypt(unsigned char *plaintext, int plaintext_len, unsigned
char *key, unsigned char *iv, unsigned char *ciphertext);

int stream_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned
char *key, unsigned char *iv, unsigned char *plaintext);

