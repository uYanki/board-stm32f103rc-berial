#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h>
#include "stm32f10x.h"

#define USE_UART1         1
#define USE_UART2         1

#define println(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__)
#define ARRAY_SIZE(arr)   (sizeof(arr) / sizeof(arr[0]))

void board_init(void);

#endif