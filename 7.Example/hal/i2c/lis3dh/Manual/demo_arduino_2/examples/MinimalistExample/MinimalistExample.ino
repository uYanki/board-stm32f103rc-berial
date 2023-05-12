/******************************************************************************
MinimalistExample.ino

Marshall Taylor @ MHEtLive Electronics
Nov 16, 2016
https://github.com/MHEtLive/LIS3DH_Breakout
https://github.com/MHEtLive/MHEtLive_LIS3DH_Arduino_Library

Description:
Most basic example of use.

Example using the LIS3DH with basic settings.  This sketch collects
Accelerometer data every second, then presents it on the serial monitor.

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation
Either can be omitted if not used

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

LIS3DH myIMU; //Default constructor is I2C, addr 0x19.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  //Call .begin() to configure the IMU
  myIMU.begin();

}


void loop()
{
  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 4);

  delay(1000);
}
