
#ifndef DRIVER_INA219_READ_TEST_H
#define DRIVER_INA219_READ_TEST_H
#include "driver_ina219_interface.h"
#ifdef __cplusplus
extern "C"{
#endif
uint8_t ina219_read_test(ina219_address_t addr_pin, double r, uint32_t times);
#ifdef __cplusplus
}
#endif
#endif
