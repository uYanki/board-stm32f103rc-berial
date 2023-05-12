#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdint.h>

#define TEST_CRC32 1

#ifdef __cplusplus
extern "C" {
#endif

uint32_t CRC32(const uint8_t* pString);
uint32_t CRC32n(const uint8_t* pString, int iLen);
uint32_t CRC32np(const uint8_t* pString, int iLen, uint32_t uPrevCRC);
uint64_t FNV64(const uint8_t* s);
uint64_t FNV64n(const uint8_t* s, int iLen);

#ifdef __cplusplus
}
#endif

#endif
