#ifndef __BOARD_H__
#define __BOARD_H__

#include "board_conf.h"

#include <stdbool.h>

#include "stm32f10x.h"

#include "delay.h"
#include "debug.h"

#if USE_CMD_PARSER
#include "cmd_parser/cmd.h"
#endif

#define UNUSED(v)                        (void)(v)
#define nullptr                          (void*)0

// inline function
#define INLINE                           __inline

// number of elements in an array
#define ARRAY_SIZE(arr)                  (sizeof(arr) / sizeof(arr[0]))
// byte offset of member in structure
#define MEMBER_OFFSET(structure, member) ((int)&(((structure*)0)->member))
// size of a member of a structure
#define MEMBER_SIZE(structure, member)   (sizeof(((structure*)0)->member))

// swap number
#define swap_int(a, b)                   (a ^= b, b ^= a, a ^= b)  // a ^= b ^= a ^= b

// gpio

#if USE_PINCTRL

#include "pinctrl.h"

#if USE_LED
#define LED PAout(8)
#endif

#if USE_KEY
#define KEY PAin(0)
#endif

#endif

#if USE_LED
#define LED_GPIO_Port GPIOA
#define LED_GPIO_Pin  GPIO_Pin_8
#endif

#if USE_KEY
#define KEY_GPIO_Port GPIOA
#define KEY_GPIO_Pin  GPIO_Pin_0
#endif

// uart

#define USE_UART (USE_UART1 || USE_UART2)

#ifndef CONFIG_U1BAUD
#define CONFIG_U1BAUD 115200
#endif

#ifndef CONFIG_U2BAUD
#define CONFIG_U2BAUD 115200
#endif

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
