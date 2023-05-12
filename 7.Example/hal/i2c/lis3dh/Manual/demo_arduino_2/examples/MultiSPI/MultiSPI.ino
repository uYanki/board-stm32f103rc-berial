/******************************************************************************
MultiSPI.ino

Marshall Taylor @ MHEtLive Electronics
Nov 16, 2016
https://github.com/MHEtLive/LIS3DH_Breakout
https://github.com/MHEtLive/MHEtLive_LIS3DH_Arduino_Library

Description:
Example using up to two LIS3DHs on the same SPI channel, with different CS pins.
If only one sensor is attached, this sketch reports failure on that channel and
runs with the single sensor instead.

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation
Either can be omitted if not used

Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23

Hardware connections
***CAUTION -- SPI pins can not be connected directly to 5v IO***

Connect SDA/SDI lines to pin 11 through level shifters (MOSI)
Connect SCL pin lines to pin 13 through level shifters (SCLK)
Connect SDO/SA0 lines to pin 12 through level shifters (MISO)
Connect CS to free pins through level shifters.  This example uses 9 and 10.
Connect GND and ***3.3v*** power to the IMU.  The sensors are not 5v tolerant.

(Multiple SPI devices share pins except for the Chip Select lines which
are unique for each device on the bus.)

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@MHEtLive.com.

Distributed as-is; no warranty is given.
******************************************************************************/

#include "MHEtLiveLIS3DH.h"
#include "Wire.h"
#include "SPI.h"

//Create two instances of the driver class
LIS3DH SensorOne( SPI_MODE, 10 );
LIS3DH SensorTwo( SPI_MODE, 9 );


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  //Call .begin() to configure the IMUs
  uint8_t returnData = 0;
  returnData = SensorOne.begin();
  if(( returnData != 0x00 )&&( returnData != 0xFF ))
  {
	  Serial.println("Problem starting the sensor with CS @ Pin 10.");
  }
  else
  {
	  Serial.println("Sensor with CS @ Pin 10 started.");
  }
  returnData = SensorTwo.begin();
  if(( returnData != 0x00 )&&( returnData != 0xFF ))
  {
	  Serial.println("Problem starting the sensor with CS @ Pin 9.");
  }
  else
  {
	  Serial.println("Sensor with CS @ Pin 9 started.");
  }
  
}


void loop()
{
  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X1 = ");
  Serial.println(SensorOne.readFloatAccelX(), 4);
  Serial.print(" Y1 = ");
  Serial.println(SensorOne.readFloatAccelY(), 4);
  Serial.print(" Z1 = ");
  Serial.println(SensorOne.readFloatAccelZ(), 4);
  Serial.print(" X2 = ");
  Serial.println(SensorTwo.readFloatAccelX(), 4);
  Serial.print(" Y2 = ");
  Serial.println(SensorTwo.readFloatAccelY(), 4);
  Serial.print(" Z2 = ");
  Serial.println(SensorTwo.readFloatAccelZ(), 4);
  
  Serial.print("\nSensorOne Bus Errors Reported:\n");
  Serial.print(" All '1's = ");
  Serial.println(SensorOne.allOnesCounter);
  Serial.print(" Non-success = ");
  Serial.println(SensorOne.nonSuccessCounter);
  Serial.print("SensorTwo Bus Errors Reported:\n");
  Serial.print(" All '1's = ");
  Serial.println(SensorTwo.allOnesCounter);
  Serial.print(" Non-success = ");
  Serial.println(SensorTwo.nonSuccessCounter);
  delay(1000);
}
