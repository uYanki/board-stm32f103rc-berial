/*
  LIS3DH_core_spi.h - Library for interacting with the STMicro LIS3DH
                      MEMS digital output motion sensor, ultra low-power
                      high performance 3-axes "nano" accelerometer.
  Created by Craig Wm. Versek, 2014-12-04
 */
#ifndef _LISH3D_H_INCLUDED
#define _LISH3D_H_INCLUDED

#include <Arduino.h>

#define DEFAULT_STARTUP_DELAY_MS 500
#define VREF_MAX 5.46
///*Vdd_MAX - 0.04 V */
#define RES24BIT 16777216
//#define ADDR_MAX 32
///* 001r rrrr 2**5 possible*/
#define RDATA_NUM_BYTES 27

/*******************************************************************************
  LIS3DH_CoreSPIClass
  
*******************************************************************************/
class LIS3DH_CoreSPIClass {
public:
  LIS3DH_CoreSPIClass(const int slaveSelectLowPin,
                      const int dataReadyLowPin
                     );
  //Configuration methods
  void begin(int startup_delay_ms = DEFAULT_STARTUP_DELAY_MS);
  //Functionality method
//  int startConversions();
//  int stopConversions();
//  int readDataRaw(byte *data_array);
//  void attach_dataReadyLow_interrupt(void (*function)(void));
//  void detach_dataReadyLow_interrupt();
  byte _readRegister (int addr);
  int  _writeRegister(int addr, byte value);
private:
  //Attributes
  int _slaveSelectLowPin;
  int _dataReadyLowPin;
};

/*******************************************************************************
  LIS3DH NAMESPACE DEFINITIONS
*******************************************************************************/
#define LIS3DH_RDATA_NUM_BYTES 6
#define LIS3DH_SPI_CLOCK_SPEED 8000000
#ifdef __cplusplus
namespace LIS3DH {
#endif
const int ADDR_MAX = 0x3D;
const byte ADDR_MASK = 0b00011111;
//const int RDATA_NUM_BYTES = 6;
enum reg {
    STATUS_REG_AUX = 0x07,
    OUT_ADC1_L     = 0x08,
    OUT_ADC1_H     = 0x09,
    OUT_ADC2_L     = 0x0A,
    OUT_ADC2_H     = 0x0B,
    OUT_ADC3_L     = 0x0C,
    OUT_ADC3_H     = 0x0D,
    INT_COUNTER_REG = 0x0E,
    WHO_AM_I       = 0x0F,
    TEMP_CFG_REG = 0x1F,
    CTRL_REG1 = 0x20,
    CTRL_REG2 = 0x21,
    CTRL_REG3 = 0x22,
    CTRL_REG4 = 0x23,
    CTRL_REG5 = 0x24,
    CTRL_REG6 = 0x25,
    REFERENCE = 0x26,
    STATUS_REG2 = 0x27,
    OUT_X_L = 0x28,  
};

//    enum ID_bits {
//        DEV_ID7 = 0x80,
//        DEV_ID6 = 0x40,
//        DEV_ID5 = 0x20,
//        DEV_ID2 = 0x04,
//        DEV_ID1 = 0x02,
//        DEV_ID0 = 0x01,

//        ID_const = 0x10,
//        ID_ADS129x = DEV_ID7,
//        ID_ADS129xR = (DEV_ID7 | DEV_ID6),

//        ID_4CHAN = 0,
//        ID_6CHAN = DEV_ID0,
//        ID_8CHAN = DEV_ID1,

//        ID_ADS1294 = (ID_ADS129x | ID_4CHAN),
//        ID_ADS1296 = (ID_ADS129x | ID_6CHAN),
//        ID_ADS1298 = (ID_ADS129x | ID_8CHAN),
//        ID_ADS1294R = (ID_ADS129xR | ID_4CHAN),
//        ID_ADS1296R = (ID_ADS129xR | ID_6CHAN),
//        ID_ADS1298R = (ID_ADS129xR | ID_8CHAN)
//    };


#ifdef __cplusplus
}
#endif /* namespace ADS129x */


#endif /* _LISH3D_H_INCLUDED */
