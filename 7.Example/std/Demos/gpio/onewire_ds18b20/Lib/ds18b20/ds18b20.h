#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "board.h"

#define DS18B20_GPIO_CLK_EN() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

#define DS18b20_GPIO_PORT     GPIOB
#define DS18B20_GPIO_PIN      GPIO_Pin_10

#define DS18B20_DQ_0()        PBout(10) = 0
#define DS18B20_DQ_1()        PBout(10) = 1
#define DS18B20_DQ_N()        PBin(10)

void  ds18b20_init(void);
float ds18B20_read_temperature(int16_t* temp);

#endif
