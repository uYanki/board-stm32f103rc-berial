#ifndef __DHT11_H__
#define __DHT11_H__

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "gpio.h"

uint8_t dht11_read(float* temp, float* humi);

float convert_c_to_f(float);
float convert_f_to_c(float);
float compute_heat_index(float temperature, float percentHumidity, bool isFahrenheit);

#endif