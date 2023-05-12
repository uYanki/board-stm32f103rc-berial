#ifndef __DHT11_H
#define __DHT11_H

#include "board.h"

#include "stm32f10x.h"

typedef struct {
    uint8_t humi_int;   // 湿度的整数部分
    uint8_t humi_deci;  // 湿度的小数部分
    uint8_t temp_int;   // 温度的整数部分
    uint8_t temp_deci;  // 温度的小数部分
    uint8_t check_sum;  // 校验和
} dht11_data_t;

#define DHT11_GPIO_CLK_EN() RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)

#define DHT11_GPIO_PORT     GPIOB
#define DHT11_GPIO_PIN      GPIO_Pin_10

#define DHT11_DATA_0()      GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN)
#define DHT11_DATA_1()      GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN)
#define DHT11_DATA_N()      GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

#define DHT11_DELAY_US(us)  delay_us(us)
#define DHT11_DELAY_MS(ms)  delay_ms(ms)

void dht11_init(void);
bool dht11_read_temp_and_humidity(dht11_data_t* data);

#endif /* __DHT11_H */
