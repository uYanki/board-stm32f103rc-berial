#ifndef __BSP_DMA_H__
#define __BSP_DMA_H__

#include <stm32f10x.h>

#define USART_DR	0x40013804

extern unsigned char arr[2000];

void DMA_Configuration(void);

#endif
