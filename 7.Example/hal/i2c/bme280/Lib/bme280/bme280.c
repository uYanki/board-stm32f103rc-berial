#include "bme280.h"

#ifdef BME280_FLOAT_ENABLE

#define BME280_TEMPERATURE_MIN -40
#define BME280_TEMPERATURE_MAX 85

static double compensate_temperature(uint32_t raw_temp)
{
    double var1, var2;
    double ret;

    var1         = ((double)raw_temp) / 16384.0 - ((double)calib.t1) / 1024.0;
    var1         = var1 * ((double)calib.t2);
    var2         = (((double)raw_temp) / 131072.0 - ((double)calib.t1) / 8192.0);
    var2         = (var2 * var2) * ((double)calib.t3);
    calib.t_fine = (int32_t)(var1 + var2);
    ret          = (var1 + var2) / 5120.0;

    if (ret < BME280_TEMPERATURE_MIN) {
        ret = BME280_TEMPERATURE_MIN;
    } else if (ret > BME280_TEMPERATURE_MAX) {
        ret = BME280_TEMPERATURE_MAX;
    }

    return ret;
}

#define BME280_PRESSURE_MIN 30000.0
#define BME280_PRESSURE_MAX 110000.0

static double compensate_pressure(uint32_t raw_press)
{
    double var1, var2, var3;
    double ret;

    var1 = ((double)calib.t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)calib.p6) / 32768.0;
    var2 = var2 + var1 * ((double)calib.p5) * 2.0;
    var2 = (var2 / 4.0) + (((double)calib.p4) * 65536.0);
    var3 = ((double)calib.p3) * var1 * var1 / 524288.0;
    var1 = (var3 + ((double)calib.p2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)calib.p1);

    /* avoid exception caused by division by zero */
    if (var1 > (0.0)) {
        ret  = 1048576.0 - (double)raw_press;
        ret  = (ret - (var2 / 4096.0)) * 6250.0 / var1;
        var1 = ((double)calib.p9) * ret * ret / 2147483648.0;
        var2 = ret * ((double)calib.p8) / 32768.0;
        ret  = ret + (var1 + var2 + ((double)calib.p7)) / 16.0;

        if (ret < BME280_PRESSURE_MIN) {
            ret = BME280_PRESSURE_MIN;
        } else if (ret > BME280_PRESSURE_MAX) {
            ret = BME280_PRESSURE_MAX;
        }
    } else {
        ret = BME280_PRESSURE_MIN;
    }

    return ret;
}

#define BME280_HUMIDITY_MIN 0.0
#define BME280_HUMIDITY_MAX 100.0

static double compensate_humidity(uint32_t raw_humi)
{
    double var1, var2, var3, var4, var5, var6;
    double ret;

    var1 = ((double)calib.t_fine) - 76800.0;
    var2 = (((double)calib.h4) * 64.0 + (((double)calib.h5) / 16384.0) * var1);
    var3 = raw_humi - var2;
    var4 = ((double)calib.h2) / 65536.0;
    var5 = (1.0 + (((double)calib.h3) / 67108864.0) * var1);
    var6 = 1.0 + (((double)calib.h6) / 67108864.0) * var1 * var5;
    var6 = var3 * var4 * (var5 * var6);
    ret  = var6 * (1.0 - ((double)calib.h1) * var6 / 524288.0);

    if (ret > BME280_HUMIDITY_MAX) {
        ret = BME280_HUMIDITY_MAX;
    } else if (ret < BME280_HUMIDITY_MIN) {
        ret = BME280_HUMIDITY_MIN;
    }

    return ret;
}

#else

#define BME280_TEMPERATURE_MIN -4000
#define BME280_TEMPERATURE_MAX 8500

static int32_t compensate_temperature(uint32_t raw_temp)
{
    int32_t var1;
    int32_t var2;
    int32_t ret;

    var1         = (raw_temp >> 3) - (calib.t1 << 1);
    var1         = (var1 * calib.t2) >> 11;
    var2         = (raw_temp >> 4) - calib.t1;
    var2         = (((var2 * var2) >> 12) * (calib.t3)) >> 14;

    calib.t_fine = var1 + var2;

    ret          = (calib.t_fine * 5 + 128) >> 8;

    if (ret < BME280_TEMPERATURE_MIN) {
        ret = BME280_TEMPERATURE_MIN;
    } else if (ret > BME280_TEMPERATURE_MAX) {
        ret = BME280_TEMPERATURE_MAX;
    }

    return ret;
}

