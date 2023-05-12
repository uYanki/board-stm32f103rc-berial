#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "lcd1602_i2c.h"
#include "stdio.h"

/*
 * I2C1_SCL -> PB6
 * I2C1_SDA -> PB7
 */

// 0x7E = (0x3F << 1)
#define WRITE_LCD_ADDR 0x7E

static uint8_t lcd1602_initialized = 0;
static uint8_t lcd1602_send_failed = 0;

static void lcd1602_send(uint8_t data, uint8_t mode)
{
    char    data_u, data_l;
    uint8_t data_t[4];
    data_u    = (data & 0xF0) | mode;
    data_l    = ((data << 4) & 0xF0) | mode;
    data_t[0] = data_u | 0x0C;  // en=1, rs=0
    data_t[1] = data_u | 0x08;  // en=0, rs=0
    data_t[2] = data_l | 0x0C;  // en=1, rs=0
    data_t[3] = data_l | 0x08;  // en=0, rs=0
    while (HAL_I2C_Master_Transmit(&hi2c1, WRITE_LCD_ADDR, data_t, 4, 1000) != HAL_OK) {
        if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF) {
            HAL_I2C_DeInit(&hi2c1);
            HAL_I2C_Init(&hi2c1);
            lcd1602_send_failed = 1;
            lcd1602_initialized = 0;
            break;
        }
    }
}

void lcd1602_command(uint8_t data)
{
    lcd1602_send(data, 0);
}

void lcd1602_write(uint8_t data)
{
    lcd1602_send(data, 1);
}

void lcd1602_i2c_init(void)
{
    if (lcd1602_initialized == 1) {
        return;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    HAL_Delay(50);

    lcd1602_command(LCD_RETURNHOME);
    HAL_Delay(5);
    lcd1602_command(LCD_FUNCTIONSET | LCD_DISPLAYCONTROL);
    HAL_Delay(5);
    lcd1602_command(LCD_DISPLAYCONTROL | LCD_ENTRYMODESET);
    HAL_Delay(5);
    lcd1602_command(LCD_SETDDRAMADDR);
    HAL_Delay(5);
    HAL_Delay(15);

    if (lcd1602_send_failed == 0) {
        lcd1602_initialized = 1;
    }
}

void lcd1602_i2c_show_str(char* str0, char* str1)
{
    uint8_t i;

    lcd1602_i2c_init();

    if (str0) {
        lcd1602_command(LCD_SETDDRAMADDR);
        for (i = 0; *str0 != '\0' && i < 16; i++, str0++) {
            lcd1602_write((uint8_t)(*str0));
        }
        for (; i < 16; i++) {
            lcd1602_write(' ');
        }
    }
    if (str1) {
        lcd1602_command(LCD_SETDDRAMADDR | 0x40);
        for (i = 0; *str1 != '\0' && i < 16; i++, str1++) {
            lcd1602_write((uint8_t)(*str1));
        }
        for (; i < 16; i++) {
            lcd1602_write(' ');
        }
    }
}
