/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAX30100_H
#define MAX30100_H

#include <stdint.h>
#include "sys.h"
#include "MAX30100_Registers.h"

#define DEFAULT_MODE                MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30100_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH         MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT     MAX30100_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT      MAX30100_LED_CURR_50MA

//#define I2C_BUS_SPEED               400000UL


void max_begin(void);
void setMode(Mode mode);
void setLedsPulseWidth(LEDPulseWidth ledPulseWidth);
void setSamplingRate(SamplingRate samplingRate);
void setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
void setHighresModeEnabled(u8 enabled);
void update(void);

extern uint16_t rawIRValue;
extern uint16_t rawRedValue;


uint8_t readRegister(uint8_t address);
void writeRegister(uint8_t address, uint8_t data);
void burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length);
void readFifoData(void);

u8 Write_One_Byte(u8 addr,u8 data);
u8 Read_One_Byte(u8 addr);
u8 Buff_Read(u8 address,u8 *buf, u8 len);
//void I2C_GPIO_Config(void);
//void I2C_Mode_Config(void);
//void I2C_ByteWrite(u8* pBuffer, u8 WriteAddr);
//void I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);

#endif