#define BME280_HUMIDITY_MAX    102400

static uint32_t compensate_humidity(uint32_t raw_humi)
{
    int32_t  var1, var2, var3, var4, var5;
    uint32_t ret;

    var1 = calib.t_fine - 76800;
    var2 = raw_humi << 14;
    var3 = calib.h4 << 20;
    var4 = calib.h5 * var1;
    var5 = (var2 - var3 - var4 + 16384) >> 15;
    var2 = (var1 * calib.h6) >> 10;
    var3 = (var1 * calib.h3) >> 11;
    var4 = ((var2 * (var3 + 32768)) >> 10) + 2097152;
    var2 = (var4 * calib.h2 + 8192) >> 14;
    var3 = var5 * var2;
    var4 = ((var3 >> 15) * (var3 >> 15)) >> 7;
    var5 = var3 - ((var4 * calib.h1) >> 4);

    if (var5 < 0)
        var5 = 0;
    else if (var5 > 419430400)
        var5 = 419430400;

    ret = var5 >> 12;

    if (ret > BME280_HUMIDITY_MAX)
        ret = BME280_HUMIDITY_MAX;

    return ret;
}

#ifdef BME280_64BIT_ENABLE

// 64-bit int

#define BME280_PRESSURE_MIN 3000000
#define BME280_PRESSURE_MAX 11000000

static uint32_t compensate_pressure(uint32_t raw_press)
{
    int64_t  var1, var2, var3, var4;
    uint32_t pressure;

    var1 = calib.t_fine - 128000;
    var2 = var1 * var1 * calib.p6;
    var2 += (var1 * calib.p5) << 17;
    var2 += (int64_t)calib.p4 << 35;
    var1 = ((var1 * var1 * calib.p3) >> 8) + ((var1 * calib.p2) >> 12);
    var3 = 140737488355328;
    var1 = ((var3 + var1) * (calib.p1)) >> 33;

    /* To avoid divide by zero exception */

    if (var1) {
        var4     = 1048576 - raw_press;
        var4     = (((var4 << 31) - var2) * 3125) / var1;
        var1     = (calib.p9 * (var4 >> 13) * (var4 >> 13)) >> 25;
        var2     = (calib.p8 * var4) >> 19;
        var4     = ((var4 + var1 + var2) >> 8) + (calib.p7 << 4);

        pressure = ((var4 >> 1) * 100) >> 7;

        if (pressure < BME280_PRESSURE_MIN) {
            pressure = BME280_PRESSURE_MIN;
        } else if (pressure > BME280_PRESSURE_MAX) {
            pressure = BME280_PRESSURE_MAX;
        }
    } else {
        pressure = BME280_PRESSURE_MIN;
    }

    return pressure;
}

#else

// 32-bit int

#define BME280_PRESSURE_MIN 30000
#define BME280_PRESSURE_MAX 110000

