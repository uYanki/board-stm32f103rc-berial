#ifndef __SHA1_H__
#define __SHA1_H__

#include <stdint.h>

#define TEST_SHA1 1

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    uint8_t  buffer[64];
} SHA1_CTX;

#ifdef __cplusplus
extern "C" {
#endif

void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, uint8_t* data, uint32_t len);
void SHA1Final(uint8_t digest[20], SHA1_CTX* context);

void sha1(uint8_t* src, int srclen, uint8_t* des);
void sha1s(uint8_t* src, int srclen, char* des);

#ifdef __cplusplus
}
#endif

#endif
