
#ifndef DRIVER_INA219_SHOT_H
#define DRIVER_INA219_SHOT_H
#include "driver_ina219_interface.h"
#ifdef __cplusplus
extern "C"{
#endif
#define INA219_SHOT_DEFAULT_BUS_VOLTAGE_RANGE            INA219_BUS_VOLTAGE_RANGE_32V             // set bus voltage range 32V 
#define INA219_SHOT_DEFAULT_BUS_VOLTAGE_ADC_MODE         INA219_ADC_MODE_12_BIT_1_SAMPLES         // set bus voltage adc mode 12 bit 1 sample 
#define INA219_SHOT_DEFAULT_SHUNT_VOLTAGE_ADC_MODE       INA219_ADC_MODE_12_BIT_1_SAMPLES         // set shunt voltage adc mode 12 bit 1 sample 
#define INA219_SHOT_DEFAULT_PGA                          INA219_PGA_320_MV                        // set pga 320 mV 
uint8_t ina219_shot_init(ina219_address_t addr_pin, double r);
uint8_t ina219_shot_deinit(void);
uint8_t ina219_shot_read(float *mV, float *mA, float *mW);
#ifdef __cplusplus
}
#endif
#endif
