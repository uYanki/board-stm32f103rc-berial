#include "ina219.h"

#define ina219_read_word(reg)        i2cdev_get_word((i2c_cli_t)dev, reg, 0x00, ORDER_FIRST_HIGH)
#define ina219_write_word(reg, data) i2cdev_write_word((i2c_cli_t)dev, reg, data, ORDER_FIRST_HIGH)

#define ina219_delay_ms(n)           HAL_Delay(n)

uint8_t  ina219_read_bit(ina219_dev_t dev, uint8_t reg, uint8_t idx);
void     ina219_write_bit(ina219_dev_t dev, uint8_t reg, uint8_t idx, uint16_t data);
uint16_t ina219_read_bits(ina219_dev_t dev, uint8_t reg, uint8_t start, uint8_t len);
void     ina219_write_bits(ina219_dev_t dev, uint8_t reg, uint8_t start, uint8_t len, uint16_t data);

uint8_t ina219_read_bit(ina219_dev_t dev, uint8_t reg, uint8_t idx)
{
    return (ina219_read_word(reg) >> (idx & 0x0F)) & 0x01;
}

void ina219_write_bit(ina219_dev_t dev, uint8_t reg, uint8_t idx, uint16_t data)
{
    uint16_t buff;

    buff = ina219_read_word(reg);

    idx &= 0x0F;

    if (data) {
        buff |= (1 << idx);
    } else {
        buff &= ~(1 << idx);
    }

    ina219_write_word(reg, buff);
}

uint16_t ina219_read_bits(ina219_dev_t dev, uint8_t reg, uint8_t start, uint8_t len)
{
    uint16_t buff, data;

    if (len >= 16) return 0;

    buff = ina219_read_word(reg);

    data = buff & (0xFFFF >> (16 - len) << start);
    data >>= start;

    return data;
}

void ina219_write_bits(ina219_dev_t dev, uint8_t reg, uint8_t start, uint8_t len, uint16_t data)
{
    uint16_t buff, mask;

    if (len >= 16) return;

    buff = ina219_read_word(reg);
    mask = 0xFFFF >> (16 - len) << start;
    data <<= start;

    buff &= ~mask;
    buff |= mask & data;

    ina219_write_word(reg, buff);
}

//

INLINE void ina219_soft_reset(ina219_dev_t dev)
{
    ina219_write_bit(dev, INA219_REG_CONF, 15, 1);
}

INLINE void ina219_set_bus_voltage_range(ina219_dev_t dev, ina219_bus_voltage_range_t range)
{
    ina219_write_bit(dev, INA219_REG_CONF, 13, (uint8_t)range);
}

INLINE ina219_bus_voltage_range_t ina219_get_bus_voltage_range(ina219_dev_t dev)
{
    return (ina219_bus_voltage_range_t)ina219_read_bit(dev, INA219_REG_CONF, 13);
}

INLINE void ina219_set_pga(ina219_dev_t dev, ina219_pga_t pga)
{
    ina219_write_bits(dev, INA219_REG_CONF, 11, 2, (uint8_t)pga);
}

INLINE ina219_pga_t ina219_get_pga(ina219_dev_t dev)
{
    return (ina219_pga_t)ina219_read_bits(dev, INA219_REG_CONF, 11, 2);
}

INLINE void ina219_set_bus_voltage_adc_mode(ina219_dev_t dev, ina219_adc_mode_t mode)
{
    ina219_write_bits(dev, INA219_REG_CONF, 7, 4, (uint8_t)mode);
}

INLINE ina219_adc_mode_t ina219_get_bus_voltage_adc_mode(ina219_dev_t dev)
{
    return (ina219_adc_mode_t)ina219_read_bits(dev, INA219_REG_CONF, 7, 4);
}

INLINE void ina219_set_shunt_voltage_adc_mode(ina219_dev_t dev, ina219_adc_mode_t mode)
{
    ina219_write_bits(dev, INA219_REG_CONF, 3, 4, (uint8_t)mode);
}

INLINE ina219_adc_mode_t ina219_get_shunt_voltage_adc_mode(ina219_dev_t dev)
{
    return (ina219_adc_mode_t)ina219_read_bits(dev, INA219_REG_CONF, 3, 4);
}

void ina219_set_mode(ina219_dev_t dev, ina219_mode_t mode)
{
    uint8_t m, mode1, mode2, t;

    ina219_write_bits(dev, INA219_REG_CONF, 0, 3, (uint8_t)mode);

    if ((mode >= (uint8_t)INA219_MODE_SHUNT_VOLTAGE_TRIGGERED) &&
        (mode <= (uint8_t)INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED)) {
        mode1 = (uint8_t)ina219_get_shunt_voltage_adc_mode(dev);
        mode2 = (uint8_t)ina219_get_bus_voltage_adc_mode(dev);
        m     = (mode1 > mode2) ? mode1 : mode2;
        if (m <= 8) {
            delay_ms(1);
        } else {
            t = (uint8_t)(0.532 * (2 << (m - 8))) + 1;
            delay_ms(t);
        }
    }
}

INLINE ina219_mode_t ina219_get_mode(ina219_dev_t dev)
{
    return (ina219_mode_t)ina219_read_bits(dev, INA219_REG_CONF, 0, 3);
}

INLINE float ina219_read_shunt_voltage(ina219_dev_t dev, int16_t* raw)
{
    int16_t s = ina219_read_word(INA219_REG_SHUNT_VOLTAGE);

    if (*raw != NULL) *raw = s;

    return (float)(s) / 100.0f;  // mV
}

float ina219_read_bus_voltage(ina219_dev_t dev, uint16_t* raw)
{
    uint16_t u = ina219_read_word(INA219_REG_BUS_VOLTAGE);

    if ((u & 1) != 0) {
        if ((u & 2) == 0) {
            u >>= 3;
        }
    }

    if (*raw != NULL) *raw = u;

    return (float)(*raw) * 4.0f;  // mV
}

INLINE float ina219_read_current(ina219_dev_t dev, int16_t* raw)
{
    int16_t s;

    s = ina219_read_word(INA219_REG_CURRENT);

    if (*raw != NULL) *raw = s;

    return (float)(s * dev->current_lsb * 1000);  // mA
}

INLINE float ina219_read_power(ina219_dev_t dev, uint16_t* raw)
{
    uint16_t u = ina219_read_word(INA219_REG_POWER);

    if (*raw != NULL) *raw = u;

    return (float)(u * dev->current_lsb * 20.0 * 1000.0);  // mW
}

INLINE uint16_t ina219_get_calibration(ina219_dev_t dev)
{
    return ina219_read_word(INA219_REG_CALIBRATION) >> 1;
}

uint16_t ina219_calculate_calibration(ina219_dev_t dev)
{
    double  v;
    uint8_t pga = 0;

    if ((dev->resistance >= -0.000001f) && (dev->resistance <= 0.000001f))
        pga = (uint8_t)ina219_get_pga(dev);

    switch (pga) {
        case 0: v = 0.04; break;
        case 1: v = 0.08; break;
        case 2: v = 0.16; break;
        case 3: v = 0.32; break;
        default: return 0;
    }

    println("cal ok");

    dev->current_lsb = v / dev->resistance / 32768.0;
    return (uint16_t)(0.04096 * 32768.0 / v);  // calibration
}

INLINE void ina219_set_calibration(ina219_dev_t dev, uint16_t data)
{
    ina219_write_word(INA219_REG_CALIBRATION, data << 1);
}