static uint32_t compensate_pressure(uint32_t raw_press)
{
    int32_t  var1, var2, var3, var4;
    uint32_t var5;

    uint32_t ret;

    var1 = (calib.t_fine >> 1) - 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * calib.p6;
    var2 += (var1 * calib.p5) << 1;
    var2 = (var2 >> 2) + (calib.p4 << 16);
    var3 = (calib.p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3;
    var4 = (calib.p2 * var1) >> 1;
    var1 = (var3 + var4) >> 18;
    var1 = ((32768 + var1) * calib.p1) >> 15;

    /* avoid exception caused by division by zero */
    if (var1) {
        var5 = 1048576 - raw_press;
        ret  = (var5 - (var2 >> 12)) * 3125;

        if (ret < 0x80000000) {
            ret = (ret << 1) / ((uint32_t)var1);
        } else {
            ret = (ret / (uint32_t)var1) << 1;
        }

        var1 = ((calib.p9) * ((((ret >> 3) * (ret >> 3)) >> 13))) >> 12;
        var2 = (((int32_t)(ret >> 2)) * ((int32_t)calib.p8)) >> 14;
        ret  = (uint32_t)((int32_t)ret + ((var1 + var2 + calib.p7) >> 4));

        if (ret < BME280_PRESSURE_MIN) {
            ret = BME280_PRESSURE_MIN;
        } else if (ret > BME280_PRESSURE_MAX) {
            ret = BME280_PRESSURE_MAX;
        }
    } else {
        ret = BME280_PRESSURE_MIN;
    }

    return ret;
}

#endif

#endif

void bme280_get_data(float* temperature, float* humidity, float* pressure)
{
    uint8_t buff[8];
    i2cdev_read_block(&bme280, BME280_REG_PRESS_MSB, buff, 8);

    // parse sensor data

    uint32_t raw_press = (buff[0] << 12) | (buff[1] << 4) | (buff[2] >> 4);
    uint32_t raw_temp  = (buff[3] << 12) | (buff[4] << 4) | (buff[5] >> 4);
    uint32_t raw_humi  = (buff[6] << 8) | buff[7];

    // compensate data

#ifdef BME280_FLOAT_ENABLE
    double comp_temp  = compensate_temperature(raw_temp);  // 优先计算温度
    double comp_press = compensate_pressure(raw_press);
    double comp_humi  = compensate_humidity(raw_humi);
#else
    int32_t  comp_temp  = compensate_temperature(raw_temp);
    uint32_t comp_press = compensate_pressure(raw_press);
    uint32_t comp_humi  = compensate_humidity(raw_humi);
#endif

#if defined BME280_FLOAT_ENABLE
    *temperature = comp_temp;
    *pressure    = 0.01 * comp_press;
    *humidity    = comp_humi;
#elif defined BME280_32BIT_ENABLE
    *temperature        = 0.01f * comp_temp;
    *pressure           = 0.01f * comp_press;
    *humidity           = 1.0f / 1024.0f * comp_humi;
#elif defined BME280_64BIT_ENABLE
    *temperature = 0.01f * comp_temp;
    *pressure    = 0.0001f * comp_press;
    *humidity    = 1.0f / 1024.0f * comp_humi;
#endif

#if BME280_DEBUG
    // println("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\n", *temperature, *pressure, *humidity);
    println("%0.2lf,%0.2lf,%0.2lf,%0.2lf", *temperature, *pressure, *humidity, bme280_get_altitude_with_sealevel(1011, *pressure));
#endif
}

bool bme280_init(void)
{
    // check ID

    uint8_t ID = i2cdev_get_byte(&bme280, BME280_REG_CHIP_ID, 0x00);

    switch (ID) {
        case 0x56:
        case 0x57:
        case 0x58:
#if BME280_DEBUG
            println("BMP280 ID:0x%02x", ID);
#endif
            return false;
        case 0x60:
#if BME280_DEBUG
            println("BME280 ID:0x%02x", ID);
#endif
            break;
        default:
#if BME280_DEBUG
            println("Unkowun ID:0x%02x", ID);
#endif
            return false;
    }

    // soft reset: make sure IIR is off ...

    i2cdev_write_byte(&bme280, BME280_REG_RESET, 0xB6);

    // if NVM not copied yet, wait for NVM to copy

    do {
        bme280_delay_ms(10);  // startup time = 2ms
    } while (i2cdev_get_byte(&bme280, BME280_REG_STATUS, 0x00) & 0x01);

    // read calibration data

    uint8_t data[26];

    i2cdev_read_block(&bme280, BME280_REG_DIG_T1, data, 26);  // 0x88
    calib.t1 = (data[1] << 8) | data[0];
    calib.t2 = (data[3] << 8) | data[2];
    calib.t3 = (data[5] << 8) | data[4];
    calib.p1 = (data[7] << 8) | data[6];
    calib.p2 = (data[9] << 8) | data[8];
    calib.p3 = (data[11] << 8) | data[10];
    calib.p4 = (data[13] << 8) | data[12];
    calib.p5 = (data[15] << 8) | data[14];
    calib.p6 = (data[17] << 8) | data[16];
    calib.p7 = (data[19] << 8) | data[18];
    calib.p8 = (data[21] << 8) | data[20];
    calib.p9 = (data[23] << 8) | data[22];
    calib.h1 = data[25];

    i2cdev_read_block(&bme280, BME280_REG_DIG_H2, data, 7);  // 0xE1
    calib.h2 = (data[1] << 8) | data[0];
    calib.h3 = data[2];
    calib.h4 = (data[3] << 4) | (data[4] & 0xF);
    calib.h5 = (data[5] << 4) | (data[4] >> 4);
    calib.h6 = data[6];

    return true;
}

/*
   bme280_set_mode(
        BME280_MODE_NORMAL,
        BME280_SAMPLE_X16,
        BME280_SAMPLE_X16,
        BME280_SAMPLE_X16,
        BME280_FILTER_COEFF_16,
        BME280_STANDBY_0P5_MS);
*/
void bme280_set_mode(
    bme280_mode_t    mode,
    bme280_sample_t  humidity,
    bme280_sample_t  temperature,
    bme280_sample_t  pressure,
    bme280_filter_t  filter,
    bme280_standby_t duration)
{
    // sleep before setting

    i2cdev_write_byte(&bme280, BME280_REG_CTRL_MEAS, (uint8_t)BME280_MODE_SLEEP);

    // set BME280_REG_CTRL_MEAS after setting the BME280_REG_CTRL_HUM, otherwise the values won't be applied (see DS 5.4.3)
    i2cdev_write_byte(&bme280, BME280_REG_CTRL_HUM, (uint8_t)humidity);
    i2cdev_write_byte(&bme280, BME280_REG_CONFIG, (uint8_t)(duration << 5) | ((uint8_t)filter << 2) | ((uint8_t)BME280_SPI3W_DISABLE));
    i2cdev_write_byte(&bme280, BME280_REG_CTRL_MEAS, ((uint8_t)temperature << 5) | ((uint8_t)pressure << 2) | ((uint8_t)mode << 0));
}

bool bme280_force_measure(void)
{
    // 测量后自动休眠 sleep after measure
    i2cdev_write_bits(&bme280, BME280_REG_CTRL_MEAS, 0, 2, BME280_MODE_FORCED);

    uint32_t t = bme280_millis();  // wait
    while (i2cdev_get_byte(&bme280, BME280_REG_STATUS, 0x00) & 0x08) {
        if ((bme280_millis() - t) > 2000)
            return false;
        bme280_delay_ms(10);
    }
    return true;
}

/*!
 *   Calculates the altitude (in meters) from the specified atmospheric
 *   pressure (in hPa), and sea-level pressure (in hPa).
 *   @param  seaLevel  sea-level pressure in hPa 海平面气压
 *   @param  atmospheric pressure in hPa
 *   @returns the altitude value read from the device
 */
float bme280_get_altitude_with_sealevel(float sealevel, float atmospheric)
{
    // Equation taken from BMP180 datasheet (page 16):
    //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

    // Note that using the equation from wikipedia can give bad results
    // at high altitude. See this thread for more information:
    //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

    return 44330.0 * (1.0 - pow(atmospheric / sealevel, 0.1903));
}

/*!
 *   Calculates the pressure at sea level (in hPa) from the specified
 * altitude (in meters), and atmospheric pressure (in hPa).
 *   @param  altitude      Altitude in meters
 *   @param  atmospheric   Atmospheric pressure in hPa
 *   @returns the pressure at sea level (in hPa) from the specified altitude
 */
float bme280_sealevel_for_altitude(float altitude, float atmospheric)
{
    // Equation taken from BMP180 datasheet (page 17):
    //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

    // Note that using the equation from wikipedia can give bad results
    // at high altitude. See this thread for more information:
    //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

    return atmospheric / pow(1.0 - (altitude / 44330.0), 5.255);
}

/**
 * @param pressure unit: pa, 1hpa(1百帕) = 100pa(100帕斯卡)
 */
float bme280_get_altitude(float pressure)
{
    return (1.0 - pow(pressure / 101325, 1 / 5.25588)) / 0.0000225577;
}