
#include "gy614.h"

gy614_baud_rate_t gy614_get_baud_rate(void) { return (gy614_baud_rate_t)gy614_read_byte(GY614_REG_BAUD_RATE); }

void gy614_set_baud_rate(gy614_baud_rate_t rate) { gy614_write_byte(GY614_REG_BAUD_RATE, (uint8_t)rate); }

gy614_sample_rate_t gy614_get_sample_rate(void) { return (gy614_sample_rate_t)gy614_read_byte(GY614_REG_SAMPLE_RATE); }

void gy614_set_sample_rate(gy614_sample_rate_t rate) { gy614_write_byte(GY614_REG_SAMPLE_RATE, (uint8_t)rate); }

gy614_output_mode_t gy614_get_output_mode(void) { return (gy614_output_mode_t)gy614_read_byte(GY614_REG_OUTPUT_MODE); }

void gy614_set_output_mode(gy614_output_mode_t mode) { gy614_write_byte(GY614_REG_OUTPUT_MODE, (uint8_t)mode); }

gy614_output_format_t gy614_get_output_format(void) { return (gy614_output_format_t)gy614_read_byte(GY614_REG_OUTPUT_FORMAT); }

void gy614_set_output_format(gy614_output_format_t format) { gy614_write_byte(GY614_REG_OUTPUT_FORMAT, (uint8_t)format); }

void gy614_save_config(void) { gy614_write_byte(GY614_REG_CONFIG_STATE, (uint8_t)GY614_CONFIG_SET); }

void gy614_reset_config(void) { gy614_write_byte(GY614_REG_CONFIG_STATE, (uint8_t)GY614_CONFIG_RESET); }

// 8bit i2c address ()
void gy614_set_address(uint8_t address) { gy614_write_byte(GY614_REG_I2C_ADDRESS, address); }

uint8_t gy614_get_address(void) { return gy614_read_byte(GY614_REG_I2C_ADDRESS); }

// emissivity: 1~100
void gy614_set_emissivity(uint8_t emissivity) { gy614_write_byte(GY614_REG_EMISSIVITY, emissivity); }

uint8_t gy614_get_emissivity(void) { return gy614_read_byte(GY614_REG_EMISSIVITY); }

// 结果除100就是实际温度
uint16_t gy614_get_object_temp(void) { return gy614_read_word(GY614_REG_TEMP_OBJECT_H); }

uint16_t gy614_get_ambient_temp(void) { return gy614_read_word(GY614_REG_TEMP_AMBIENT_H); }

uint16_t gy614_get_brow_temp(void) { return gy614_read_word(GY614_REG_TEMP_BROW_H); }

uint8_t gy614_get_chip_info(void) { return gy614_read_byte(GY614_REG_CHIP_INFO); }

uint8_t gy614_get_fireware_info(void) { return gy614_read_byte(GY614_REG_FIREWARE_INFO); }

// offset: 0~255,default=100
uint8_t gy614_get_temp_offset(void) { return gy614_read_byte(GY614_REG_TEMP_OFFSET); }

void gy614_set_temp_offset(uint8_t offset) { gy614_write_byte(GY614_REG_TEMP_OFFSET, offset); }