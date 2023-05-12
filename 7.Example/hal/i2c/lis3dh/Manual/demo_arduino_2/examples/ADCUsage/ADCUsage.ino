/******************************************************************************
ADCUsage.ino

Marshall Taylor @ MHEtLive Electronics
Nov 16, 2016
https://github.com/MHEtLive/LIS3DH_Breakout
https://github.com/MHEtLive/MHEtLive_LIS3DH_Arduino_Library

Description:
ADC pin reading.

This returns 10 bit values for the 3 analog in pins.  The ADC works a little different
than the arduino, in that the detectable range is centered around half-rail voltage and
is not rail to rail.  Connect a potentiometer between 3.3v and ground, and with the
wiper connected to a pin.

Also shown in this sketch is how the temperature is handled in the LIS3DH.
When temperature is enabled, ADC3 can be read to determine the temperature change
only, with a LSb worth 1 degree celsius.

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
  
  myIMU.settings.adcEnabled = 1;
  //Note:  By also setting tempEnabled = 1, temperature data is available
  //on ADC3.  Temperature *differences* can be read at a rate of
  //1 degree C per unit of ADC3
  myIMU.settings.tempEnabled = 0;
  myIMU.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.xAccelEnabled = 0;
  myIMU.settings.yAccelEnabled = 0;
  myIMU.settings.zAccelEnabled = 0;
  
  //Call .begin() to configure the IMU
  myIMU.begin();

}

void loop()
{
  //Get all parameters
  Serial.print("\nADC:\n");
  Serial.print(" 1 = ");
  Serial.println(myIMU.read10bitADC1());
  Serial.print(" 2 = ");
  Serial.println(myIMU.read10bitADC2());
  Serial.print(" 3 = ");
  Serial.println(myIMU.read10bitADC3());
  
  delay(300);
}
