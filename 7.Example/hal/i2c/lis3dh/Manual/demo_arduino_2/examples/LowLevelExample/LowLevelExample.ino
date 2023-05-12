/******************************************************************************
LowLevelExample.ino

Marshall Taylor @ MHEtLive Electronics
Nov 16, 2016
https://github.com/MHEtLive/LIS3DH_Breakout
https://github.com/MHEtLive/MHEtLive_LIS3DH_Arduino_Library

Description:
Example using the LIS3DH with ONLY read and write methods.  It's up to you to
read the datasheets and get the sensor to behave as you will.

This sketch saves a significant amount of memory because the settings and complex
math (such as floating point variables) doesn't exist.  The cost of saved memory is
that the values are in 'counts', or raw data from the register.  The user is
responsible for converting these raw values into something meaningful.

Use the register words from MHEtLiveLIS3DH.h to manually configure the IC.

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation

Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23

Hardware connections:
Connect I2C SDA line to A4
Connect I2C SCL line to A5
Connect GND and 3.3v power to the IMU

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@MHEtLive.com.

Distributed as-is; no warranty is given.
******************************************************************************/

#include "MHEtLiveLIS3DH.h"
#include "Wire.h"
#include "SPI.h"

uint16_t errorsAndWarnings = 0;

LIS3DHCore myIMU( I2C_MODE, 0x19 );
//LIS3DHCore myIMU( SPI_MODE, 10 );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  //Call .beginCore() to configure the IMU
  if( myIMU.beginCore() != 0 )
  {
    Serial.print("Error at beginCore().\n");
  }
  else
  {
    Serial.print("\nbeginCore() passed.\n");
  }
  
  uint8_t dataToWrite = 0;  //Temporary variable


  //Setup the accelerometer******************************
  dataToWrite = 0; //Start Fresh!
  dataToWrite |= 0x4 << 4; //ODR of 50Hz
  dataToWrite |= 0x7; //Enable all axes

  //Now, write the patched together data
  errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG1, dataToWrite);

  dataToWrite = 0x80;
  errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG4, dataToWrite);

  dataToWrite = 0x80; //ADC enable
  errorsAndWarnings += myIMU.writeRegister(LIS3DH_TEMP_CFG_REG, dataToWrite);

  

//  //Test interrupt configuration profile on int1
//  {
//	dataToWrite = 0x40; //INT1 src
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG3, dataToWrite);
//	dataToWrite = 0x08; //latch output int
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);
//	dataToWrite = 0x40; //
//	//errorsAndWarnings += myIMU.writeRegister(LIS3DH_REFERENCE, dataToWrite);
//	dataToWrite = 0x0A; //High X and high Y only
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_CFG, dataToWrite);
//	dataToWrite = 0x3F; // half amplitude?
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);
//	dataToWrite = 0x01; //duration?
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);
//  }

  
  //Test interrupt configuration profile on int2
  {
	dataToWrite = 0x40; //INT2 src
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG6, dataToWrite);
	dataToWrite = 0x08; //latch output int
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);
	dataToWrite = 0x40; //
	//errorsAndWarnings += myIMU.writeRegister(LIS3DH_REFERENCE, dataToWrite);
	dataToWrite = 0x0A; //High X and high Y only
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_CFG, dataToWrite);
	dataToWrite = 0x3F; // half amplitude?
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);
	dataToWrite = 0x01; //duration?
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);
  }

  //Get the ID:
  uint8_t readData = 0;
  Serial.print("\nReading LIS3DH_WHO_AM_I: 0x");
  myIMU.readRegister(&readData, LIS3DH_WHO_AM_I);
  Serial.println(readData, HEX);

}


void loop()
{
  int16_t temp;
  uint8_t temp2;
  
  uint8_t readData = 0;
  Serial.print("\nReading LIS3DH_WHO_AM_I: 0x");
  myIMU.readRegister(&readData, LIS3DH_WHO_AM_I);
  Serial.println(readData, HEX);
  
  Serial.print("\nReading LIS3DH_STATUS_REG_AUX: 0x");
  myIMU.readRegister(&readData, LIS3DH_STATUS_REG_AUX);
  Serial.println(readData, HEX);

  Serial.print("\nReading LIS3DH_INT1_SOURCE: 0x");
  myIMU.readRegister(&readData, LIS3DH_INT1_SOURCE);
  Serial.println(readData, HEX);

  //Clear interrupts
  //errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_SOURCE, 0x00);

  //Dump regs:
  for( int i = LIS3DH_STATUS_REG_AUX; i <= LIS3DH_INT1_DURATION; i++)
  {
	Serial.print("0x");
	Serial.print(i,HEX);
	Serial.print(": 0x");
	myIMU.readRegister(&readData, i);
	Serial.println(readData, HEX);
  }

  //Read a register into the temp variable.
//  if( myIMU.readRegister(&temp2, LIS3DH_OUT_Z_H) != 0 )
//  {
//    errorsAndWarnings++;
//  }
//  Serial.println(temp2);

  //Get all parameters
  Serial.print("\nAccelerometer Counts:\n");

  Serial.print(" X = ");
  //Read a register into the temp variable.
  if( myIMU.readRegisterInt16(&temp, LIS3DH_OUT_X_L) != 0 )
  {
    errorsAndWarnings++;
  }
  Serial.println(temp);
  
  Serial.print(" Y = ");  
  //Read a register into the temp variable.
  if( myIMU.readRegisterInt16(&temp, LIS3DH_OUT_Y_L) != 0 )
  {
    errorsAndWarnings++;
  }
  Serial.println(temp);

  Serial.print(" Z = ");
  //Read a register into the temp variable.
  if( myIMU.readRegisterInt16(&temp, LIS3DH_OUT_Z_L) != 0 )
  {
    errorsAndWarnings++;
  }
  Serial.println(temp);


  Serial.print("\nADC Counts:\n");

  Serial.print(" ADC1 = ");
  //Read a register into the temp variable.
  if( myIMU.readRegisterInt16(&temp, LIS3DH_OUT_ADC1_L) != 0 )
  {
    errorsAndWarnings++;
  }
  Serial.println(temp);
  
  Serial.print(" ADC2 = ");  
  //Read a register into the temp variable.
  if( myIMU.readRegisterInt16(&temp, LIS3DH_OUT_ADC2_L) != 0 )
  {
    errorsAndWarnings++;
  }
  Serial.println(temp);

  Serial.print(" ADC3 = ");
  //Read a register into the temp variable.
  if( myIMU.readRegisterInt16(&temp, LIS3DH_OUT_ADC3_L) != 0 )
  {
    errorsAndWarnings++;
  }
  Serial.println(temp);
  
  
  Serial.println();
  Serial.print("Total reported Errors and Warnings: ");
  Serial.println(errorsAndWarnings);
  
 
  delay(3000);
}