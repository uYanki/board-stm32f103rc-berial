/*!
 *  \file    i2c_SHT2x.h
 *  \author  Patrick Taling (not the original author) 
 *  \date    20/04/2017
 *  \version 1.0
 *
 *  \brief  Simple function to read two Sensirion Humidity and Temperature sensors (SHT21 address 0x40) with the same I2C ADRESS through an I2C Switch (TCA9548A address 0x70)
 *			and write text and measure to an I2C LCD (PCF8574T address 0x27) with the ATxmega256a3u.
 *
 *  \details The file i2c_SHT2x.h is the library for two or more Sensirion Humidity and Temperature sensors (SHT21 address 0x40)
 *
 *	The library needs the i2c library from w.e.dolman (<a href="mailto:w.e.dolman@hva.nl">w.e.dolman@hva.nl</a>)
 *	For i2c.c use code 21.8 from "de taal C en de Xmega tweede druk" http://dolman-wim.nl/xmega/book/index.php
 *	For i2c.h use code 21.9 from "de taal C en de Xmega tweede druk" http://dolman-wim.nl/xmega/book/index.php
 *
 *	The library needs some parts of the i2c_lcd library from Noel200 from http://www.elektroda.pl/rtvforum/topic2756081.html.
 *	The library can be downloaded from: http://www.elektroda.pl/rtvforum/login.php?redirect=download.php&id=670533.
 *	Go to LCD_PCF8574T/lcd_pcf/ and use i2c_lcd.c and i2c_lcd.h from the package
 *
 *	The libraby needs some parts of the the SHT2x library from SENIRION from https://www.sensirion.com/en/products/humidity-sensors/humidity-temperature-sensor-sht2x-digital-i2c-accurate/
 *	The libraby can be download from: https://www.sensirion.com/en/products/all-documents-of-sensirions-humidity-sensors-for-download/
 *	Go to SHT2x/ Sample Code SHT21 and download the zip file Sensirion_Humidity_Sensors_SHT21_Sample_Code_C-file
 *
 * ## Original author information ##

 * ==============================================================================
 * S E N S I R I O N AG, Laubisruetistr. 50, CH-8712 Staefa, Switzerland
 * ==============================================================================
 * Project : SHT2x Sample Code (V1.2)
 * File : SHT2x.c
 * Author : MST
 * Controller: NEC V850/SG3 (uPD70F3740)
 * Compiler : IAR compiler for V850 (3.50A)
 * Brief : Sensor layer. Functions for sensor access
 * ==============================================================================

 * ####
 *
 *       
 *	\verbatim
 *		#include "i2c.h"
 *		#include "Typedefs.h"
 *	\endverbatim
 *           \par
 *
 *           \note An AVR-project can use multiple I2C's. One shoud take care that
 *           in different source files there are no multiple I2C
 *           definitions for the same I2C.
 */

#ifndef SHT2x_H
#define SHT2x_H

#include "i2c.h"
#include "Typedefs.h"

// CRC
#define POLYNOMIAL 0x131; //P(x)=x^8+x^5+x^4+1 = 0b100110001 = 305

#define	TRIG_T_MEASUREMENT_HM  		0xE3 // command trig. temp meas. hold master
#define	TRIG_RH_MEASUREMENT_HM  	0xE5 // command trig. humidity meas. hold master
#define	TRIG_T_MEASUREMENT_POLL  	0xF3 // command trig. temp meas. no hold master
#define	TRIG_RH_MEASUREMENT_POLL  	0xF5 // command trig. humidity meas. no hold master
#define	USER_REG_W  				0xE6 // command writing user register
#define	USER_REG_R 					0xE7 // command reading user register
#define	SOFT_RESET 					0xFE // command soft reset

#define	SHT2x_RES_12_14BIT 			0x00 // RH=12bit, T=14bit
#define	SHT2x_RES_8_12BIT 			0x01 // RH= 8bit, T=12bit
#define	SHT2x_RES_10_13BIT 			0x80 // RH=10bit, T=13bit
#define	SHT2x_RES_11_11BIT			0x81 // RH=11bit, T=11bit
#define	SHT2x_RES_MASK 				0x81 // Mask for res. bits (7,0) in user reg.

#define	SHT2x_EOB_ON 				0x40 // end of battery
#define	SHT2x_EOB_MASK 				0x40 // Mask for EOB bit(6) in user reg.

#define	SHT2x_HEATER_ON 			0x04 // heater on
#define	SHT2x_HEATER_OFF 			0x00 // heater off
#define	SHT2x_HEATER_MASK 			0x04 // Mask for Heater bit(2) in user reg.

#define LOW 						0
#define	HIGH						1

#define TRUE						1
#define FALSE						0

#define CONSTANT_6					6
#define CONSTANT_125				125
#define CONSTANT_46_85				46.85
#define CONSTANT_175_72				175.72
#define RESOLUTION_16				65536 //2^16

#define ACK_ERROR 					0x01
#define	TIME_OUT_ERROR				0x02
#define	CHECKSUM_ERROR 				0x04
#define	UNIT_ERROR 					0x08

#define CMD_CHIP_MEMORY_LOC_1		0xFA
#define ADR_CHIP_MEMORY_LOC_1		0x0F
#define CMD_CHIP_MEMORY_LOC_2		0xFC
#define ADR_CHIP_MEMORY_LOC_2		0xC9

// in i2c.h #define ACK = 0,
// in i2c.h #define NO_ACK = 1,

typedef enum{
	HUMIDITY,
	TEMP
}etSHT2xMeasureType;

#define	I2C_SHT2x_ADR  				0x40	// write = 0x80, read = 0x81		
#define I2C_TCA9548A_ADDR			0x70	// write = 0xE0, read = 0xE1

#define CMD_TCA9548A_OFF			0x00
#define CMD_TCA9548A_CH0			0x01
#define CMD_TCA9548A_CH1			0x02
#define CMD_TCA9548A_CH2			0x04
#define CMD_TCA9548A_CH3			0x08
#define CMD_TCA9548A_CH4			0x10
#define CMD_TCA9548A_CH5			0x20
#define CMD_TCA9548A_CH6			0x40
#define CMD_TCA9548A_CH7			0x80

uint8_t i2c_SHT2x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum);
uint8_t i2c_SHT2x_ReadUserRegister(uint8_t *pRegisterValue, int channel);
uint8_t i2c_SHT2x_WriteUserRegister(uint8_t *pRegisterValue, int channel);
uint8_t i2c_SHT2x_MeasureHM(etSHT2xMeasureType eSHT2xMeasureType, nt16 *pMeasurand, int channel);
uint8_t i2c_SHT2x_SoftReset(int channel);
float i2c_SHT2x_CalcRH(uint16_t sRH);
float i2c_SHT2x_CalcTemperatureC(uint16_t sT);
uint8_t i2c_SHT2x_GetSerialNumber(uint8_t SerialNumber[], int channel);
void i2c_TCA9548A_CMD(int channel);
void Sht2x_Temperature_Humidity(int channel);
#endif