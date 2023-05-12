#ifndef __PINCTRL_H__
#define __PINCTRL_H__

#include "stm32f10x.h"

#define BIT_ADDR(addr, bit) *((volatile uint32_t*)((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bit << 2)))

#define GPIOA_ODR_ADDR      (GPIOA_BASE + 12)
#define GPIOB_ODR_ADDR      (GPIOB_BASE + 12)
#define GPIOC_ODR_ADDR      (GPIOC_BASE + 12)
#define GPIOD_ODR_ADDR      (GPIOD_BASE + 12)
#define GPIOE_ODR_ADDR      (GPIOE_BASE + 12)
#define GPIOF_ODR_ADDR      (GPIOF_BASE + 12)
#define GPIOG_ODR_ADDR      (GPIOG_BASE + 12)

#define GPIOA_IDR_ADDR      (GPIOA_BASE + 8)
#define GPIOB_IDR_ADDR      (GPIOB_BASE + 8)
#define GPIOC_IDR_ADDR      (GPIOC_BASE + 8)
#define GPIOD_IDR_ADDR      (GPIOD_BASE + 8)
#define GPIOE_IDR_ADDR      (GPIOE_BASE + 8)
#define GPIOF_IDR_ADDR      (GPIOF_BASE + 8)
#define GPIOG_IDR_ADDR      (GPIOG_BASE + 8)

#define PAout(n)            BIT_ADDR(GPIOA_ODR_ADDR, n)
#define PAin(n)             BIT_ADDR(GPIOA_IDR_ADDR, n)
#define PBout(n)            BIT_ADDR(GPIOB_ODR_ADDR, n)
#define PBin(n)             BIT_ADDR(GPIOB_IDR_ADDR, n)
#define PCout(n)            BIT_ADDR(GPIOC_ODR_ADDR, n)
#define PCin(n)             BIT_ADDR(GPIOC_IDR_ADDR, n)
#define PDout(n)            BIT_ADDR(GPIOD_ODR_ADDR, n)
#define PDin(n)             BIT_ADDR(GPIOD_IDR_ADDR, n)
#define PEout(n)            BIT_ADDR(GPIOE_ODR_ADDR, n)
#define PEin(n)             BIT_ADDR(GPIOE_IDR_ADDR, n)
#define PFout(n)            BIT_ADDR(GPIOF_ODR_ADDR, n)
#define PFin(n)             BIT_ADDR(GPIOF_IDR_ADDR, n)
#define PGout(n)            BIT_ADDR(GPIOG_ODR_ADDR, n)
#define PGin(n)             BIT_ADDR(GPIOG_IDR_ADDR, n)

void _wfi(void);
void _enable_irq(void);
void _disable_irq(void);

#endif
