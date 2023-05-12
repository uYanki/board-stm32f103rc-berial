/******************************************************************************
FifoExample.ino

Marshall Taylor @ MHEtLive Electronics
Nov 16, 2016
https://github.com/MHEtLive/LIS3DH_Breakout
https://github.com/MHEtLive/MHEtLive_LIS3DH_Arduino_Library

Description:
This sketch configures the FIFO, then only collects data when the watermark
is passed.

The data output to the serial monitor is in CSV (comma separated variables) format, and
can be copy-pasted into a spreadsheet to make graphs.

When doing math on profiles of acceleration, this is a good way to make sure the data
samples occur regularly.

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation
Either can be omitted if not used

Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23

Hardware connections:
Connect to SPI through a logic level converter (or to a 3.3v processor).
Pin 10 is used as the chip select line.

This code is released under the [MIT License](http://opensource.org/licenses/MIT).

Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@MHEtLive.com.

Distributed as-is; no warranty is given.
******************************************************************************/

#include "MHEtLiveLIS3DH.h"
#include "Wire.h"
#include "SPI.h"

LIS3DH myIMU(SPI_MODE, 10); //Constructing with SPI interface information
//LIS3DH myIMU(I2C_MODE, 0x19); //Alternate constructor for I2C

uint32_t sampleNumber = 0; //Used to make CSV output row numbers

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  myIMU.settings.adcEnabled = 0;
  //Note:  By also setting tempEnabled = 1, temperature data is available
  //instead of ADC3 in.  Temperature *differences* can be read at a rate of
  //1 degree C per unit of ADC3 data.
  myIMU.settings.tempEnabled = 0;
  myIMU.settings.accelSampleRate = 10;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16
  myIMU.settings.xAccelEnabled = 1;
  myIMU.settings.yAccelEnabled = 1;
  myIMU.settings.zAccelEnabled = 1;

  //FIFO control settings
  myIMU.settings.fifoEnabled = 1;
  myIMU.settings.fifoThreshold = 20;  //Can be 0 to 32
  myIMU.settings.fifoMode = 1;  //FIFO mode.
  //fifoMode can be:
  //  0 (Bypass mode, FIFO off)
  //  1 (FIFO mode)
  //  3 (FIFO until full)
  //  4 (FIFO when trigger)
  
  //Call .begin() to configure the IMU (except for the fifo)
  myIMU.begin();

  Serial.print("Configuring FIFO with no error checking...");
  myIMU.fifoBegin(); //Configure fifo
  Serial.print("Done!\n");
  
  Serial.print("Clearing out the FIFO...");
  myIMU.fifoClear();
  Serial.print("Done!\n");
  myIMU.fifoStartRec(); //cause fifo to start taking data (re-applies mode bits)
  
}

void loop()
{
  //float temp;  //This is to hold read data
  //uint16_t tempUnsigned;
  //
  while(( myIMU.fifoGetStatus() & 0x80 ) == 0) {};  //Wait for watermark
  
  //Now loop until FIFO is empty.
  //If having problems with the fifo not restarting after reading data, use the watermark
  //bits (b5 to b0) instead.
  //while(( myIMU.fifoGetStatus() & 0x1F ) > 2) //This checks that there is only a couple entries left
  while(( myIMU.fifoGetStatus() & 0x20 ) == 0) //This checks for the 'empty' flag
  {
	  Serial.print(sampleNumber);
	  Serial.print(",");
	  Serial.print(myIMU.readFloatAccelX());
	  Serial.print(",");
	  Serial.print(myIMU.readFloatAccelY());
	  Serial.print(",");
	  Serial.print(myIMU.readFloatAccelZ());
	  Serial.println();
	  sampleNumber++;
  }

}