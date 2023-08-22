#ifndef __BOARD_CONF_H__
#define __BOARD_CONF_H__

#define USE_KEY       0
#define USE_IRQ_KEY   0
#define USE_LED       0

#define USE_PINCTRL   1

/* UART */

#define USE_UART1     0
#define USE_UART2     1

#define USE_IRQ_U1RX  0
#define USE_IRQ_U2RX  1

// baudrate
#define CONFIG_U1BAUD 115200
#define CONFIG_U2BAUD 115200

#if USE_UART2
#undef USE_IRQ_U1RX
#endif

// 1: Poll 2:IRQ_U1RX 4:IRQ_U2RX
#define USE_CMD_PARSER 4

#endif
