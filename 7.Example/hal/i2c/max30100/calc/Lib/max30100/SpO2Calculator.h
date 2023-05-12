
#ifndef MAX30100_SPO2CALCULATOR_H
#define MAX30100_SPO2CALCULATOR_H

#include <stdint.h>
#define CALCULATE_EVERY_N_BEATS 3

void    SpO2Calculator(void);
void    SPO2update(float irACValue, float redACValue, uint8_t beatDetected);
void    reset(void);
uint8_t getSpO2(void);

extern uint8_t spO2LUT[43];

extern float    irACValueSqSum;
extern float    redACValueSqSum;
extern uint8_t  beatsDetectedNum;
extern uint32_t samplesRecorded;
extern uint8_t  spO2;

#endif
