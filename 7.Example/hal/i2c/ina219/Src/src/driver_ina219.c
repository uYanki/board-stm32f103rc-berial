
#include "driver_ina219.h"
#include <math.h>

#define MANUFACTURER_NAME  "Texas Instruments"  // manufacturer name
#define SUPPLY_VOLTAGE_MIN 3.0f                 // chip min supply voltage
#define SUPPLY_VOLTAGE_MAX 5.5f                 // chip max supply voltage

uint8_t ina219_set_resistance(ina219_handle_t* handle, double resistance)
{
    r = resistance; /* set resistance */
}
uint8_t ina219_get_resistance(ina219_handle_t* handle, double* resistance)
{
    *resistance = r; /* get resistance */
}

uint8_t ina219_soft_reset(ina219_handle_t* handle)
{
    read(INA219_REG_CONF, (uint16_t*)&prev);                    /* read config */
    prev &= ~(1 << 15);                                         /* clear soft reset */
    prev |= 1 << 15;                                            /* set soft reset */
    return a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev); /* write config */
}

uint8_t ina219_set_bus_voltage_range(ina219_handle_t* handle, ina219_bus_voltage_range_t range)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    prev &= ~(1 << 13);                                         /* clear range bit */
    prev |= range << 13;                                        /* set range bit */
    return a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev); /* write config */
}
uint8_t ina219_get_bus_voltage_range(ina219_handle_t* handle, ina219_bus_voltage_range_t* range)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    *range = (ina219_bus_voltage_range_t)((prev >> 13) & 0x01); /* get range */
    return 0;                                                   /* success return 0 */
}
uint8_t ina219_set_pga(ina219_handle_t* handle, ina219_pga_t pga)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    prev &= ~(3 << 11);                                         /* clear pga bit */
    prev |= pga << 11;                                          /* set pga bit */
    return a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev); /* write config */
}
uint8_t ina219_get_pga(ina219_handle_t* handle, ina219_pga_t* pga)
{
    uint8_t  res;
    uint16_t prev;
    res  = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */
    *pga = (ina219_pga_t)((prev >> 11) & 0x03);                  /* get pga */
    return 0;                                                    /* success return 0 */
}
uint8_t ina219_set_bus_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t mode)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    prev &= ~(0xF << 7);                                        /* clear mode bit */
    prev |= mode << 7;                                          /* set mode bit */
    return a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev); /* write config */
}
uint8_t ina219_get_bus_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t* mode)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    *mode = (ina219_adc_mode_t)((prev >> 7) & 0xF); /* get mode */
    return 0;                                       /* success return 0 */
}
uint8_t ina219_set_shunt_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t mode)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    prev &= ~(0xF << 3);                                        /* clear mode bit */
    prev |= mode << 3;                                          /* set mode bit */
    return a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev); /* write config */
}
uint8_t ina219_get_shunt_voltage_adc_mode(ina219_handle_t* handle, ina219_adc_mode_t* mode)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    *mode = (ina219_adc_mode_t)((prev >> 3) & 0xF); /* get mode */
    return 0;                                       /* success return 0 */
}

uint8_t ina219_set_mode(ina219_handle_t* handle, ina219_mode_t mode)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    prev &= ~(0x7 << 0);                                       /* clear mode bit */
    prev |= mode << 0;                                         /* set mode bit */
    res = a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev); /* write config */

    if ((mode >= INA219_MODE_SHUNT_VOLTAGE_TRIGGERED) && /* check mode */
        (mode <= INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED)) {
        uint8_t m, mode1, mode2, t;

        mode1 = (prev >> 3) & 0xF;               /* get shunt adc mode */
        mode2 = (prev >> 7) & 0xF;               /* get bus adc mode */
        m     = (mode1 > mode2) ? mode1 : mode2; /* get max mode */
        if (m <= 8)                              /* check mode */
        {
            delay_ms(1); /* delay 1 ms */
        } else {
            t = (uint8_t)(0.532 * pow(2, m - 8)) + 1; /* get time */
            delay_ms(t);                              /* delay time */
        }

        return 0; /* success return 0 */
    } else {
        return 0; /* success return 0 */
    }
}
uint8_t ina219_get_mode(ina219_handle_t* handle, ina219_mode_t* mode)
{
    uint8_t  res;
    uint16_t prev;

    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */

    *mode = (ina219_mode_t)((prev >> 0) & 0x7); /* get mode */
    return 0;                                   /* success return 0 */
}
uint8_t ina219_read_shunt_voltage(ina219_handle_t* handle, int16_t* raw, float* mV)
{
    uint8_t res;
    union {
        uint16_t u;
        int16_t  s;
    } u;

    res = a_ina219_iic_read(INA219_REG_SHUNT_VOLTAGE, (uint16_t*)&u.u); /* read shunt voltage */

    *raw = u.s;                    /* set the raw */
    *mV  = (float)(*raw) / 100.0f; /* set the converted data */
    return 0;                      /* success return 0 */
}
uint8_t ina219_read_bus_voltage(ina219_handle_t* handle, uint16_t* raw, float* mV)
{
    uint8_t res;

    res = a_ina219_iic_read(INA219_REG_BUS_VOLTAGE, (uint16_t*)raw); /* read bus voltage */
    if (((*raw) & (1 << 0)) != 0)
        if (((*raw) & (1 << 1)) == 0)
            *raw = (*raw) >> 3; /* right shift 3 */
    *mV = (float)(*raw) * 4.0f; /* set the converted data */
    return 0;                   /* success return 0 */
}

