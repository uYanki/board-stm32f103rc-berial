#include "bmp280.h"
#include <stdlib.h>

#define dig_T1 bmp280->T1
#define dig_T2 bmp280->T2
#define dig_T3 bmp280->T3
#define dig_P1 bmp280->P1
#define dig_P2 bmp280->P2
#define dig_P3 bmp280->P3
#define dig_P4 bmp280->P4
#define dig_P5 bmp280->P5
#define dig_P6 bmp280->P6
#define dig_P7 bmp280->P7
#define dig_P8 bmp280->P8
#define dig_P9 bmp280->P9

static uint8_t bmp280_read_uint8(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, 0xFF);
    return buff;
}

static uint16_t bmp280_read_word(uint8_t reg)
{
    uint8_t buff[2];
    HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, 2, 0xFF);
    return (buff[1] << 8) | buff[0];
}

static void bmp280_write_byte(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&BMP280_I2C, BMP280_DEV, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

/**
 * 在 bmp280_init() 函数里默认初始化 t_standby 为 0.5ms，
 * 温度和气压的采样精度设为最低，
 * 滤波器系数设为最低，
 * 并且进入 sleep mode。
 */
struct bmp280* bmp280_init()
{
    uint8_t        bmp280_id;
    uint8_t        lsb, msb;
    uint8_t        ctrlmeas_reg, config_reg;
    struct bmp280* bmp280;

    bmp280_id = bmp280_read_uint8(BMP280_CHIPID_REG);
    if (bmp280_id == 0x58) {
        bmp280                     = malloc(sizeof(struct bmp280));
        bmp280->mode               = BMP280_SLEEP_MODE;
        bmp280->t_sb               = BMP280_T_SB1;
        bmp280->p_oversampling     = BMP280_P_MODE_1;
        bmp280->t_oversampling     = BMP280_T_MODE_1;
        bmp280->filter_coefficient = BMP280_FILTER_MODE_1;
    } else {
        printf("Read BMP280 id error!\r\n");
        return NULL;
    }

    /* read the temperature calibration parameters */
    dig_T1 = bmp280_read_word(BMP280_DIG_T1_LSB_REG);
    dig_T2 = bmp280_read_word(BMP280_DIG_T2_LSB_REG);
    dig_T3 = bmp280_read_word(BMP280_DIG_T3_LSB_REG);

    /* read the pressure calibration parameters */
    dig_P1 = bmp280_read_word(BMP280_DIG_P1_LSB_REG);
    dig_P2 = bmp280_read_word(BMP280_DIG_P2_LSB_REG);
    dig_P3 = bmp280_read_word(BMP280_DIG_P3_LSB_REG);
    dig_P4 = bmp280_read_word(BMP280_DIG_P4_LSB_REG);
    dig_P5 = bmp280_read_word(BMP280_DIG_P5_LSB_REG);
    dig_P6 = bmp280_read_word(BMP280_DIG_P6_LSB_REG);
    dig_P7 = bmp280_read_word(BMP280_DIG_P7_LSB_REG);
    dig_P8 = bmp280_read_word(BMP280_DIG_P8_LSB_REG);
    dig_P9 = bmp280_read_word(BMP280_DIG_P9_LSB_REG);

    bmp280_reset(bmp280);

    ctrlmeas_reg = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;
    config_reg   = bmp280->t_sb << 5 | bmp280->filter_coefficient << 2;

    bmp280_write_byte(BMP280_CTRLMEAS_REG, ctrlmeas_reg);
    bmp280_write_byte(BMP280_CONFIG_REG, config_reg);

    HAL_Delay(100);

    return bmp280;
}

void bmp280_reset(struct bmp280* bmp280)
{
    bmp280_write_byte(BMP280_RESET_REG, BMP280_RESET_VALUE);
}

void bmp280_set_standby_time(struct bmp280* bmp280, BMP280_T_SB t_standby)
{
    uint8_t config_reg;

    bmp280->t_sb = t_standby;
    config_reg   = bmp280->t_sb << 5 | bmp280->filter_coefficient << 2;

    bmp280_write_byte(BMP280_CONFIG_REG, config_reg);
}

void bmp280_set_work_mode(struct bmp280* bmp280, BMP280_WORK_MODE mode)
{
    uint8_t ctrlmeas_reg;

    bmp280->mode = mode;
    ctrlmeas_reg = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;

    bmp280_write_byte(BMP280_CTRLMEAS_REG, ctrlmeas_reg);
}

void bmp280_set_temperature_oversampling_mode(struct bmp280* bmp280, BMP280_T_OVERSAMPLING t_osl)
{
    uint8_t ctrlmeas_reg;

    bmp280->t_oversampling = t_osl;
    ctrlmeas_reg           = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;

    bmp280_write_byte(BMP280_CTRLMEAS_REG, ctrlmeas_reg);
}

void bmp280_set_pressure_oversampling_mode(struct bmp280* bmp280, BMP280_P_OVERSAMPLING p_osl)
{
    uint8_t ctrlmeas_reg;

    bmp280->t_oversampling = p_osl;
    ctrlmeas_reg           = bmp280->t_oversampling << 5 | bmp280->p_oversampling << 2 | bmp280->mode;

    bmp280_write_byte(BMP280_CTRLMEAS_REG, ctrlmeas_reg);
}

void bmp280_set_filter_mode(struct bmp280* bmp280, BMP280_FILTER_COEFFICIENT f_coefficient)
{
    uint8_t config_reg;

    bmp280->filter_coefficient = f_coefficient;
    config_reg                 = bmp280->t_sb << 5 | bmp280->filter_coefficient << 2;

    bmp280_write_byte(BMP280_CONFIG_REG, config_reg);
}

/* Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC. */
static double bmp280_compensate_temperature_double(struct bmp280* bmp280, int32_t adc_T)
{
    double var1, var2, temperature;

    var1           = (((double)adc_T) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    var2           = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0) * (((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
    bmp280->t_fine = (int32_t)(var1 + var2);
    temperature    = (var1 + var2) / 5120.0;

    return temperature;
}

/* Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa */
static double bmp280_compensate_pressure_double(struct bmp280* bmp280, int32_t adc_P)
{
    double var1, var2, pressure;

    var1 = ((double)bmp280->t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);

    if (var1 == 0.0) {
        return 0;  // avoid exception caused by division by zero
    }

    pressure = 1048576.0 - (double)adc_P;
    pressure = (pressure - (var2 / 4096.0)) * 6250.0 / var1;
    var1     = ((double)dig_P9) * pressure * pressure / 2147483648.0;
    var2     = pressure * ((double)dig_P8) / 32768.0;
    pressure = pressure + (var1 + var2 + ((double)dig_P7)) / 16.0;

    return pressure;
}

#if 0
static int32_t bmp280_compensate_temperature_int32(struct bmp280* bmp280, int32_t adc_T) {
    int32_t var1, var2, temperature;

    var1           = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2           = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    bmp280->t_fine = var1 + var2;
    temperature    = (bmp280->t_fine * 5 + 128) >> 8;

    return temperature;
}

static uint32_t bmp280_compensate_pressure_int64(struct bmp280* bmp280, int32_t adc_P) {
    int64_t var1, var2, pressure;

    var1 = ((int64_t)bmp280->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0) return 0;  // avoid exception caused by division by zero

    pressure = 1048576 - adc_P;
    pressure = (((pressure << 31) - var2) * 3125) / var1;
    var1     = (((int64_t)dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2     = (((int64_t)dig_P8) * pressure) >> 19;
    pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

    return (uint32_t)pressure;
}
#endif

/* Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC. */
double bmp280_get_temperature(struct bmp280* bmp280)
{
    uint8_t lsb, msb, xlsb;
    int32_t adc_T;
    double  temperature;

    xlsb        = bmp280_read_uint8(BMP280_TEMPERATURE_XLSB_REG);
    lsb         = bmp280_read_uint8(BMP280_TEMPERATURE_LSB_REG);
    msb         = bmp280_read_uint8(BMP280_TEMPERATURE_MSB_REG);

    adc_T       = (msb << 12) | (lsb << 4) | (xlsb >> 4);
    temperature = bmp280_compensate_temperature_double(bmp280, adc_T);

    return temperature;
}

/* Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa */
double bmp280_get_pressure(struct bmp280* bmp280)
{
    uint8_t lsb, msb, xlsb;
    int32_t adc_P;
    double  pressure;

    xlsb     = bmp280_read_uint8(BMP280_PRESSURE_XLSB_REG);
    lsb      = bmp280_read_uint8(BMP280_PRESSURE_LSB_REG);
    msb      = bmp280_read_uint8(BMP280_PRESSURE_MSB_REG);

    adc_P    = (msb << 12) | (lsb << 4) | (xlsb >> 4);
    pressure = bmp280_compensate_pressure_double(bmp280, adc_P);

    return pressure;
}

/**
 * 仅在 BMP280 被设置为 normal mode 后，
 * 可使用该接口直接读取温度和气压。
 */
void bmp280_get_temperature_and_pressure(struct bmp280* bmp280, double* temperature, double* pressure)
{
    *temperature = bmp280_get_temperature(bmp280);
    *pressure    = bmp280_get_pressure(bmp280);
}

/**
 * 当 BMP280 被设置为 forced mode 后，
 * 可使用该接口直接读取温度和气压。
 */
void bmp280_forced_mode_get_temperature_and_pressure(struct bmp280* bmp280, double* temperature, double* pressure)
{
    bmp280_set_work_mode(bmp280, BMP280_FORCED_MODE);

    HAL_Delay(100);

    bmp280_get_temperature_and_pressure(bmp280, temperature, pressure);
}

/**
 * 此 demo 使用 forced mode 以 1s 为周期，
 * 对温度和气压进行数据采集并打印。
 */
void bmp280_demo()
{
    struct bmp280* bmp280;
    double         temperature, pressure;
    bmp280 = bmp280_init();
    if (bmp280 != NULL) {
        while (1) {
            bmp280_forced_mode_get_temperature_and_pressure(bmp280, &temperature, &pressure);
            printf("temperature = %.2f ,pressure = %.2f\r\n", temperature, pressure);
            HAL_Delay(1000);
        }
    } else
        printf("create bmp280 error!\r\n");
}
