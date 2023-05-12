/******************************************************************************
IntUsage.ino

Marshall Taylor @ MHEtLive Electronics
Nov 16, 2016
https://github.com/MHEtLive/LIS3DH_Breakout
https://github.com/MHEtLive/MHEtLive_LIS3DH_Arduino_Library

Description:
Interrupt support for the LIS3DH is extremely flexible, so configuration must be left
to the user.  This sketch demonstrates how to make a interrupt configuration function.

Use configIntterupts() as a template, then comment/uncomment desired options.
See ST docs for information
  Doc ID 18198 (AN3308): LIS3DHpplication information
  Doc ID 17530: LIS3DH datasheet
  
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

LIS3DH myIMU(I2C_MODE, 0x19); //Default constructor is I2C, addr 0x19.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  //Accel sample rate and range effect interrupt time and threshold values!!!
  myIMU.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  myIMU.settings.accelRange = 2;      //Max G force readable.  Can be: 2, 4, 8, 16

  myIMU.settings.adcEnabled = 0;
  myIMU.settings.tempEnabled = 0;
  myIMU.settings.xAccelEnabled = 1;
  myIMU.settings.yAccelEnabled = 1;
  myIMU.settings.zAccelEnabled = 1;
  
  //Call .begin() to configure the IMU
  myIMU.begin();
  
  configIntterupts();

}


void loop()
{
  //Get all parameters
  //Serial.print("\nAccelerometer:\n");
  //Serial.print(" X = ");
  //Serial.println(myIMU.readFloatAccelX(), 4);
  //Serial.print(" Y = ");
  //Serial.println(myIMU.readFloatAccelY(), 4);
  //Serial.print(" Z = ");
  //Serial.println(myIMU.readFloatAccelZ(), 4);

  uint8_t dataRead;

  Serial.print("LIS3DH_INT1_SRC: 0x");
  myIMU.readRegister(&dataRead, LIS3DH_INT1_SRC);//cleared by reading
  Serial.println(dataRead, HEX);
  Serial.println("Decoded events:");
  if(dataRead & 0x40) Serial.println("Interrupt Active");
  if(dataRead & 0x20) Serial.println("Z high");
  if(dataRead & 0x10) Serial.println("Z low");
  if(dataRead & 0x08) Serial.println("Y high");
  if(dataRead & 0x04) Serial.println("Y low");
  if(dataRead & 0x02) Serial.println("X high");
  if(dataRead & 0x01) Serial.println("X low");
  Serial.println();

  delay(1000);

  //  Use this section to read back blocks of data from the LIS3DH for troubleshooting.
//    //Dump regs:
//  for( int i = LIS3DH_INT_COUNTER_REG; i <= LIS3DH_INT1_DURATION; i++)
//  {
//	Serial.print("0x");
//	Serial.print(i,HEX);
//	Serial.print(": 0x");
//	myIMU.readRegister(&dataRead, i);
//	Serial.println(dataRead, HEX);
//  }
  
}

void configIntterupts()
{
  uint8_t dataToWrite = 0;

  //LIS3DH_INT1_CFG   
  //dataToWrite |= 0x80;//AOI, 0 = OR 1 = AND
  //dataToWrite |= 0x40;//6D, 0 = interrupt source, 1 = 6 direction source
  //Set these to enable individual axes of generation source (or direction)
  // -- high and low are used generically
  //dataToWrite |= 0x20;//Z high
  //dataToWrite |= 0x10;//Z low
  dataToWrite |= 0x08;//Y high
  //dataToWrite |= 0x04;//Y low
  //dataToWrite |= 0x02;//X high
  //dataToWrite |= 0x01;//X low
  myIMU.writeRegister(LIS3DH_INT1_CFG, dataToWrite);
  
  //LIS3DH_INT1_THS   
  dataToWrite = 0;
  //Provide 7 bit value, 0x7F always equals max range by accelRange setting
  dataToWrite |= 0x10; // 1/8 range
  myIMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);
  
  //LIS3DH_INT1_DURATION  
  dataToWrite = 0;
  //minimum duration of the interrupt
  //LSB equals 1/(sample rate)
  dataToWrite |= 0x01; // 1 * 1/50 s = 20ms
  myIMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);
  
  //LIS3DH_CLICK_CFG   
  dataToWrite = 0;
  //Set these to enable individual axes of generation source (or direction)
  // -- set = 1 to enable
  //dataToWrite |= 0x20;//Z double-click
  dataToWrite |= 0x10;//Z click
  //dataToWrite |= 0x08;//Y double-click 
  dataToWrite |= 0x04;//Y click
  //dataToWrite |= 0x02;//X double-click
  dataToWrite |= 0x01;//X click
  myIMU.writeRegister(LIS3DH_CLICK_CFG, dataToWrite);
  
  //LIS3DH_CLICK_SRC
  dataToWrite = 0;
  //Set these to enable click behaviors (also read to check status)
  // -- set = 1 to enable
  //dataToWrite |= 0x20;//Enable double clicks
  dataToWrite |= 0x04;//Enable single clicks
  //dataToWrite |= 0x08;//sine (0 is positive, 1 is negative)
  dataToWrite |= 0x04;//Z click detect enabled
  dataToWrite |= 0x02;//Y click detect enabled
  dataToWrite |= 0x01;//X click detect enabled
  myIMU.writeRegister(LIS3DH_CLICK_SRC, dataToWrite);
  
  //LIS3DH_CLICK_THS   
  dataToWrite = 0;
  //This sets the threshold where the click detection process is activated.
  //Provide 7 bit value, 0x7F always equals max range by accelRange setting
  dataToWrite |= 0x0A; // ~1/16 range
  myIMU.writeRegister(LIS3DH_CLICK_THS, dataToWrite);
  
  //LIS3DH_TIME_LIMIT  
  dataToWrite = 0;
  //Time acceleration has to fall below threshold for a valid click.
  //LSB equals 1/(sample rate)
  dataToWrite |= 0x08; // 8 * 1/50 s = 160ms
  myIMU.writeRegister(LIS3DH_TIME_LIMIT, dataToWrite);
  
  //LIS3DH_TIME_LATENCY
  dataToWrite = 0;
  //hold-off time before allowing detection after click event
  //LSB equals 1/(sample rate)
  dataToWrite |= 0x08; // 4 * 1/50 s = 160ms
  myIMU.writeRegister(LIS3DH_TIME_LATENCY, dataToWrite);
  
  //LIS3DH_TIME_WINDOW 
  dataToWrite = 0;
  //hold-off time before allowing detection after click event
  //LSB equals 1/(sample rate)
  dataToWrite |= 0x10; // 16 * 1/50 s = 320ms
  myIMU.writeRegister(LIS3DH_TIME_WINDOW, dataToWrite);

  //LIS3DH_CTRL_REG5
  //Int1 latch interrupt and 4D on  int1 (preserve fifo en)
  myIMU.readRegister(&dataToWrite, LIS3DH_CTRL_REG5);
  dataToWrite &= 0xF3; //Clear bits of interest
  dataToWrite |= 0x08; //Latch interrupt (Cleared by reading int1_src)
  //dataToWrite |= 0x04; //Pipe 4D detection from 6D recognition to int1?
  myIMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);

  //LIS3DH_CTRL_REG3
  //Choose source for pin 1
  dataToWrite = 0;
  //dataToWrite |= 0x80; //Click detect on pin 1
  dataToWrite |= 0x40; //AOI1 event (Generator 1 interrupt on pin 1)
  dataToWrite |= 0x20; //AOI2 event ()
  //dataToWrite |= 0x10; //Data ready
  //dataToWrite |= 0x04; //FIFO watermark
  //dataToWrite |= 0x02; //FIFO overrun
  myIMU.writeRegister(LIS3DH_CTRL_REG3, dataToWrite);
 
  //LIS3DH_CTRL_REG6
  //Choose source for pin 2 and both pin output inversion state
  dataToWrite = 0;
  dataToWrite |= 0x80; //Click int on pin 2
  //dataToWrite |= 0x40; //Generator 1 interrupt on pin 2
  //dataToWrite |= 0x10; //boot status on pin 2
  //dataToWrite |= 0x02; //invert both outputs
  myIMU.writeRegister(LIS3DH_CTRL_REG6, dataToWrite);
  
}