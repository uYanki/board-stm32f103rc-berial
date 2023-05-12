#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static void print_binary(uint8_t n)
{
    uint8_t i = 0x80;
    do {
        printf("%c", n & i ? '1' : '0');
    } while (i >>= 1);
}

///////////////////////////////////////////////// other

static uint8_t calc_crc(uint8_t arr[], uint8_t len)
{
    uint8_t i, j, byte, carry, crc = 0;
#if 0
    for (i = 0; i < len; ++i) {
        byte = arr[i];
        for (j = 8; j != 0; --j) {
            carry = (crc ^ byte) & 0x80;
            crc <<= 1;
            if (carry) crc ^= 0x7;
            byte <<= 1;
        }
    }
#else
    for (i = 0; i < len; ++i) {
        byte = crc ^ arr[i];
        for (j = 0; j < 8; ++j) {
            carry = byte & 0x80;
            byte <<= 1;
            if (carry) byte ^= 0x7;
        }
        crc = byte;
    }
#endif
    return crc;
}

#endif
