
#include <math.h>
#include <stdlib.h>

#include "tcs3472x.h"

bool tcs3472x_init(void)
{
    // get device id
    uint8_t id = tcs3472x_read_byte(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ID);
    printv("0x%02x", id);
    if ((id == TCS347X_ID_34721_34725) || (id == TCS347X_ID_34723_34727)) {
        /* Set default integration time and gain */
        tcs3472x_set_integration_time(TCS3472X_INTEGRATION_TIME_50MS);
        tcs3472x_set_gain(TCS3472X_GAIN_1X);
        /* Note: by default, the device is in power down mode on bootup */
        tcs3472x_set_interrupt_enable(false);
        tcs3472x_set_wait_enable(false);
        tcs3472x_set_rgbc_enable(true);
        tcs3472x_set_power_on();
        return true;
    } else {
        return false;
    }
}

bool tcs3472x_read_rgbc(uint16_t* red, uint16_t* green, uint16_t* blue, uint16_t* clear)
{
    // read status
    uint8_t status = tcs3472x_read_byte(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_STATUS);
    // print_binary(status);

    // find rgbc clean channel interrupt
    if ((status >> 4) & 0x01) {
        // clear interrupt, 每次TCS3472X产生中断后,都需要清除中断标志
        tcs3472x_write_byte(TCS3472X_CMD_CLEAR, 0x00);
    }

    // if data ready (这位一直为1,不知道怎么回事)
    if (status & 0x01) {
        uint8_t buf[8];
        tcs3472x_read_block(TCS3472X_CMD_TYPE_AUTO_INCREMENT | TCS3472X_REG_CDATAL, buf, 8);
        *clear = ((uint16_t)buf[1] << 8) | buf[0];
        *red   = ((uint16_t)buf[3] << 8) | buf[2];
        *green = ((uint16_t)buf[5] << 8) | buf[4];
        *blue  = ((uint16_t)buf[7] << 8) | buf[6];
        return true;
    }
    return false;
}

void tcs3472x_set_wait_enable(bool enable) { tcs3472x_write_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 3, enable); }

bool tcs3472x_get_wait_enable(void) { return tcs3472x_read_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 3); }

void tcs3472x_set_rgbc_enable(bool enable) { tcs3472x_write_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 1, enable); }

bool tcs3472x_get_rgbc_enable(void) { return tcs3472x_read_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 1); }

void tcs3472x_set_power_on(void) { tcs3472x_write_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 0, 1); }

void tcs3472x_set_power_off(void) { tcs3472x_write_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 0, 0); }

bool tcs3472x_get_power_mode(void) { return tcs3472x_read_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 0); }

void tcs3472x_set_integration_time(tcs3472x_integration_time_t t) { tcs3472x_write_byte(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ATIME, t); }

tcs3472x_integration_time_t tcs3472x_get_integration_time(void) { return tcs3472x_read_byte(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ATIME); }

void tcs3472x_delay_integration_time(void)
{
    switch (tcs3472x_get_integration_time()) {
        case TCS3472X_INTEGRATION_TIME_2P4MS: delay_ms(3); break;
        case TCS3472X_INTEGRATION_TIME_24MS: delay_ms(24); break;
        case TCS3472X_INTEGRATION_TIME_50MS: delay_ms(50); break;
        case TCS3472X_INTEGRATION_TIME_101MS: delay_ms(101); break;
        case TCS3472X_INTEGRATION_TIME_154MS: delay_ms(154); break;
        case TCS3472X_INTEGRATION_TIME_700MS: delay_ms(700); break;
        default: break;
    }
}

void tcs3472x_set_wait_time(tcs3472x_wait_time_t t)
{
    tcs3472x_write_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_CONFIG, 1, (t & 0x100) >> 8);
    tcs3472x_write_byte(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_WTIME, t & 0xFF);
}

tcs3472x_wait_time_t tcs3472x_get_wait_time(void)
{
    return (tcs3472x_read_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_CONFIG, 1) << 8) |
           tcs3472x_read_byte(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_WTIME);
}

void tcs3472x_delay_wait_time(void)
{
    if (tcs3472x_get_wait_enable()) {
        switch (tcs3472x_get_wait_time()) {
            case TCS3472X_WAIT_TIME_2P4MS: delay_ms(3); break;
            case TCS3472X_WAIT_TIME_204MS: delay_ms(204); break;
            case TCS3472X_WAIT_TIME_614MS: delay_ms(614); break;
            case TCS3472X_WAIT_TIME_29MS: delay_ms(29); break;
            case TCS3472X_WAIT_TIME_2450MS: delay_ms(2450); break;
            case TCS3472X_WAIT_TIME_7400MS: delay_ms(7400); break;
            default: break;
        }
    }
}

