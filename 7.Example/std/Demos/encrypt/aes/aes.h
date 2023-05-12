/*
This optimized AES implementation conforms to FIPS-197,
and has been certified by NIST (cert. #198, NCSCAES).
*/

#ifndef __AES_H__
#define __AES_H__

#include <stdint.h>

#define TEST_AES 1

typedef struct {
    uint32_t erk[64]; /* encryption round keys */
    uint32_t drk[64]; /* decryption round keys */
    int      nr;      /* number of rounds */
} aes_context;

int  aes_set_key(aes_context* ctx, uint8_t* key, int nbits);
void aes_encrypt(aes_context* ctx, uint8_t input[16], uint8_t output[16]);
void aes_decrypt(aes_context* ctx, uint8_t input[16], uint8_t output[16]);

#endif /* aes.h */
