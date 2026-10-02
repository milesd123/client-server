#include "../headers/hmac.h"

unsigned cal_hmac(unsigned char *mac, char *message)
{
    /* The secret key for hashing */
    const char key[20] = "SecretHashingKeyAAA";
    
    /* Change the length accordingly with your chosen hash engine.
    * Be careful of the length of string with the chosen hash engine. For
    example, SHA1 needed 20 characters. */
    unsigned int len = 20;
    
    /* Create and initialize the context */
    HMAC_CTX *ctx;
    ctx = HMAC_CTX_new();
   
    /* Initialize the HMAC operation. */
    HMAC_Init_ex(ctx, (const void*) &key, len, EVP_sha256(), NULL);
   
    /* Provide the message to HMAC, and start HMAC authentication. */
    HMAC_Update(ctx, message, strlen(message));
    

    /* HMAC_Final() writes the hashed values to md, which must have enough
    space for the hash function output. */
    unsigned int outlen = 0;

    HMAC_Final(ctx, mac, &outlen);
    /* Releases any associated resources and finally frees context variable
    */
    HMAC_CTX_free(ctx);

    return outlen;
}