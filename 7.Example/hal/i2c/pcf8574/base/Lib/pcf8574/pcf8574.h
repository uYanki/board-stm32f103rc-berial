
#ifndef __PCF8574_H__
#define __PCF8574_H__

#include "i2c.h"
#include "utils.h"

// PCF8574 的中断引脚: 在输入模式的时候，如果有引脚的高低电平跳变，则会产生中断

typedef enum {
    PCF8574_ADDRESS_A000 = 0x20,
    PCF8574_ADDRESS_A001,
    PCF8574_ADDRESS_A010,
    PCF8574_ADDRESS_A011,
    PCF8574_ADDRESS_A100,
    PCF8574_ADDRESS_A101,
    PCF8574_ADDRESS_A110,
    PCF8574_ADDRESS_A111,
} pcf8574_address_t;

#define PCF8574_DEV (PCF8574_ADDRESS_A000 << 1)  // <----- dev
#define PCF8574_I2C hi2c2                        // <----- hi2c

typedef enum {
    PCF8574_PIN_0 = 1 << 0,
    PCF8574_PIN_1 = 1 << 1,
    PCF8574_PIN_2 = 1 << 2,
    PCF8574_PIN_3 = 1 << 3,
    PCF8574_PIN_4 = 1 << 4,
    PCF8574_PIN_5 = 1 << 5,
    PCF8574_PIN_6 = 1 << 6,
    PCF8574_PIN_7 = 1 << 7,
} pcf8574_pin_t;

typedef enum {
    PCF8574_PIN_LEVEL_LOW  = 0,
    PCF8574_PIN_LEVEL_HIGH = 1,
} pcf8574_pin_level_t;

uint8_t pcf8574_read_port(void);
void    pcf8574_write_port(uint8_t level);
void    pcf8574_toggle_port(void);

pcf8574_pin_level_t pcf8574_read_pin(pcf8574_pin_t pin);
void                pcf8574_write_pin(pcf8574_pin_t pin, pcf8574_pin_level_t level);
void                pcf8574_toggle_pin(pcf8574_pin_t pin);

void pcf8574_toggle_pins(uint8_t pins);

void pcf8574_shift_left(uint8_t n);    // 算术左移
void pcf8574_shift_right(uint8_t n);   // 算术右移
void pcf8574_rotate_left(uint8_t n);   // 循环左移
void pcf8574_rotate_right(uint8_t n);  // 循环右移

void pcf8574_reverse(void);  // 倒序

uint8_t pcf8574_read_key(pcf8574_pin_t pin);
uint8_t pcf8574_matrixkey4x4_scan(uint8_t* row, uint8_t* col);

#endif
