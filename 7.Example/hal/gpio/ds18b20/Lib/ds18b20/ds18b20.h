#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <stdio.h>
#include "gpio.h"

uint8_t DS18B20_Reset(void);

float DS18B20_ConvertTemp(int16_t temp);

uint8_t DS18B20_ReadID(uint8_t* id);
int16_t DS18B20_ReadTempReg(void);
int16_t DS18B20_ReadTempRegByID(uint8_t* id);

uint8_t DS18B20_ReadTempRegCRC(int16_t* read_temp);

void DS18B20_SetResolution(uint8_t res);

#endif