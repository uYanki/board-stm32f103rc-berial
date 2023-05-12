#ifndef __MD5_H__
#define __MD5_H__

#include <stdint.h>

#define TEST_MD5 1

typedef struct {
    uint32_t count[2];
    uint32_t state[4];
    uint8_t  buffer[64];
} MD5_CTX;

#define F(x, y, z)        ((x & y) | (~x & z))
#define G(x, y, z)        ((x & z) | (y & ~z))
#define H(x, y, z)        (x ^ y ^ z)
#define I(x, y, z)        (y ^ (x | ~z))
#define ROTATE_LEFT(x, n) ((x << n) | (x >> (32 - n)))

#define FF(a, b, c, d, x, s, ac)  \
    {                             \
        a += F(b, c, d) + x + ac; \
        a = ROTATE_LEFT(a, s);    \
        a += b;                   \
    }

#define GG(a, b, c, d, x, s, ac)  \
    {                             \
        a += G(b, c, d) + x + ac; \
        a = ROTATE_LEFT(a, s);    \
        a += b;                   \
    }

#define HH(a, b, c, d, x, s, ac)  \
    {                             \
        a += H(b, c, d) + x + ac; \
        a = ROTATE_LEFT(a, s);    \
        a += b;                   \
    }

#define II(a, b, c, d, x, s, ac)  \
    {                             \
        a += I(b, c, d) + x + ac; \
        a = ROTATE_LEFT(a, s);    \
        a += b;                   \
    }

extern void MD5Init(MD5_CTX* context);
extern void MD5Update(MD5_CTX* context, uint8_t* input, uint32_t inputlen);
extern void MD5Final(MD5_CTX* context, uint8_t digest[16]);

static void MD5Transform(uint32_t state[4], uint8_t block[64]);
static void MD5Encode(uint8_t* output, uint32_t* input, uint32_t len);
static void MD5Decode(uint32_t* output, uint8_t* input, uint32_t len);

void md5(uint8_t* src, int src_len, uint8_t* dst);
void md5s(uint8_t* src, int src_len, char* dst);

#endif  // __MD5_H__