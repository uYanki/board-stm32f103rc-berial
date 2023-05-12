#include "xmc5883l.h"

#define PI         3.1415926
#define RAD_TO_DEG 57.2958

float mgPerDigit;
float Gauss_LSB_XY = 1090.0;

bool xmc5883l_init()
{
#if defined CONFIG_HMC5883L
    if ((xmc5883l_read_byte(HMC5883L_REG_IDENT_A) != 0x48) ||
        (xmc5883l_read_byte(HMC5883L_REG_IDENT_B) != 0x34) ||
        (xmc5883l_read_byte(HMC5883L_REG_IDENT_C) != 0x33))
        return false;
    printf("ok\r\n");
    xmc5883l_set_range(HMC5883L_RANGE_1_3GA);
    xmc5883l_set_measurement_mode(HMC5883L_CONTINOUS);
    xmc5883l_set_data_rate(HMC5883L_DATARATE_15HZ);
    xmc5883l_set_samples(HMC5883L_SAMPLES_1);
    mgPerDigit = 0.92f;
#elif defined CONFIG_QMC5883L
    xmc5883l_write_byte(QMC5883L_REG_IDENT_B, 0X01);
    xmc5883l_write_byte(QMC5883L_REG_IDENT_C, 0X40);
    xmc5883l_write_byte(QMC5883L_REG_IDENT_D, 0X01);
    xmc5883l_write_byte(QMC5883L_REG_CONFIG_1, 0X1D);
    if ((xmc5883l_read_byte(QMC5883L_REG_IDENT_B) != 0x01) ||
        (xmc5883l_read_byte(QMC5883L_REG_IDENT_C) != 0x40) ||
        (xmc5883l_read_byte(QMC5883L_REG_IDENT_D) != 0x01))
        return false;
    xmc5883l_SetRange(QMC5883L_RANGE_8GA);
    xmc5883l_SetMeasurementMode(QMC5883L_CONTINOUS);
    xmc5883l_SetDataRate(QMC5883L_DATARATE_50HZ);
    xmc5883l_SetSamples(QMC5883L_SAMPLES_1);
    mgPerDigit = 4.35f;
#elif defined CONFIG_VMC5883L
    xmc5883l_write_byte(VCM5883L_CTR_REG1, 0X00);
    xmc5883l_write_byte(VCM5883L_CTR_REG2, 0X4D);
#endif
    return true;
}

vector3_int16* xmc5883l_read_raw(vector3_int16* v)
{
#if defined CONFIG_HMC5883L
    v->x = xmc5883l_read_word(HMC5883L_REG_OUT_X_M);
    v->y = xmc5883l_read_word(HMC5883L_REG_OUT_Y_M);
    v->z = xmc5883l_read_word(HMC5883L_REG_OUT_Z_M);
#elif defined CONFIG_QMC5883L
    v->x       = xmc5883l_read_word(QMC5883L_REG_OUT_X_L);
    v->y       = xmc5883l_read_word(QMC5883L_REG_OUT_Y_L);
    v->z       = xmc5883l_read_word(QMC5883L_REG_OUT_Z_L);
#elif defined CONFIG_VMC5883L
    v->x = -xmc5883l_read_word(VCM5883L_REG_OUT_X_L);
    v->y = -xmc5883l_read_word(VCM5883L_REG_OUT_Y_L);
    v->z = -xmc5883l_read_word(VCM5883L_REG_OUT_Z_L);
#endif
    return v;
}

vector3_float* xmc5883l_calc_angle(vector3_int16* data, vector3_float* angle)
{
    angle->xy = (atan2(data->y, data->x) * RAD_TO_DEG + 180);
    angle->xz = (atan2(data->z, data->x) * RAD_TO_DEG + 180);
    angle->yz = (atan2(data->z, data->y) * RAD_TO_DEG + 180);
    return angle;
}

vector3_int16* xmc5883l_calibrate(vector3_int16* data, vector3_int16* min, vector3_int16* max)
{
    if (data->x < min->x)
        data->x = min->x;
    else if (data->x > max->x)
        data->x = max->x;
    if (data->y < min->y)
        data->y = min->y;
    else if (data->y > max->y)
        data->y = max->y;
    if (data->z < min->z)
        data->z = min->z;
    else if (data->z > max->z)
        data->z = max->z;
    return data;
}

void xmc5883l_set_range(xmc5883l_range_t range)
{
#if defined CONFIG_HMC5883L
    switch (range) {
        case HMC5883L_RANGE_0_88GA: Gauss_LSB_XY = 1370.0; break;
        case HMC5883L_RANGE_1_3GA: Gauss_LSB_XY = 1090.0; break;
        case HMC5883L_RANGE_1_9GA: Gauss_LSB_XY = 820.0; break;
        case HMC5883L_RANGE_2_5GA: Gauss_LSB_XY = 660.0; break;
        case HMC5883L_RANGE_4GA: Gauss_LSB_XY = 440.0; break;
        case HMC5883L_RANGE_4_7GA: Gauss_LSB_XY = 390.0; break;
        case HMC5883L_RANGE_5_6GA: Gauss_LSB_XY = 330.0; break;
        case HMC5883L_RANGE_8_1GA: Gauss_LSB_XY = 230.0; break;
        default: break;
    }
    xmc5883l_write_byte(HMC5883L_REG_CONFIG_B, range << 5);
#elif defined CONFIG_QMC5883L
    switch (range) {
        case QMC5883L_RANGE_2GA: mgPerDigit = 1.22f; break;
        case QMC5883L_RANGE_8GA: mgPerDigit = 4.35f; break;
        default: break;
    }
    xmc5883l_write_byte(QMC5883L_REG_CONFIG_2, range << 4);
#elif defined CONFIG_VMC5883L
    // default 8G
#endif
}

