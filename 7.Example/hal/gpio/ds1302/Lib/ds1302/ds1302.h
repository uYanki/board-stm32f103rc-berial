#ifndef __DS1302_H__
#define __DS1302_H__

#include "gpio.h"

#define RST_W(state) HAL_GPIO_WritePin(DS1302_RST_GPIO_Port, DS1302_RST_Pin, state)
#define CLK_W(state) HAL_GPIO_WritePin(DS1302_CLK_GPIO_Port, DS1302_CLK_Pin, state)
#define DAT_W(state) HAL_GPIO_WritePin(DS1302_DAT_GPIO_Port, DS1302_DAT_Pin, state)

#define DAT_R() HAL_GPIO_ReadPin(DS1302_DAT_GPIO_Port, DS1302_DAT_Pin)

void ds1302_init(void);

void ds1302_start_time(void);
void ds1302_stop_time(void);

uint8_t ds1302_get_year(void);
uint8_t ds1302_get_month(void);
uint8_t ds1302_get_date(void);
uint8_t ds1302_get_hour(void);
uint8_t ds1302_get_minite(void);
uint8_t ds1302_get_second(void);

void ds1302_set_time(uint8_t time[]);
void ds1302_get_time(uint8_t time[]);

// ram addresses range from 0 to 30
void    ds1302_write_ram(uint8_t addr, uint8_t val);
uint8_t ds1302_read_ram(uint8_t addr);
// fill ram with 0
void ds1302_clear_ram(void);

void ds1302_get_time_brust(uint8_t time[]);
void ds1302_set_time_brust(uint8_t time[]);

void ds1302_read_ram_brust(uint8_t len, uint8_t* buf);
void ds1302_write_ram_brust(uint8_t len, uint8_t* buf);

#endif