uint8_t ina219_read_current(ina219_handle_t* handle, int16_t* raw, float* mA)
{
    uint8_t res;
    union {
        uint16_t u;
        int16_t  s;
    } u;

    res = a_ina219_iic_read(INA219_REG_CURRENT, (uint16_t*)&u.u); /* read current */

    *raw = u.s;                                          /* set the raw */
    *mA  = (float)((double)(*raw) * current_lsb * 1000); /* set the converted data */
    return 0;                                            /* success return 0 */
}
uint8_t ina219_read_power(ina219_handle_t* handle, uint16_t* raw, float* mW)
{
    uint8_t res;
    res = a_ina219_iic_read(INA219_REG_POWER, (uint16_t*)raw); /* read power */
    *mW = (float)((double)(*raw) * current_lsb * 20.0 * 1000.0); /* set the converted data */
    return 0;                                                    /* success return 0 */
}
uint8_t ina219_get_calibration(ina219_handle_t* handle, uint16_t* data)
{
    uint8_t res;
    res = a_ina219_iic_read(INA219_REG_CALIBRATION, (uint16_t*)data); /* read calibration */

    *data = (*data) >> 1; /* get data*/
    return 0;             /* success return 0 */
}

uint8_t ina219_calculate_calibration(ina219_handle_t* handle, uint16_t* calibration)
{
    uint8_t  res;
    uint16_t prev;
    uint8_t  pga;
    double   v;
    if ((r >= -0.000001f) && (r <= 0.000001f))                      /* check the r */
        res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read conf */
    pga = (prev >> 11) & 0x3;                                       /* get pga */
    switch (pga)                                                    /* select the pga */
    {
        case 0: {
            v   = 0.04; /* 0.04 V */
            res = 0;    /* set ok */

            break;
        }
        case 1: {
            v   = 0.08; /* 0.08 V */
            res = 0;    /* set ok */

            break;
        }
        case 2: {
            v   = 0.16; /* 0.16 V */
            res = 0;    /* set ok */

            break;
        }
        case 3: {
            v   = 0.32; /* 0.32 V */
            res = 0;    /* set ok */

            break;
        }
        default: {
            v   = 0.0; /* 0.0 V */
            res = 1;   /* set failed */

            break;
        }
    }
    if (res == 1) else
        {
            current_lsb  = v / r / pow(2.0, 15.0);                     /* current lsb */
            *calibration = (uint16_t)(0.04096 / (v / pow(2.0, 15.0))); /* set calibration */

            return 0; /* success return 0 */
        }
}
uint8_t ina219_set_calibration(ina219_handle_t* handle, uint16_t data)
{
    uint8_t res;
    res = a_ina219_iic_write(INA219_REG_CALIBRATION, data << 1); /* write calibration */
    return 0;                                                    /* success return 0 */
}

uint8_t ina219_deinit(ina219_handle_t* handle)
{
    uint8_t  res;
    uint16_t prev;
    res = a_ina219_iic_read(INA219_REG_CONF, (uint16_t*)&prev); /* read config */
    prev &= ~(0x07);                                            /* clear mode */
    res = a_ina219_iic_write(INA219_REG_CONF, (uint16_t)prev);  /* write config */
    res = iic_deinit();                                         /* iic deinit */
    return 0;                                                   /* success return 0 */
}
