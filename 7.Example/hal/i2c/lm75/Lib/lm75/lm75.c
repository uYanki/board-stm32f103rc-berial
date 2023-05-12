
#include "lm75.h"

float lm75_get_limit(uint8_t reg)
{
    uint16_t raw = lm75_read_word(reg) >> 7;
    if (raw & 0x0100) {
        return -0.5f * ((~raw & 0xFF) + 1);
    } else {
        return raw * 0.5f;
    }
}

void lm75_set_limit(uint8_t reg, float value)
{
    uint16_t raw;
    if (value > 0) {
        raw = value * 2;
        raw &= 0xFF;
    } else {
        raw = -value * 2;
        raw &= 0xFF;
        raw = ~(raw - 1);
        raw |= 1 << 15;
    }
    raw <<= 7;
    lm75_write_word(reg, raw);
}

float lm75_read_temp(void)
{
    uint16_t raw = lm75_read_word(LM75_REG_TEMP) >> 5;
    if (raw & 0x0400) {
        raw |= 0xF800U;
        raw = -(~raw + 1);
    }
    return raw * 0.125f;
}