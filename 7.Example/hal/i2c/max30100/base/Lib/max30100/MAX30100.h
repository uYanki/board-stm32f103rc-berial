
#ifndef MAX30100_H
#define MAX30100_H

#include "i2c.h"

#define MAX30100_I2C hi2c2
#define MAX30100_DEV (0x57 << 1)

#include "Registers.h"

#define DEFAULT_MODE            MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE   MAX30100_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH     MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT MAX30100_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT  MAX30100_LED_CURR_50MA

extern uint16_t rawIRValue, rawRedValue;  // output

void    MAX30100_WriteByte(uint8_t reg, uint8_t data);
uint8_t MAX30100_ReadByte(uint8_t reg);
void    MAX30100_ReadBytes(uint8_t reg, uint8_t* buff, uint8_t len);

void MAX30100_Init(void);
void MAX30100_SetMode(Mode mode);
void MAX30100_SetLedsPulseWidth(LEDPulseWidth ledPulseWidth);
void MAX30100_SetSamplingRate(SamplingRate samplingRate);
void MAX30100_SetLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
void MAX30100_SetHighresModeEnabled(uint8_t enabled);

// void MAX30100_ReadFIFO(uint16_t* ir, uint16_t* red);
void    MAX30100_ReadFIFO(void);
void    MAX30100_ResetFIFO(void);
uint8_t MAX30100_GetPartId(void);  // return 0x11

void    MAX30100_StartTemperatureSampling();
uint8_t MAX30100_IsTemperatureReady();
float   MAX30100_RetrieveTemperature();
void    MAX30100_Shutdown();
void    MAX30100_Resume();

#endif
