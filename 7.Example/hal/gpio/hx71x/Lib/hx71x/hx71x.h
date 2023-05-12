
#ifndef __HX71X_H__
#define __HX71X_H__

#include "gpio.h"
#include "utils.h"

#define CONFIG_HX710 1
#define CONFIG_HX711 0

typedef enum {
    HX71X_MODE_CHANNEL_A_GAIN_128 = 1,  // channel A
    HX71X_MODE_CHANNEL_A_GAIN_64  = 3,  // channel A
#if CONFIG_HX711
    HX71X_MODE_CHANNEL_B_GAIN_32 = 2,  // channel B
#endif
} hx71x_mode_t;

static int32_t TareWeight = 0;  // 皮重(unused)

int32_t hx71x_read_raw(void);
int32_t hx71x_read_raw_average(uint8_t times);

float hx71x_convert_to_voltage(int32_t raw);
float hx71x_convert_to_pressure(int32_t raw);

void         hx71x_set_mode(hx71x_mode_t mode);
hx71x_mode_t hx71x_get_mode(void);

#endif