void tcs3472x_set_gain(tcs3472x_gain_t gain) { tcs3472x_write_bits(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_CONTROL, 0, 2, gain); }

tcs3472x_gain_t tcs3472x_get_gain(void) { return tcs3472x_read_bits(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_CONTROL, 0, 2); }

void tcs3472x_set_interrupt_enable(bool enable) { tcs3472x_write_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 4, enable); }

bool tcs3472x_get_interrupt_enable(void) { return tcs3472x_read_bit(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_ENABLE, 4); }

void tcs3472x_set_interrupt_mode(tcs3472x_persistence_t mode) { tcs3472x_write_bits(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_PERS, 0, 4, mode); }

tcs3472x_persistence_t tcs3472x_get_interrupt_mode(void) { return tcs3472x_read_bits(TCS3472X_CMD_TYPE_REPEATED_BYTE | TCS3472X_REG_PERS, 0, 4); }

void tcs3472x_set_clear_low_limit(uint16_t threshold) { tcs3472x_write_word(TCS3472X_CMD_TYPE_AUTO_INCREMENT | TCS3472X_REG_AILTL, threshold); }

uint16_t tcs3472x_get_clear_low_limit(void) { return tcs3472x_read_word(TCS3472X_CMD_TYPE_AUTO_INCREMENT | TCS3472X_REG_AILTL); }

void tcs3472x_set_clear_high_limit(uint16_t threshold) { tcs3472x_write_word(TCS3472X_CMD_TYPE_AUTO_INCREMENT | TCS3472X_REG_AILTH, threshold); }

uint16_t tcs3472x_get_clear_high_limit(void) { return tcs3472x_read_word(TCS3472X_CMD_TYPE_AUTO_INCREMENT | TCS3472X_REG_AILTH); }

// https://www.waveshare.net/wiki/TCS34725_Color_Sensor

#define TCS3472X_R_COEF    0.136
#define TCS3472X_G_COEF    1.000
#define TCS3472X_B_COEF    -0.444
#define TCS3472X_GA        1.0
#define TCS3472X_DF        310.0
#define TCS3472X_CT_COEF   3810.0
#define TCS3472X_CT_OFFSET 1391.0

// 色温（单位K）
uint16_t tcs3472x_calculate_color_temperature(uint16_t r, uint16_t g, uint16_t b, uint16_t c)
{
    uint16_t ir;

    ir = r + g + b;
    ir = (ir > c) ? ((ir - c) / 2) : 0;

    return TCS3472X_CT_COEF * (b - ir) / (r - ir) + TCS3472X_CT_OFFSET;
}

uint16_t tcs3472x_calculate_lux(uint16_t r, uint16_t g, uint16_t b, uint16_t c)
{
    float    atime;
    uint16_t ir;
    uint8_t  gain;

    atime = (256 - tcs3472x_get_integration_time()) * 2.4;  // ms

    ir    = r + g + b;
    ir    = (ir > c) ? ((ir - c) / 2) : 0;

    switch (tcs3472x_get_gain()) {
        case TCS3472X_GAIN_1X: gain = 1; break;
        case TCS3472X_GAIN_4X: gain = 4; break;
        case TCS3472X_GAIN_16X: gain = 16; break;
        case TCS3472X_GAIN_60X: gain = 60; break;
    }

    return (TCS3472X_R_COEF * (r - ir) +
            TCS3472X_G_COEF * (g - ir) +
            TCS3472X_B_COEF * (b - ir)) /
           ((atime * gain) / (TCS3472X_GA * TCS3472X_DF));
}

void tcs3472x_get_rgb565_rgb888(uint16_t r, uint16_t g, uint16_t b, uint16_t* rgb565, uint32_t* rgb888)
{
    float i = 1;

    if (r >= g && r >= b) {
        i = r / 255 + 1;
    } else if (g >= r && g >= b) {
        i = g / 255 + 1;
    } else if (b >= g && b >= r) {
        i = b / 255 + 1;
    }

    if (i != 0) {
        r /= i;
        g /= i;
        b /= i;
    }

    if (r > 30) r -= 30;
    if (g > 30) g -= 30;
    if (b > 30) b -= 30;

    r = r * 255 / 225;
    g = g * 255 / 225;
    b = b * 255 / 225;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    *rgb565 = (r << 11) | (g << 5) | (b);
    *rgb888 = (r << 16) | (g << 8) | (b);
}

// RGB 转 HSL

