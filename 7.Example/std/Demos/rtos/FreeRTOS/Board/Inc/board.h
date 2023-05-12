#ifndef __BOARD_H__
#define __BOARD_H__

#include "board_conf.h"

#include "FreeRTOS.h"

#include <stdio.h>
#include <stdbool.h>

#include "stm32f10x.h"

#include "debug.h"

#define INLINE __inline

// gpio

#if USE_USER_LED
#define LED_GPIO_Port GPIOA
#define LED_GPIO_Pin  GPIO_Pin_8
#endif

#if USE_USER_KEY
#define KEY_GPIO_Port GPIOA
#define KEY_GPIO_Pin  GPIO_Pin_0
#endif

// uart

#define USE_UART (USE_UART1 || USE_UART2)

#if USE_IRQ_U1RX

// 会和串口2的发送有冲突

/**
 * @brief uart1 rx flag:
 * - @val 0: waiting for a newline character to be received ('\n')
 * - @val 1: receiving completed
 */
extern uint8_t uart1_rxflg;
/**
 * @brief uart1 rx buffer (end with '\0')
 */
#define uart1_rxcap 255
extern uint8_t uart1_rxbuf[uart1_rxcap];

#endif

#if USE_IRQ_U2RX
extern uint8_t uart2_rxflg;
#define uart2_rxcap 255
extern uint8_t uart2_rxbuf[uart2_rxcap];
#endif

// func

void board_init(void);

#endif
