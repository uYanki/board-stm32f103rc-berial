/*
  LIS3DH_core_spi.h - Library for interacting with the STMicro LIS3DH
                      MEMS digital output motion sensor, ultra low-power
                      high performance 3-axes "nano" accelerometer.
  Created by Craig Wm. Versek, 2014-12-04
 */

#include <SPI.h>
#include "LIS3DH.h"

//configure the accelerometer chip

LIS3DH_CoreSPIClass acc(6,   //slaveSelectLowPin
                        15    //dataReadyLowPin
                       );

void setup() {
  Serial.begin(9600);
  //start up the SPI bus
  SPI.begin();
  SPI.setClockDivider(24); //96MHz clock /24 = 4 MHz
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  
  //SPI.setClockDivider(SPI_CLOCK_DIV21);  //FIXME SPI_CLOCK_DIV these names are not defined anymore in Arduino 1.5.4
  //start controlling the voltage supply
  acc.begin();
  delay(1000);
  

}

void loop() {
  delay(1000);
  byte data;
  for (int i = 0; i < LIS3DH::ADDR_MAX; i++){
    data = acc._readRegister(i);
    Serial.print(i);
    Serial.print(": ");
    print_binary(data, 8);
    Serial.print("\n");
  }
}

void print_binary(int v, int num_places)
{
    int mask=0, n;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places

    while(num_places)
    {

        if (v & (0x0001 << num_places-1))
        {
             Serial.print("1");
        }
        else
        {
             Serial.print("0");
        }

        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            Serial.print("_");
        }
    }
}
