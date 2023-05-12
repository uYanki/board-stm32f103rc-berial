/*!
 *  \file    i2c_SHT2x.c
 *  \author  Patrick Taling (not the original author) 
 *  \date    20/04/2017
 *  \version 1.0
 *
 *  \brief  Simple function to read two Sensirion Humidity and Temperature sensors (SHT21 address 0x40) with the same I2C ADRESS through an I2C Switch (TCA9548A address 0x70)
 *			and write text and measure to an I2C LCD (PCF8574T address 0x27) with the ATxmega256a3u.
 *
 *  \details The file i2c_SHT2x.c is the library for two or more Sensirion Humidity and Temperature sensors (SHT21 address 0x40)
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
 *		#include "I2C_SHT2x.h"
 *		#include "Typedefs.h"
 *		#include "i2c.h"
 *		#include "i2c_lcd.h"
 *	\endverbatim
 *           \par
 *
 *           \note An AVR-project can use multiple I2C's. One shoud take care that
 *           in different source files there are no multiple I2C
 *           definitions for the same I2C.
 */

#define F_CPU 2000000UL
#include "I2C_SHT2x.h"
#include "Typedefs.h"
#include "i2c.h"
#include "i2c_lcd.h"
#include <util/delay.h>
#include <assert.h>
#include <math.h>

extern volatile float Temperature;
extern volatile float Humidity;

//==============================================================================
uint8_t i2c_SHT2x_CheckCrc(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
	//calculates 8-Bit checksum with given polynomial
//==============================================================================
{
	uint8_t crc = 0;
	uint8_t byteCtr;
	
	for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
	{ 	crc ^= (data[byteCtr]);
		for (uint8_t bit = 8; bit > 0; --bit)
		{ 	
			if (crc & 0x80){
				crc = (crc << 1) ^ POLYNOMIAL;
			}
			else crc = (crc << 1);
		}
	}
	if (crc != checksum) return CHECKSUM_ERROR;
	else return 0;
}
//===========================================================================
uint8_t i2c_SHT2x_ReadUserRegister(uint8_t *pRegisterValue, int channel)
//===========================================================================
{
	uint8_t error=0;
	
	i2c_TCA9548A_CMD(channel);
		
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 0);
	error |= i2c_write(&TWIE, USER_REG_R);
	i2c_stop(&TWIE);
	
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 1);
	*pRegisterValue = i2c_read(&TWIE, I2C_NACK);
	i2c_stop(&TWIE);
		
	return error;
}

//===========================================================================
uint8_t i2c_SHT2x_WriteUserRegister(uint8_t *pRegisterValue, int channel)
//===========================================================================
{
	uint8_t error=0;
	
	i2c_TCA9548A_CMD(channel);
	
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 0);
	error |= i2c_write(&TWIE, USER_REG_W);
	error |= i2c_write(&TWIE, *pRegisterValue);
	i2c_stop(&TWIE);
	return error;
}
//===========================================================================
uint8_t i2c_SHT2x_MeasureHM(etSHT2xMeasureType eSHT2xMeasureType, nt16 *pMeasurand, int channel)
//===========================================================================
{
	uint8_t checksum;
	uint8_t data[2];
	uint8_t error=0;
	
	i2c_TCA9548A_CMD(channel);

	//-- write I2C sensor address and command --
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 0);
	
	switch(eSHT2xMeasureType)
	{ 	case HUMIDITY:	error |= i2c_write(&TWIE, TRIG_RH_MEASUREMENT_HM); break;
		case TEMP:		error |= i2c_write(&TWIE, TRIG_T_MEASUREMENT_HM); break;
		default:		assert(0);
	}
	i2c_stop(&TWIE);
		
	//-- wait until hold master is released --
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 1);
	
	//-- read two data bytes and one checksum byte --
	pMeasurand->s16.u8H = data[0] = i2c_read(&TWIE, I2C_ACK);
	pMeasurand->s16.u8L = data[1] = i2c_read(&TWIE, I2C_ACK);
	checksum = i2c_read(&TWIE, I2C_NACK);
	i2c_stop(&TWIE);
	
	//-- verify checksum --
	error |= i2c_SHT2x_CheckCrc (data,2,checksum);
	
	return error;
}
//===========================================================================
uint8_t i2c_SHT2x_SoftReset(int channel)
//===========================================================================
{
	uint8_t error=0;
	i2c_TCA9548A_CMD(channel);
	
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 0);
	error |= i2c_write(&TWIE, SOFT_RESET);
	i2c_stop(&TWIE);

	_delay_ms(15); // wait till sensor has restarted
	return error;
}
//==============================================================================
float i2c_SHT2x_CalcRH(uint16_t sRH)
	//-- calculate relative humidity [%RH] --
	//humidityRH = -6 + (125 *((float)sRH/65536));	//RESOLUTION_16
