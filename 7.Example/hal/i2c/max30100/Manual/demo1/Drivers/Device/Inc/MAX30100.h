#ifndef __MAX30100_H
#define __MAX30100_H

#include "stm32f1xx_hal.h"

#define INTERRUPT_REG 0X00
#define INTERRUPT_REG_A_FULL (0X01 << 7)
#define INTERRUPT_REG_TEMP_RDY (0X01 << 6)
#define INTERRUPT_REG_HR_RDY (0X01 << 5)
#define INTERRUPT_REG_SPO2_RDY (0X01 << 4)
#define INTERRUPT_REG_PWR_RDY (0X01 << 0)

#define SAMPLES_PER_SECOND 100  // ¼ì²âÆµÂÊ

uint8_t max30100_Bus_Write(uint8_t Register_Address, uint8_t Word_Data);
uint8_t max30100_Bus_Read(uint8_t Register_Address);
void    max30100_FIFO_Read(uint8_t Register_Address, uint16_t Word_Data[][2], uint8_t count);
void    max30100_init(void);

#endif /*__MAX30100_h*/