xmc5883l_range_t xmc5883l_get_range(void)
{
#if defined CONFIG_HMC5883L  // bit5
    return xmc5883l_read_bit(HMC5883L_REG_CONFIG_B, 5);
#elif defined CONFIG_QMC5883L  // bit4
    return xmc5883l_read_bit(QMC5883L_REG_CONFIG_2, 4);
#elif defined CONFIG_VMC5883L
    return QMC5883L_RANGE_8GA;
#endif
}

void xmc5883l_set_measurement_mode(xmc5883l_mode_t mode)
{
#if defined CONFIG_HMC5883L  // 0b00000011
    xmc5883l_write_bits(HMC5883L_REG_MODE, 0, 2, mode);
#elif defined CONFIG_QMC5883L  // 0b00000011
    xmc5883l_write_bits(QMC5883L_REG_CONFIG_1, 0, 2, mode);
#elif defined CONFIG_VMC5883L  // 0b00000001
    xmc5883l_write_bit(QMC5883L_REG_CONFIG_1, 0, mode);
#endif
}

xmc5883l_mode_t xmc5883l_get_measurement_mode(void)
{
#if defined CONFIG_HMC5883L
    return xmc5883l_read_bits(HMC5883L_REG_MODE, 0, 2);
#elif defined CONFIG_QMC5883L
    return xmc5883l_read_bits(QMC5883L_REG_CONFIG_1, 0, 2);
#elif defined CONFIG_VMC5883L
    return xmc5883l_read_bit(VCM5883L_CTR_REG2, 0);
#endif
}

void xmc5883l_set_data_rate(xmc5883l_data_rate_t dataRate)
{
#if defined CONFIG_HMC5883L  // 0b00011100
    xmc5883l_write_bits(HMC5883L_REG_CONFIG_A, 2, 3, dataRate);
#elif defined CONFIG_QMC5883L  // 0b00001100
    xmc5883l_write_bits(QMC5883L_REG_CONFIG_1, 2, 2, dataRate);
#elif defined CONFIG_VMC5883L  // 0b00001100
    xmc5883l_write_bits(VCM5883L_CTR_REG2, 2, 2, dataRate);
#endif
}

xmc5883l_data_rate_t xmc5883l_get_data_rate(void)
{
#if defined CONFIG_HMC5883L
    return xmc5883l_read_bits(HMC5883L_REG_CONFIG_A, 2, 3);
#elif defined CONFIG_QMC5883L
    return xmc5883l_read_bits(QMC5883L_REG_CONFIG_1, 2, 2);
#elif defined CONFIG_VMC5883L
    return xmc5883l_read_bits(VCM5883L_CTR_REG2, 2, 2);
#endif
}

void xmc5883l_set_samples(xmc5883l_samples_t samples)
{
#if defined CONFIG_HMC5883L  // 0b01100000
    xmc5883l_write_bits(HMC5883L_REG_CONFIG_A, 5, 2, samples);
#elif defined CONFIG_QMC5883L || defined CONFIG_VMC5883L  // 0b11000000
    xmc5883l_write_bits(QMC5883L_REG_CONFIG_1, 6, 2, samples);
#endif
}

xmc5883l_samples_t xmc5883l_get_samples(void)
{
#if defined CONFIG_HMC5883L
    return xmc5883l_read_bits(HMC5883L_REG_CONFIG_A, 5, 2);
#elif defined CONFIG_QMC5883L || defined CONFIG_VMC5883L
    return xmc5883l_read_bits(QMC5883L_REG_CONFIG_1, 6, 2);
#endif
}

float_t xmc5883l_get_heading(vector3_int16* raw, float_t declination)
{
    // double XuT,YuT,ZuT;
    // ReadRaw();
    // XuT = raw->x / Gauss_LSB_XY * 100;
    // YuT = raw->y / Gauss_LSB_XY * 100;
    // ZuT = raw->z / Gauss_LSB_XY * 100;
    float_t heading = atan2(raw->y, raw->x);
    heading += declination;
    if (heading < 0)
        heading += 2 * PI;
    else if (heading > 2 * PI)
        heading -= 2 * PI;
    return heading * 180 / PI;
}

void compass(float angle /* angle.xy */)
{
    /*

Refer the following application note for heading calculation.
http://www.ssec.honeywell.com/magnetic/datasheets/lowcost.pdf
----------------------------------------------------------------------------------------
atan2(y, x) is the angle in radians between the positive x-axis of a plane and the point
given by the coordinates (x, y) on it.
----------------------------------------------------------------------------------------

This sketch does not utilize the magnetic component Z as tilt compensation can not be done without an Accelerometer

----------------->y
|
|
|
|
|
|
\/
x



     N
 NW  |  NE
     |
W----------E
     |
 SW  |  SE
     S

*/

    // Print the approximate direction

    char* direction;

    if ((angle < 22.5) || (angle > 337.5))
        direction = "South";
    else if ((angle > 22.5) && (angle < 67.5))
        direction = "South-West";
    else if ((angle > 67.5) && (angle < 112.5))
        direction = "West";
    else if ((angle > 112.5) && (angle < 157.5))
        direction = "North-West";
    else if ((angle > 157.5) && (angle < 202.5))
        direction = "North";
    else if ((angle > 202.5) && (angle < 247.5))
        direction = "NorthEast";
    else if ((angle > 247.5) && (angle < 292.5))
        direction = "East";
    else if ((angle > 292.5) && (angle < 337.5))
        direction = "SouthEast";

    // Angle between X-axis and the South direction
    if (angle > 180) angle = 360 - angle;

    println("[ %.2f deg ] %s", angle, direction);
}