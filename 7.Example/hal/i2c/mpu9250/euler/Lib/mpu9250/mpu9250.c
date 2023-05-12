#include "mpu9250.h"

void i2c_scan() {
    uint8_t data;
    for (uint8_t addr_7bits, addr_8bits = 1; addr_8bits != 255; addr_8bits += 2) {
        if (HAL_I2C_Mem_Read(&MPU9250_I2C, addr_8bits, 0, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xff) == HAL_OK) {
            addr_7bits = addr_8bits >> 1;
            printf("0x%2x(%d)\r\n", addr_7bits, addr_7bits);
        }
    }
}

void Delay_us(uint32_t t) {
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(t - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

uint8_t MPU9250_Init(void) {
    uint8_t res = 0;
    MPU_WriteByte(MPU_DEV, MPU_PWR_MGMT1, 0X80);  // 复位
    HAL_Delay(100);                               // 延时
    MPU_WriteByte(MPU_DEV, MPU_PWR_MGMT1, 0X00);  // 唤醒
    MPU_SetGyroFsr(3);                            // 陀螺仪:±2000dps
    MPU_SetAccelFsr(0);                           // 加速度:±2g
    MPU_SetRate(50);                              // 采样率:50Hz
    MPU_WriteByte(MPU_DEV, MPU_INT_EN, 0X00);     // 关闭所有中断
    MPU_WriteByte(MPU_DEV, MPU_USER_CTRL, 0X00);  // 关闭I2C主模式
    MPU_WriteByte(MPU_DEV, MPU_FIFO_EN, 0X00);    // 关闭FIFO
    MPU_WriteByte(MPU_DEV, MPU_INTBP_CFG, 0X82);  // INT引脚低电平有效，开启 bypass 模式
    i2c_scan();
    // 地址扫描, 奶奶的，我只能扫描出 MPU6515，扫不到磁力计
    // bypass 模式，其他 i2c 器件的 SDA,SCL 接到 EDA,ECL 上，可直接扫描能出其地址
    printf("mpu id = 0x%2x\r\n", MPU_ReadByte(MPU_DEV, MPU_DEVICE_ID));  // ID 自己看看吧
    if (MPU_ReadByte(MPU_DEV, MPU_DEVICE_ID) == MPU_ID) {
        MPU_WriteByte(MPU_DEV, MPU_PWR_MGMT1, 0X01);  // 设置CLKSEL, PLL X轴为参考
        MPU_WriteByte(MPU_DEV, MPU_PWR_MGMT2, 0X00);  // 启用加速度与陀螺仪
        MPU_SetRate(50);                              // 采样率:50Hz
    } else {
        printf("MPUXXXX init fail\r\n");
        return 1;
    }
    if (MPU_ReadByte(AK8963_DEV, MAG_WIA) == AK8963_ID) {
        MPU_WriteByte(AK8963_DEV, MAG_CNTL1, 0X11);  // 单次测量模式
    } else {
        printf("AK8963 init fail\r\n");
        return 1;
    }
    return 0;
}

// 设置陀螺仪满量程范围 fsr=0:±250dps;1:±500dps;2:±1000dps;3:±2000dps
HAL_StatusTypeDef MPU_SetGyroFsr(uint8_t fsr) {
    return MPU_WriteByte(MPU_DEV, MPU_GYRO_CFG, fsr << 3);
}

// 设置加速度满量程范围 fsr=0:±2g;1:±4g;2:±8g;3:±16g
HAL_StatusTypeDef MPU_SetAccelFsr(uint8_t fsr) {
    return MPU_WriteByte(MPU_DEV, MPU_ACCEL_CFG, fsr << 3);
}

// 设置数字低通滤波器
HAL_StatusTypeDef MPU_SetLPF(uint16_t lpf /*Hz*/) {
    uint8_t data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return MPU_WriteByte(MPU_DEV, MPU_CFG, data);
}

// 设置采样率 fs = 1000 / (rate - 1)
HAL_StatusTypeDef MPU_SetRate(uint16_t rate /*rate:4~100*/) {
    uint8_t fs;
    if (rate > 1000) rate = 1000;
    if (rate < 4) rate = 4;
    fs = 1000 / rate - 1;
    MPU_WriteByte(MPU_DEV, MPU_SAMPLE_RATE, fs);
    return MPU_SetLPF(fs / 2);  // 一半
}

// 获取温度值
float MPU_GetTemperature(void) {
    uint8_t buf[2];
    int16_t raw;
    float   temp;
    MPU_ReadBytes(MPU_DEV, MPU_TEMP_OUTH, 2, buf);
    raw  = (buf[0] << 8) | buf[1];
    temp = 21 + ((double)raw) / 333.87;
    return temp;
}

// 读取陀螺仪原始值
HAL_StatusTypeDef MPU_GetGyroscope(int16_t* gx, int16_t* gy, int16_t* gz) {
    uint8_t           buf[6];
    HAL_StatusTypeDef res;
    res = MPU_ReadBytes(MPU_DEV, MPU_GYRO_XOUTH, 6, buf);
    if (res == HAL_OK) {
        *gx = ((uint16_t)buf[0] << 8) | buf[1];
        *gy = ((uint16_t)buf[2] << 8) | buf[3];
        *gz = ((uint16_t)buf[4] << 8) | buf[5];
    }
    return res;
}
// 读取加速度原始值
HAL_StatusTypeDef MPU_GetAccelerometer(int16_t* ax, int16_t* ay, int16_t* az) {
    uint8_t           buf[6];
    HAL_StatusTypeDef res;
    res = MPU_ReadBytes(MPU_DEV, MPU_ACCEL_XOUTH, 6, buf);
    if (res == 0) {
        *ax = ((uint16_t)buf[0] << 8) | buf[1];
        *ay = ((uint16_t)buf[2] << 8) | buf[3];
        *az = ((uint16_t)buf[4] << 8) | buf[5];
    }
    return res;
}

// 读取加速度原始值
HAL_StatusTypeDef MPU_GetMagnetometer(int16_t* mx, int16_t* my, int16_t* mz) {
    uint8_t           buf[6];
    HAL_StatusTypeDef res = MPU_ReadBytes(AK8963_DEV, MAG_XOUT_L, 6, buf);
    if (res == 0) {
        *mx = ((uint16_t)buf[1] << 8) | buf[0];
        *my = ((uint16_t)buf[3] << 8) | buf[2];
        *mz = ((uint16_t)buf[5] << 8) | buf[4];
    }
    MPU_WriteByte(AK8963_DEV, MAG_CNTL1, 0X11);  // AK8963每次读完以后都需要重新设置为单次测量模式
    return res;
}

HAL_StatusTypeDef MPU_WriteBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t* buf) {
    return HAL_I2C_Mem_Write(&MPU9250_I2C, dev, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xFF);
}

HAL_StatusTypeDef MPU_ReadBytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t* buf) {
    return HAL_I2C_Mem_Read(&MPU9250_I2C, dev, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xFF);
}

HAL_StatusTypeDef MPU_WriteByte(uint8_t dev, uint8_t reg, uint8_t data) {
    return HAL_I2C_Mem_Write(&MPU9250_I2C, dev, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
}

uint8_t MPU_ReadByte(uint8_t dev, uint8_t reg) {
    uint8_t data;
    HAL_I2C_Mem_Read(&MPU9250_I2C, dev, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFF);
    return data;
}

HAL_StatusTypeDef  MPU_CalcAngle(void){
	int16_t ax,ay,az;
	if(MPU_GetAccelerometer(&ax,&ay,&az)==HAL_OK){
		printf("pitch:%f\t,roll:%f\r\n", atan2(ax, az) * RAD_TO_DEG, atan2(ay, az) * RAD_TO_DEG);
		return HAL_OK;
	}
	return HAL_ERROR;
}