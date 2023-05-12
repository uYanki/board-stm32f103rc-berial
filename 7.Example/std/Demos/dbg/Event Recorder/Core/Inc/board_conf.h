#ifndef __BOARD_CONF_H__
#define __BOARD_CONF_H__

#define USE_KEY     1
#define USE_LED     0

#define USE_PINCTRL 0

#define USE_UART1   0
#define USE_UART2   1

#if !USE_UART2
#define USE_IRQ_U1RX 1
#endif

#define USE_IRQ_U2RX 0

#endif

