#ifndef __LED_KEY_H__
#define __LED_KEY_H__

#include <stm32f10x.h>

#define KEY_ON 	0
#define KEY_OFF	1

#define LED_TOGGLE  GPIOA->ODR ^= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4

void Led_Configuration(void);
void Key_Configuration(void);
int Key_Scan(GPIO_TypeDef* GPIOx, unsigned int Pin);


#endif
