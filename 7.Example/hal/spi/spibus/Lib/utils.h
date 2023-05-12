#ifndef __UTILS_H__
#define __UTILS_H__

///////////////////////////////////////////////// delay

#include "stm32f1xx_hal.h"

#define delay_ms(ms) HAL_Delay(ms)
#define delay_us(us) HAL_Delay_us(us)

static void HAL_Delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

///////////////////////////////////////////////// log

#include <stdio.h>

// separator
#define SEPARATOR30 "-----------------------------"
#define SEPARATOR60 "----------------------------------------------------------"

// print with newline
#define println(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__)

// print variable
#define printv(fmt, var) printf("[ line: %d | function: %s ] %s = " fmt "\r\n", __LINE__, __FUNCTION__, #var, var)

// print code
// #define print_code(code) printf("[ " #code "]"), code
#define print_code(code) println("[ " #code "]"), code

static void print_binary(uint8_t n)
{
    for (uint8_t i = 0x80; i > 0; i >>= 1)
        printf("%c", n & i ? '1' : '0');
    printf("\r\n");
}

///////////////////////////////////////////////// other

// number of elements in an array
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
// byte offset of member in structure
#define MEMBER_OFFSET(structure, member) ((int)&(((structure*)0)->member))
// size of a member of a structure
#define MEMBER_SIZE(structure, member) (sizeof(((structure*)0)->member))

#endif