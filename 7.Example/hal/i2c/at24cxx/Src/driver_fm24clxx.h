
#ifndef DRIVER_FM24CLXX_H
#define DRIVER_FM24CLXX_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FM24CL04B = 511,   // FM24CL04B type
    FM24CL16B = 2047,  // FM24CL16B type
    FM24CL64B = 8191,  // FM24CL64B type
} fm24clxx_t;

typedef enum {
    FM24CLXX_ADDRESS_A000 = 0,  // A2A1A0 000
    FM24CLXX_ADDRESS_A001 = 1,  // A2A1A0 001
    FM24CLXX_ADDRESS_A010 = 2,  // A2A1A0 010
    FM24CLXX_ADDRESS_A011 = 3,  // A2A1A0 011
    FM24CLXX_ADDRESS_A100 = 4,  // A2A1A0 100
    FM24CLXX_ADDRESS_A101 = 5,  // A2A1A0 101
    FM24CLXX_ADDRESS_A110 = 6,  // A2A1A0 110
    FM24CLXX_ADDRESS_A111 = 7,  // A2A1A0 111
} fm24clxx_address_t;

#ifdef __cplusplus
}
#endif
#endif
