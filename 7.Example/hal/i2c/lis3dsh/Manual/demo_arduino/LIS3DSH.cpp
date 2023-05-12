#include <stdio.h>

#include <LIS3DSH.h>
#include <Wire.h>

// Defines ////////////////////////////////////////////////////////////////
#define _MULTI_REGISTER_ACCEL_READ

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define LIS3DSH_ADDR_SEL_HIGH	0x1d	// with SEL/SDO pulled up to VDD
#define LIS3DSH_ADDR_SEL_LOW	0x1e	// with SEL/SDO pulled down to VSS


// Private/Protected Methods //////////////////////////////////////////////////////////////

// Writes an accelerometer register
void LIS3DSH::writeReg(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(I2CAddr);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}


// Reads an accelerometer register
uint8_t LIS3DSH::readReg(uint8_t reg)
{
	uint8_t value;

	Wire.beginTransmission(I2CAddr);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(I2CAddr, 1);

	while(!Wire.available());

	value = Wire.read();
	Wire.endTransmission();

	return value;
}

// Public Methods //////////////////////////////////////////////////////////////

LIS3DSH::LIS3DSH()
{
	I2CAddr = LIS3DSH_ADDR_SEL_HIGH;
}


uint8_t LIS3DSH::setSELState(uint8_t SELState)
{
	uint8_t ReturnValue = 0;

	if(0 == SELState)
	{
		I2CAddr = LIS3DSH_ADDR_SEL_LOW;
	}
	else if(1 == SELState)
	{
		I2CAddr = LIS3DSH_ADDR_SEL_HIGH;
	}

	return ReturnValue;
}


// Turns on the LIS3DSH and places it in normal mode.
void LIS3DSH::enableDefault(void)
{
	// 0x0F = 0b00001111
	// Normal power mode, all axes enabled, 50 Hz ODR
	writeReg(LIS3DSH_CTRL_REG4, 0x5F);
	// 200 Hz antialias filter, +/- 2g FS range
	writeReg(LIS3DSH_CTRL_REG5, 0x80);
	// configure FIFO for bypass mode
	writeReg(LIS3DSH_FIFO_CTRL_REG, 0);
	// disable FIFO, enable register address auto-increment
	writeReg(LIS3DSH_CTRL_REG6, 0x10);
}


// Reads the 3 accelerometer channels
void LIS3DSH::readAccel(int16_t *pX, int16_t *pY, int16_t *pZ)
{
	uint8_t xla;
	uint8_t xha;
	uint8_t yla;
	uint8_t yha;
	uint8_t zla;
	uint8_t zha;

	if((NULL != pX) && (NULL != pY) && (NULL != pZ))
	{
		Wire.beginTransmission(I2CAddr);

#ifdef _MULTI_REGISTER_ACCEL_READ
		// assert the MSB of the address to get the accelerometer
		// to do slave-transmit subaddress updating.
		Wire.write(LIS3DSH_OUT_X_L | (1 << 7));
		Wire.endTransmission();
		Wire.requestFrom(I2CAddr, 6);

		while (Wire.available() < 6);

		xla = Wire.read();
		xha = Wire.read();
		yla = Wire.read();
		yha = Wire.read();
		zla = Wire.read();
		zha = Wire.read();

#else

		xla = readReg(LIS3DSH_OUT_X_L);
		xha = readReg(LIS3DSH_OUT_X_H);
		yla = readReg(LIS3DSH_OUT_Y_L);
		yha = readReg(LIS3DSH_OUT_Y_H);
		zla = readReg(LIS3DSH_OUT_Z_L);
		zha = readReg(LIS3DSH_OUT_Z_H);

#endif	//MULTI_REGISTER_ACCEL_READ

		*pX = (int16_t)(xha << 8 | xla);
		*pY = (int16_t)(yha << 8 | yla);
		*pZ = (int16_t)(zha << 8 | zla);
	}
}


void LIS3DSH::readTemperature(int8_t *pTemperature)
{
	uint8_t RegisterValue;

	if(NULL != pTemperature)
	{
		RegisterValue = readReg(LIS3DSH_OUT_TEMP);
		*pTemperature = (int8_t)RegisterValue;
	}
}


void LIS3DSH::setFullScaleRange(uint8_t FullScaleRange)
{
	uint8_t RegisterValue;

	RegisterValue = readReg(LIS3DSH_CTRL_REG5);

	RegisterValue &= ~(0x38);
	RegisterValue |= ((FullScaleRange & 0x07) << 3);

	writeReg(LIS3DSH_CTRL_REG5, RegisterValue);
}


void LIS3DSH::setBandwidth(uint8_t Bandwidth)
{
	uint8_t RegisterValue;

	RegisterValue = readReg(LIS3DSH_CTRL_REG5);

	RegisterValue &= ~(0xC0);
	RegisterValue |= ((Bandwidth & 0x03) << 6);

	writeReg(LIS3DSH_CTRL_REG5, RegisterValue);
}


void LIS3DSH::setOutputDataRate(uint8_t DataRate)
{
	uint8_t RegisterValue;

	RegisterValue = readReg(LIS3DSH_CTRL_REG4);

	RegisterValue &= ~(0xF0);
	RegisterValue |= ((DataRate & 0x0f) << 4);

	writeReg(LIS3DSH_CTRL_REG4, RegisterValue);
}