typedef struct color_rgbc {
    uint16_t c;  //[0-65536]
    uint16_t r;
    uint16_t g;
    uint16_t b;
} color_rgbc_t;  // RGBC

typedef struct color_hsl {
    uint16_t h;  //[0,360]
    uint8_t  s;  //[0,100]
    uint8_t  l;  //[0,100]
} color_hsl_t;   // HSL

#define max3v(v1, v2, v3) ((v1) < (v2) ? ((v2) < (v3) ? (v3) : (v2)) : ((v1) < (v3) ? (v3) : (v1)))
#define min3v(v1, v2, v3) ((v1) > (v2) ? ((v2) > (v3) ? (v3) : (v2)) : ((v1) > (v3) ? (v3) : (v1)))

void rgb_to_hsl(color_rgbc_t* rgb, color_hsl_t* hsl)
{
    uint8_t maxVal, minVal, difVal;
    uint8_t r = rgb->r * 100 / rgb->c;  //[0-100]
    uint8_t g = rgb->g * 100 / rgb->c;
    uint8_t b = rgb->b * 100 / rgb->c;

    maxVal    = max3v(r, g, b);
    minVal    = min3v(r, g, b);
    difVal    = maxVal - minVal;

    // 计算亮度
    hsl->l    = (maxVal + minVal) / 2;  //[0-100]

    if (maxVal == minVal)  // 若r=g=b,灰度
    {
        hsl->s = hsl->h = 0;
    } else {
        // 计算色调
        if (maxVal == r) {
            if (g >= b)
                hsl->h = 60 * (g - b) / difVal;
            else
                hsl->h = 60 * (g - b) / difVal + 360;
        } else {
            if (maxVal == g)
                hsl->h = 60 * (b - r) / difVal + 120;
            else if (maxVal == b)
                hsl->h = 60 * (r - g) / difVal + 240;
        }
        // 计算饱和度
        if (hsl->l <= 50)
            hsl->s = difVal * 100 / (maxVal + minVal);  //[0-100]
        else
            hsl->s = difVal * 100 / (200 - (maxVal + minVal));
    }
}

double calculate_color_temperature(uint16_t r, uint16_t g, uint16_t b)
{
    static double M[3][3] =
        {
            {2.789, 1.7517, 1.1302},
            {    1, 4.5907, 0.0601},
            {    0, 0.0565, 5.5943}
    };

    double X = 0, Y = 0, Z = 0;
    double x = 0, y = 0;
    double CCT = 0;
    double n   = 0;

    // 三刺激值
    X          = M[0][0] * r + M[0][1] * g + M[0][2] * b;
    Y          = M[1][0] * r + M[1][1] * g + M[1][2] * b;
    Z          = M[2][0] * r + M[2][1] * g + M[2][2] * b;

    // 色坐标
    x          = X / (X + Y + Z);
    y          = Y / (X + Y + Z);

    // 色温
    n          = (x - 0.3320) / (0.1858 - y);
    CCT        = 437 * n * n * n + 3601 * n * n + 6831 * n + 5517;

    return CCT;
}

uint16_t calculate_brightness(uint16_t r, uint16_t g, uint16_t b)
{
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

temperature_rank_t get_temperature_rank(double cct)
{
    if (cct > 5000)
        return TEMP_RANK_COLD;
    else if (cct > 3300)
        return TEMP_RANK_MIDDLE;
    else
        return TEMP_RANK_WARM;
}

uint16_t tcs3472x_calculate_color_temperature_ex(uint16_t r, uint16_t g, uint16_t b)
{
    float X, Y, Z; /* RGB to XYZ correlation      */
    float xc, yc;  /* Chromaticity co-ordinates   */
    float n;       /* McCamy's formula            */
    float cct;

    /* 1. Map RGB values to their XYZ counterparts.    */
    /* Based on 6500K fluorescent, 3000K fluorescent   */
    /* and 60W incandescent values for a wide range.   */
    /* Note: Y = Illuminance or lux                    */
    X   = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
    Y   = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
    Z   = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

    /* 2. Calculate the chromaticity co-ordinates      */
    xc  = (X) / (X + Y + Z);
    yc  = (Y) / (X + Y + Z);

    /* 3. Use McCamy's formula to determine the CCT    */
    n   = (xc - 0.3320F) / (0.1858F - yc);

    /* Calculate the final CCT */
    cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    /* Return the results in degrees Kelvin */
    return (uint16_t)cct;
}

uint16_t tcs3472x_calculate_lux_ex(uint16_t r, uint16_t g, uint16_t b)
{
    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    return 0.32466F * r + 1.57837F * g + -0.73191F * b;
}
