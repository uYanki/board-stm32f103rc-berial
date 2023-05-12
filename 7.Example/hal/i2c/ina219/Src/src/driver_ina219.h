
#ifndef DRIVER_INA219_H
#define DRIVER_INA219_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    INA219_ADDRESS_0 = (0x40 << 1),  // A0 = GND, A1 = GND
    INA219_ADDRESS_1 = (0x41 << 1),  // A0 = VS+, A1 = GND
    INA219_ADDRESS_2 = (0x42 << 1),  // A0 = SDA, A1 = GND
    INA219_ADDRESS_3 = (0x43 << 1),  // A0 = SCL, A1 = GND
    INA219_ADDRESS_4 = (0x44 << 1),  // A0 = GND, A1 = VS+
    INA219_ADDRESS_5 = (0x45 << 1),  // A0 = VS+, A1 = VS+
    INA219_ADDRESS_6 = (0x46 << 1),  // A0 = SDA, A1 = VS+
    INA219_ADDRESS_7 = (0x47 << 1),  // A0 = SCL, A1 = VS+
    INA219_ADDRESS_8 = (0x48 << 1),  // A0 = GND, A1 = SDA
    INA219_ADDRESS_9 = (0x49 << 1),  // A0 = VS+, A1 = SDA
    INA219_ADDRESS_A = (0x4A << 1),  // A0 = SDA, A1 = SDA
    INA219_ADDRESS_B = (0x4B << 1),  // A0 = SCL, A1 = SDA
    INA219_ADDRESS_C = (0x4C << 1),  // A0 = GND, A1 = SCL
    INA219_ADDRESS_D = (0x4D << 1),  // A0 = VS+, A1 = SCL
    INA219_ADDRESS_E = (0x4E << 1),  // A0 = SDA, A1 = SCL
    INA219_ADDRESS_F = (0x4F << 1)   // A0 = SCL, A1 = SCL
} ina219_address_t;
typedef enum {
    INA219_BUS_VOLTAGE_RANGE_16V = 0,  // ±16V
    INA219_BUS_VOLTAGE_RANGE_32V = 1,  // ±32V
} ina219_bus_voltage_range_t;
typedef enum {
    INA219_PGA_40_MV  = 0,  // ±40 mV
    INA219_PGA_80_MV  = 1,  // ±80 mV
    INA219_PGA_160_MV = 2,  // ±160 mV
    INA219_PGA_320_MV = 3,  // ±320 mV
} ina219_pga_t;
typedef enum {
    INA219_ADC_MODE_9_BIT_1_SAMPLES    = 0x0,  // 9 bit / 1 samples
    INA219_ADC_MODE_10_BIT_1_SAMPLES   = 0x1,  // 10 bit / 1 samples
    INA219_ADC_MODE_11_BIT_1_SAMPLES   = 0x2,  // 11 bit / 1 samples
    INA219_ADC_MODE_12_BIT_1_SAMPLES   = 0x3,  // 12 bit / 1 samples
    INA219_ADC_MODE_12_BIT_2_SAMPLES   = 0x9,  // 12 bit / 2 samples
    INA219_ADC_MODE_12_BIT_4_SAMPLES   = 0xA,  // 12 bit / 4 samples
    INA219_ADC_MODE_12_BIT_8_SAMPLES   = 0xB,  // 12 bit / 8 samples
    INA219_ADC_MODE_12_BIT_16_SAMPLES  = 0xC,  // 12 bit / 16 samples
    INA219_ADC_MODE_12_BIT_32_SAMPLES  = 0xD,  // 12 bit / 32 samples
    INA219_ADC_MODE_12_BIT_64_SAMPLES  = 0xE,  // 12 bit / 64 samples
    INA219_ADC_MODE_12_BIT_128_SAMPLES = 0xF,  // 12 bit / 128 samples
} ina219_adc_mode_t;
typedef enum {
    INA219_MODE_POWER_DOWN                   = 0x0,  // power down
    INA219_MODE_SHUNT_VOLTAGE_TRIGGERED      = 0x1,  // shunt voltage triggered
    INA219_MODE_BUS_VOLTAGE_TRIGGERED        = 0x2,  // bus voltage triggered
    INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED  = 0x3,  // shunt and bus triggered
    INA219_MODE_ADC_OFF                      = 0x4,  // adc off
    INA219_MODE_SHUNT_VOLTAGE_CONTINUOUS     = 0x5,  // shunt voltage continuous
    INA219_MODE_BUS_VOLTAGE_CONTINUOUS       = 0x6,  // bus voltage continuous
    INA219_MODE_SHUNT_BUS_VOLTAGE_CONTINUOUS = 0x7,  // shunt and bus voltage continuous
} ina219_mode_t;
typedef struct ina219_handle_s {
    uint8_t iic_addr;  // iic device address

    double  r;            // resistance
    double  current_lsb;  // current lsb
    uint8_t inited;       // inited flag
} ina219_handle_t;

uint8_t ina219_info(ina219_info_t* info);
uint8_t ina219_set_addr_pin(ina219_handle_t* handle, ina219_address_t addr_pin);
uint8_t ina219_get_addr_pin(ina219_handle_t* handle, ina219_address_t* addr_pin);
uint8_t ina219_set_resistance(ina219_handle_t* handle, double resistance);
uint8_t ina219_get_resistance(ina219_handle_t* handle, double* resistance);
uint8_t ina219_init(ina219_handle_t* handle);
uint8_t ina219_deinit(ina219_handle_t* handle);
uint8_t ina219_read_shunt_voltage(ina219_handle_t* handle, int16_t* raw, float* mV);
uint8_t ina219_read_bus_voltage(ina219_handle_t* handle, uint16_t* raw, float* mV);
uint8_t ina219_read_current(ina219_handle_t* handle, int16_t* raw, float* mA);
uint8_t ina219_read_power(ina219_handle_t* handle, uint16_t* raw, float* mW);
uint8_t ina219_soft_reset(ina219_handle_t* handle);
uint8_t ina219_set_bus_voltage_range(ina219_handle_t* handle, ina219_bus_voltage_range_t range);
uint8_t ina219_get_bus_voltage_range(ina219_handle_t* handle, ina219_bus_voltage_range_t* range);
uint8_t ina219_set_pga(ina219_handle_t* handle, ina219_pga_t pga);
uint8_t ina219_get_pga(ina219_handle_t* handle, ina219_pga_t* pga);
uint8_t ina219_set_bus_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t mode);
uint8_t ina219_get_bus_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t* mode);
uint8_t ina219_set_shunt_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t mode);
uint8_t ina219_get_shunt_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t* mode);
uint8_t ina219_set_mode(ina219_handle_t* handle, ina219_mode_t mode);
uint8_t ina219_get_mode(ina219_handle_t* handle, ina219_mode_t* mode);
uint8_t ina219_set_calibration(ina219_handle_t* handle, uint16_t data);
uint8_t ina219_get_calibration(ina219_handle_t* handle, uint16_t* data);
uint8_t ina219_calculate_calibration(ina219_handle_t* handle, uint16_t* calibration);
uint8_t ina219_set_reg(ina219_handle_t* handle, uint8_t reg, uint16_t data);
uint8_t ina219_get_reg(ina219_handle_t* handle, uint8_t reg, uint16_t* data);
#ifdef __cplusplus
}
#endif
#endif
