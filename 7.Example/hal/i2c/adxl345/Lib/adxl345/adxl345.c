
#include "adxl345.h"

bool adxl345_init()
{
    uint8_t ID = adxl345_read_uint8(ADXL345_REG_DEVID);
    if (ID != ADXL345_DEVICE_ID) {
        println("Unkowun ID: %d", ID);
        return false;
    }

    adxl345_write_uint8(ADXL345_REG_DATA_FORMAT, 0x2B);  // 数据格式
    adxl345_write_uint8(ADXL345_REG_BW_RATE, 0x0A);      // 数据速率及功耗模式
    adxl345_write_uint8(ADXL345_REG_POWER_CTL, 0x28);    // 电源特性
    adxl345_write_uint8(ADXL345_REG_INT_ENABLE, 0);      // 中断使能
    adxl345_write_uint8(ADXL345_REG_OFSX, 0);            // XYZ偏移
    adxl345_write_uint8(ADXL345_REG_OFSY, 0);
    adxl345_write_uint8(ADXL345_REG_OFSZ, 0);

    return true;
}

void adxl345_read_raw(int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t buff[6];

    adxl345_read_bytes(ADXL345_REG_DATAX0, buff, 6);

    *x = (int16_t)((buff[1] << 8) | buff[0]);
    *y = (int16_t)((buff[3] << 8) | buff[2]);
    *z = (int16_t)((buff[5] << 8) | buff[4]);

#if ADXL345_DEBUG  // print pitch & roll
    println("%.2f,%.2f", atan2(*x, *z) * RAD_TO_DEG, atan2(*y, *z) * RAD_TO_DEG);
#endif
}

void adxl345_read_data(float* x, float* y, float* z)
{
    int16_t raw_x, raw_y, raw_z;

    adxl345_read_raw(&raw_x, &raw_y, &raw_z);

    *x = ADXL345_SCALE_FACTOR * raw_x;
    *y = ADXL345_SCALE_FACTOR * raw_y;
    *z = ADXL345_SCALE_FACTOR * raw_z;
}
