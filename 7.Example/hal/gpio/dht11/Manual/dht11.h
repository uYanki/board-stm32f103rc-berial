/*
 * dht11.h
 *
 *  Created on: Jul 4, 2022
 *      Author: hejunlin
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f1xx_hal.h"
#include "main.h"


#define OUTPUT 1
#define INPUT 0


/* Exported constants --------------------------------------------------------*/

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_5

#define DHT_GPIO_LOW() HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET) // GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
#define DHT_GPIO_HIGH() HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET) // GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define DHT_DAT           HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
// 设置IO口方向
void DHT11_SetGpioMode(u8 pMode);
u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_ReadData(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_ReadByte(void);//读出一个字节
u8 DHT11_ReadBit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11


#endif /* INC_DHT11_H_ */
