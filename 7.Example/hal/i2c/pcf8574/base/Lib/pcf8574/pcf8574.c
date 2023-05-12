

#include "pcf8574.h"

static uint8_t pcf8574_output_level = 0;

uint8_t pcf8574_read_port(void)
{
    uint8_t level;
    HAL_I2C_Master_Receive(&PCF8574_I2C, PCF8574_DEV, &level, 1, 0xFF);
    return level;
}

void pcf8574_write_port(uint8_t level)
{
    pcf8574_output_level = level;
    HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_DEV, &level, 1, 0xFF);
}

void pcf8574_toggle_port(void)
{
    pcf8574_write_port(~pcf8574_output_level);
}

pcf8574_pin_level_t pcf8574_read_pin(pcf8574_pin_t pin)
{
    return (pcf8574_read_port() & pin) ? PCF8574_PIN_LEVEL_HIGH : PCF8574_PIN_LEVEL_LOW;
}

void pcf8574_write_pin(pcf8574_pin_t pin, pcf8574_pin_level_t level)
{
    pcf8574_output_level &= ~pin;
    if (level == PCF8574_PIN_LEVEL_HIGH)
        pcf8574_output_level |= pin;
    pcf8574_write_port(pcf8574_output_level);
}

void pcf8574_toggle_pin(pcf8574_pin_t pin)
{
    pcf8574_toggle_pins(pin);
}

void pcf8574_toggle_pins(uint8_t pins)
{
    pcf8574_write_port(pcf8574_output_level ^ pins);
}

void pcf8574_shift_right(uint8_t n)
{
    pcf8574_write_port(pcf8574_output_level >> n);
}

void pcf8574_shift_left(uint8_t n)
{
    pcf8574_write_port(pcf8574_output_level << n);
}

void pcf8574_rotate_right(uint8_t n)
{
    n &= 7;
    pcf8574_write_port((pcf8574_output_level >> n) | (pcf8574_output_level << (8 - n)));
}

void pcf8574_rotate_left(uint8_t n)
{
    n &= 7;
    pcf8574_write_port((pcf8574_output_level << n) | (pcf8574_output_level >> (8 - n)));
}

void pcf8574_reverse(void)
{
    uint8_t x = pcf8574_output_level;

    x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);
    x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
    x = (x >> 4) | (x << 4);

    pcf8574_write_port(x);
}

/**
 * @return 0: no key presses 1: key presses
 * @note   按键一端接地, 另一端接PCF8574
 */
uint8_t pcf8574_read_key(pcf8574_pin_t pin)
{
    pcf8574_pin_level_t level;
    pcf8574_write_pin(pin, PCF8574_PIN_LEVEL_HIGH);
    level = pcf8574_read_pin(pin);
    pcf8574_write_port(pcf8574_output_level);
    return level == PCF8574_PIN_LEVEL_HIGH;
}

/**
 * @return 0: no key presses 1: key presses
 */
uint8_t pcf8574_matrixkey4x4_scan(uint8_t* row, uint8_t* col)
{
    uint8_t levels;
    for (size_t i = 0; i < 4; ++i) {
        pcf8574_write_port(0xF0 | ~(1 << i));
        levels = ~pcf8574_read_port() & 0xF0;
        if (levels) {
#if 1
            // waiting for key to be release
            delay_ms(10);
            levels = ~pcf8574_read_port() & 0xF0;
            if (!levels) return 0;
            while (~pcf8574_read_port() & 0xF0) {}
#endif
            *row = i;
            switch (levels >> 4) {
                case PCF8574_PIN_0: *col = 0; break;
                case PCF8574_PIN_1: *col = 1; break;
                case PCF8574_PIN_2: *col = 2; break;
                case PCF8574_PIN_3: *col = 3; break;
            }
#if 1
            println("%d,%d,%02d", *row, *col, *row * 4 + *col);
#endif
            return 1;
        }
    }
    return 0;
}