//==============================================================================
{
	float humidityRH;
	sRH &= ~0x0003; // clear bits [1..0] (status bits)

	humidityRH = (float)sRH / 65536;	//RH = -6 + 125 * sRH/2^16;
	humidityRH = humidityRH * 125;
	humidityRH = humidityRH - 6;
	
	return humidityRH;
}
//==============================================================================
float i2c_SHT2x_CalcTemperatureC(uint16_t sT)
	//-- calculate temperature [°C] --
	//temperatureC = - 46.85 + (175.72 * ((float)sT /RESOLUTION_16));
//==============================================================================
{
	float temperatureC;
	sT &= ~0x0003; // clear bits [1..0] (status bits)
	
	temperatureC = (float)sT / RESOLUTION_16; //T= -46.85 + 175.72 * ST/2^16
	temperatureC = temperatureC * CONSTANT_175_72;
	temperatureC = temperatureC - CONSTANT_46_85;
	
	return temperatureC;
}

//==============================================================================
uint8_t i2c_SHT2x_GetSerialNumber(uint8_t SerialNumber[], int channel)
//==============================================================================
{
	uint8_t error=0; //error variable
		
	i2c_TCA9548A_CMD(channel);
	
	//Read from memory location 1
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 0);
	error |= i2c_write(&TWIE, CMD_CHIP_MEMORY_LOC_1); //Command for readout on-chip memory
	error |= i2c_write(&TWIE, ADR_CHIP_MEMORY_LOC_1); //on-chip memory address
	i2c_stop(&TWIE);
	
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 1);
	SerialNumber[5] = i2c_read(&TWIE, I2C_ACK); //Read SNB_3
	i2c_read(&TWIE, I2C_ACK); //Read CRC SNB_3 (CRC is not analyzed)
	SerialNumber[4] = i2c_read(&TWIE, I2C_ACK); //Read SNB_2
	i2c_read(&TWIE, I2C_ACK); //Read CRC SNB_2 (CRC is not analyzed)
	SerialNumber[3] = i2c_read(&TWIE, I2C_ACK); //Read SNB_1
	i2c_read(&TWIE, I2C_ACK);; //Read CRC SNB_1 (CRC is not analyzed)
	SerialNumber[2] = i2c_read(&TWIE, I2C_ACK); //Read SNB_0
	i2c_read(&TWIE, I2C_NACK); //Read CRC SNB_0 (CRC is not analyzed)
	i2c_stop(&TWIE);
	
	//Read from memory location 2
	i2c_TCA9548A_CMD(channel);							//kijken of dit wel echt noodzakelijk is
	
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 0);
	error |= i2c_write(&TWIE, CMD_CHIP_MEMORY_LOC_2); //Command for readout on-chip memory
	error |= i2c_write(&TWIE, ADR_CHIP_MEMORY_LOC_2); //on-chip memory address
	i2c_stop(&TWIE);
	
	error |= i2c_start(&TWIE, I2C_SHT2x_ADR, 1);
	SerialNumber[1] = i2c_read(&TWIE, I2C_ACK); //Read SNC_1
	SerialNumber[0] = i2c_read(&TWIE, I2C_ACK); //Read SNC_0
	i2c_read(&TWIE, I2C_ACK); //Read CRC SNC0/1 (CRC is not analyzed)
	SerialNumber[7] = i2c_read(&TWIE, I2C_ACK); //Read SNA_1
	SerialNumber[6] = i2c_read(&TWIE, I2C_ACK); //Read SNA_0
	i2c_read(&TWIE, I2C_NACK); //Read CRC SNA0/1 (CRC is not analyzed)
	i2c_stop(&TWIE);
	
	return error;
}


