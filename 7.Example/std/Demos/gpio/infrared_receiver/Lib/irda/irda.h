#ifndef __IRDA_H__
#define __IRDA_H__

#include "board.h"

#define IRDA_ID               0

// RCC、GPIO、EXTI
#define IRDA_GPIO_PORT        GPIOA
#define IRDA_GPIO_CLK         RCC_APB2Periph_GPIOA
#define IRDA_GPIO_PIN         GPIO_Pin_5
#define IRDA_GPIO_PORT_SOURCE GPIO_PortSourceGPIOA
#define IRDA_GPIO_PIN_SOURCE  GPIO_PinSource5
#define IRDA_EXTI_LINE        EXTI_Line5
#define IRDA_EXTI_IRQN        EXTI9_5_IRQn
#define IRDA_EXTI_IRQHANDLER  EXTI9_5_IRQHandler

// Pin Level
#define IrDa_DIN()            GPIO_ReadInputDataBit(IRDA_GPIO_PORT, IRDA_GPIO_PIN)

extern uint8_t irda_flag;
extern uint8_t irda_cnt;

void    irda_init(void);
uint8_t irda_handler(void);

#endif
