#include "mpu6050.h"
#include <string.h>  // for reset buffer
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu_i2c.h"

#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define MOTION          (0)
#define NO_MOTION       (1)
#define DEFAULT_MPU_HZ  (200)
#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)
#define q30             1073741824.0f

// 陀螺仪方向设置
static signed char gyro_orientation[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

// static signed char gyro_orientation[9] = {-1, 0, 0, 0, -1, 0, 0, 0, 1};

static unsigned short inv_row_2_scale(const signed char* row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;  // error
    return b;
}

static uint16_t inv_orientation_matrix_to_scalar(const int8_t* mtx)
{
    uint16_t scalar;
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}

static uint8_t run_self_test(void)
{
    // return 1;
    int  result;
    long gyro[3], accel[3];
    result = mpu_run_self_test(gyro, accel);
    if (result == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down to the DMP. */
        float    sens;
        uint16_t accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        log_i("setting bias succesfully ......\r\n");
        return 1;
    }
    return 0;
}

uint8_t buffer[14];

int16_t MPU6050_FIFO[6][11];
int16_t Gx_offset = 0, Gy_offset = 0, Gz_offset = 0;

/**
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 */

// 设置时钟源
void MPU6050_SetClockSource(uint8_t source)
{
    i2c_write_bits(MPU6050_DEV, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

// 滤波处理
void MPU6050_NewValues(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz)
{
    uint8_t i, j;
    int32_t sum = 0;
    for (i = 1; i < 10; i++) {  // FIFO 操作
        MPU6050_FIFO[0][i - 1] = MPU6050_FIFO[0][i];
        MPU6050_FIFO[1][i - 1] = MPU6050_FIFO[1][i];
        MPU6050_FIFO[2][i - 1] = MPU6050_FIFO[2][i];
        MPU6050_FIFO[3][i - 1] = MPU6050_FIFO[3][i];
        MPU6050_FIFO[4][i - 1] = MPU6050_FIFO[4][i];
        MPU6050_FIFO[5][i - 1] = MPU6050_FIFO[5][i];
    }
    MPU6050_FIFO[0][9] = ax;  // 将新的数据放置到 数据的最后面
    MPU6050_FIFO[1][9] = ay;
    MPU6050_FIFO[2][9] = az;
    MPU6050_FIFO[3][9] = gx;
    MPU6050_FIFO[4][9] = gy;
    MPU6050_FIFO[5][9] = gz;

    for (j = 0; j < 6; ++j) {
        for (i = sum = 0; i < 10; ++i)
            sum += MPU6050_FIFO[j][i];
        MPU6050_FIFO[j][10] = sum / 10;
    }
}

// 陀螺仪的量程
void MPU6050_SetFullScaleGyroRange(uint8_t range)
{
    i2c_write_bits(MPU6050_DEV, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

// 加速度计的量程
void MPU6050_SetFullScaleAccelRange(uint8_t range)
{
    i2c_write_bits(MPU6050_DEV, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

// 睡眠模式(0工作,1睡觉)
void MPU6050_SetSleepEnabled(uint8_t enabled)
{
    i2c_write_bit(MPU6050_DEV, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

// 读取设备标识（return 0x68）
uint8_t MPU6050_GetDeviceID(void)
{
    memset(buffer, 0, sizeof(buffer));
    i2c_read(MPU6050_DEV, MPU6050_RA_WHO_AM_I, 1, buffer);
    return buffer[0];
}

// 检查器件是否存在
uint8_t MPU6050_TestConnection(void)
{
    return (MPU6050_GetDeviceID() == 0x68);  // 0b01101000
}

//  设置 MPU6050 是否为 AUXI2C 的主机
void MPU6050_SetI2CMasterModeEnabled(uint8_t enabled)
{
    i2c_write_bit(MPU6050_DEV, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

//  设置 MPU6050 是否为 AUXI2C 的主机
void MPU6050_SetI2CBypassEnabled(uint8_t enabled)
{
    i2c_write_bit(MPU6050_DEV, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

// 初始化 MPU6050
void MPU6050_Init(void)
{
    MPU6050_SetClockSource(MPU6050_CLOCK_PLL_YGYRO);
    MPU6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    MPU6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_16);
    MPU6050_SetSleepEnabled(0);
    MPU6050_SetI2CMasterModeEnabled(0);  // 不让 MPU6050 控制 AUXI2C
    MPU6050_SetI2CBypassEnabled(0);      // 主控制器的 I2C 与MPU605 0的 AUXI2C 直通, 控制器可以直接访问HMC5883L。
}

// 初始化 DMP
uint8_t MPU6050_DMP_Init(void)
{
    if (MPU6050_GetDeviceID() != 0x68)
        NVIC_SystemReset();
    if (mpu_init(NULL) == 0) {
        // 使能传感器：陀螺仪 & 加速度计
        if (mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL)) {
            log_i("set sensors failed \r\n");
            return 1;
        } else
            log_i("set sensors successful \r\n");
        // 设置 FIFO
        if (mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)) {
            log_i("set FIFO failed \r\n");
            return 2;
        } else
            log_i("set FIFO successful \r\n");
        // 设置采样率
        if (mpu_set_sample_rate(DEFAULT_MPU_HZ)) {
            log_i("set sample rate failed \r\n");
            return 3;
        } else
            log_i("set sample rate successful \r\n");
        // 加载 dmp 固件
        if (dmp_load_motion_driver_firmware()) {
            log_i("load DMP failed \r\n");
            return 4;
        } else
            log_i("load DMP successful \r\n");
        // 设置陀螺仪方向
        if (dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) {
            log_i("set orientation failed \r\n");
            return 5;
        } else
            log_i("set orientation successful \r\n");
        if (dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |  // 设置dmp功能
                               DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                               DMP_FEATURE_GYRO_CAL)) {
            log_i("set DMP failed \r\n");
            return 6;
        } else
            log_i("set DMP successful \r\n");
        // 设置 DMP 输出速率(最大200Hz)
        if (dmp_set_fifo_rate(DEFAULT_MPU_HZ)) {
            return 7;
        }
        // 自检
        if (run_self_test()) {
            log_i("self test failed \r\n");
            return 8;
        } else
            log_i("self test successful \r\n");
        // 使能DMP
        if (mpu_set_dmp_state(1)) {
            log_i("enable DMP failed \r\n");
            return 9;
        } else
            log_i("enable DMP successful \r\n");
    }
    log_i("initialize MPU6050 successful \r\n");
    return 0;
}

// void MPU6050_DMP_Init(void) {
//     if (MPU6050_GetDeviceID() != 0x68)
//         NVIC_SystemReset();
//     if (!mpu_init(NULL)) {
//         if (!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
//             log_i("mpu_set_sensor complete ......\r\n");
//         if (!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
//             log_i("mpu_configure_fifo complete ......\r\n");
//         if (!mpu_set_sample_rate(DEFAULT_MPU_HZ))
//             log_i("mpu_set_sample_rate complete ......\r\n");
//         if (!dmp_load_motion_driver_firmware())
//             log_i("dmp_load_motion_driver_firmware complete ......\r\n");
//         if (!dmp_set_orientation(
//                 inv_orientation_matrix_to_scalar(gyro_orientation)))
//             log_i("dmp_set_orientation complete ......\r\n");
//         if (!dmp_enable_feature(
//                 DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
//                 DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
//                 DMP_FEATURE_GYRO_CAL))
//             log_i("dmp_enable_feature complete ......\r\n");
//         if (!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
//             log_i("dmp_set_fifo_rate complete ......\r\n");
//         run_self_test();
//         if (!mpu_set_dmp_state(1))
//             log_i("mpu_set_dmp_state complete ......\r\n");
//     }
// }

// 读取内置DMP姿态信息
void MPU6050_Read_DMP(float* pitch, float* roll, float* yaw)
{
    float         q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    unsigned long sensor_timestamp;
    unsigned char more;
    long          quat[4];
    short         gyro[3], accel[3], sensors;
    /* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
     * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
     *  if (sensors & INV_XYZ_GYRO) send_packet(PACKET_TYPE_GYRO, gyro);
     *  if (sensors & INV_XYZ_ACCEL) send_packet(PACKET_TYPE_ACCEL, accel);
     * Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
     * The orientation is set by the scalar passed to dmp_set_orientation during initialization. */
    dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
    if (sensors & INV_WXYZ_QUAT) {
        q0     = quat[0] / q30;
        q1     = quat[1] / q30;
        q2     = quat[2] / q30;
        q3     = quat[3] / q30;
        *pitch = asinf(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;                                       // 俯仰角,精度:0.1°,范围:-90.0° <-->+90.0°
        *roll  = atan2f(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;       // 横滚角,精度:0.1°,范围:-180.0°<-->+180.0°
        *yaw   = atan2f(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;  // 航向角,精度:0.1°,范围:-180.0°<-->+180.0°
    }
}

// 读取温度
int MPU6050_Read_Temperature(void)
{
    float   Temp;
    uint8_t H, L;
    i2c_read(MPU6050_DEV, MPU6050_RA_TEMP_OUT_H, 1, &H);
    i2c_read(MPU6050_DEV, MPU6050_RA_TEMP_OUT_L, 1, &L);
    Temp = (H << 8) + L;
    if (Temp > 32768)
        Temp -= 65536;
    Temp = (36.53 + Temp / 340) * 10;
    return (int)Temp;
}