//===========================================================================
void i2c_TCA9548A_CMD(int channel)
//===========================================================================
{
	char cmd_byte = 0X00;
	
	switch(channel){
		case 0:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH0); break;
		case 1:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH1); break;
		case 2:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH2); break;
		case 3:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH3); break;
		case 4:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH4); break;
		case 5:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH5); break;
		case 6:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH6); break;
		case 7:		cmd_byte = (CMD_TCA9548A_OFF| CMD_TCA9548A_CH7); break;
		default:	cmd_byte = CMD_TCA9548A_OFF;
	}
	i2c_start(&TWIE, I2C_TCA9548A_ADDR, 0);
	i2c_write(&TWIE, cmd_byte);
	i2c_stop(&TWIE);
}

//===========================================================================
void Sht2x_Temperature_Humidity(int channel)
//===========================================================================
{
	uint8_t error = 0;
	uint8_t userRegister;
	int endOfBattery;
	nt16 sRH;
	nt16 sT;
	uint8_t SerialNumber_SHT2x[8];
	uint16_t h;
	char buffer[20];
	
	i2c_lcd_clear();
	_delay_us(1500);

	error = 0;
	error |= i2c_SHT2x_SoftReset(channel);	
	error |= i2c_SHT2x_GetSerialNumber(SerialNumber_SHT2x, channel);
	error |= i2c_SHT2x_ReadUserRegister(&userRegister, channel);			//get actual user reg
	userRegister = (userRegister & ~SHT2x_RES_MASK) | SHT2x_RES_11_11BIT;
	error |= i2c_SHT2x_WriteUserRegister(&userRegister, channel);		//write changed user reg
	error |= i2c_SHT2x_MeasureHM(TEMP, &sT, channel);
	error |= i2c_SHT2x_MeasureHM(HUMIDITY, &sRH, channel);
	Temperature = i2c_SHT2x_CalcTemperatureC(sT.u16);
	Humidity = i2c_SHT2x_CalcRH(sRH.u16);
	error |= i2c_SHT2x_ReadUserRegister(&userRegister, channel); //get actual user reg

	if( (userRegister & SHT2x_EOB_MASK) == SHT2x_EOB_ON ) endOfBattery = TRUE;
	else endOfBattery = FALSE;

	if(error != 0){
		i2c_lcd_clear();
		_delay_us(1500);
		
		i2c_lcd_write_text("ERROR ");
		
		utoa(error, buffer, 10);
		i2c_lcd_write_text(buffer);
	}
	else if (endOfBattery){
		i2c_lcd_clear();
		_delay_us(1500);
		
		i2c_lcd_write_text("LOW BATTERY ");
	}
	else{
		i2c_lcd_clear();
		_delay_us(1500);
		
		i2c_lcd_write_text("Channel ");
		utoa(channel, buffer, 10);
		i2c_lcd_write_text(buffer);
		
		i2c_lcd_set_cursor(0,1);
		h= Temperature;
		utoa(Temperature, buffer, 10);
		i2c_lcd_write_text(buffer);
		i2c_lcd_write_text(".");
		utoa((Temperature-h)*100, buffer, 10);
		i2c_lcd_write_text(buffer);
		
		i2c_lcd_set_cursor(8,1);
		h= Humidity;
		utoa(Humidity, buffer, 10);
		i2c_lcd_write_text(buffer);
		i2c_lcd_write_text(".");
		utoa((Humidity-h)*100, buffer, 10);
		i2c_lcd_write_text(buffer);
		i2c_lcd_write_text("%");
	}
}