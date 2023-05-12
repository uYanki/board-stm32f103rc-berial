/*
  LIS3DH_core_spi.cpp - Library for interacting with the STMicro LIS3DH
                        MEMS digital output motion sensor, ultra low-power
                        high performance 3-axes "nano" accelerometer.
  Created by Craig Wm. Versek, 2014-12-04
 */
#include <Arduino.h>
#include <SPI.h>
#include "LIS3DH.h"
 
LIS3DH_CoreSPIClass::LIS3DH_CoreSPIClass(const int slaveSelectLowPin,
                                         const int dataReadyLowPin
                                        ){
  _slaveSelectLowPin = slaveSelectLowPin;
  _dataReadyLowPin   = dataReadyLowPin;
}

void LIS3DH_CoreSPIClass::begin(int startup_delay_ms) {
  //wait for the LIS3DH to be ready - it can take a while to charge caps
  delay(startup_delay_ms); 
  // Configure the Arduino pins
  pinMode(_slaveSelectLowPin, OUTPUT);
  pinMode(_dataReadyLowPin, INPUT);
  digitalWrite(_slaveSelectLowPin, HIGH);  //comm. off
  //wake up SPI interface, by sending null byte
  //digitalWrite(_slaveSelectLowPin, LOW);   //comm. off
  //SPI.transfer(0x00);
  //digitalWrite(_slaveSelectLowPin, HIGH);  //comm. off
}

//void LIS3DH_CoreSPIClass::attach_dataReadyLow_interrupt(void (*function)(void)){
//  attachInterrupt(_dataReadyLowPin, function, FALLING);
//}

//void LIS3DH_CoreSPIClass::detach_dataReadyLow_interrupt(){
//  detachInterrupt(_dataReadyLowPin);
//}

byte LIS3DH_CoreSPIClass::_readRegister(int addr) {
  byte opcode, data;
  opcode = 0b10000000 | (addr & LIS3DH::ADDR_MASK);  //bit0 = 1 -> READ, bit1 = 0 do not increment address
#ifdef SPI_HAS_TRANSACTION
  //gain control of SPI bus
  SPI.beginTransaction(SPISettings(LIS3DH_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE3));
  SPI.transfer(0x00);                         //FIXME issue transfer with no slave select to init clock polarity, is this needed for SPI_DATA_MODE 2 & 3
#endif
  digitalWrite(_slaveSelectLowPin, LOW);      //set chip as listener
  SPI.transfer(opcode);                       //send command
  data = SPI.transfer(0x00);                  //read back response (sending null byte)
  digitalWrite(_slaveSelectLowPin, HIGH);     //release chip select
#ifdef SPI_HAS_TRANSACTION
  SPI.endTransaction();                       //release the SPI bus
#endif
  return data;
}


int LIS3DH_CoreSPIClass::_writeRegister(int addr, byte value) {
  if(addr > 0 && addr <= LIS3DH::ADDR_MAX){
    byte opcode;
    opcode = addr &  LIS3DH::ADDR_MASK;         //bit0 = 0 -> WRITE, bit1 = 0 do not increment address
#ifdef SPI_HAS_TRANSACTION
    //gain control of SPI bus
    SPI.beginTransaction(SPISettings(LIS3DH_SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE3));
    SPI.transfer(0x00);                         //FIXME issue transfer with no slave select to init clock polarity, is this needed for SPI_DATA_MODE 2 & 3
#endif
    digitalWrite(_slaveSelectLowPin, LOW);
    SPI.transfer(opcode);
    SPI.transfer(value);
    digitalWrite(_slaveSelectLowPin, HIGH);
#ifdef SPI_HAS_TRANSACTION
    SPI.endTransaction();                       //release the SPI bus
#endif
    return 0;
  }
  else{ return -1; }          //error, out of range
}
