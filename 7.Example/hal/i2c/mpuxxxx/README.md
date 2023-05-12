移植自 [natanaeljr/esp32-MPU-driver](https://github.com/natanaeljr/esp32-MPU-driver) ，esp32(c++) -> stm32(c)

---

支持 MPU6xxx 系列和 MPU9xxx 系列（i2c、spi），不支持 DMP。

| part                                                         | sensors            | protocol     |
| ------------------------------------------------------------ | ------------------ | ------------ |
| **[MPU6000](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/)** | Gyro/Accel         | *I2C*, *SPI* |
| **[MPU6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/)** | Gyro/Accel         | *I2C*        |
| **[MPU6500](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6500/)** | Gyro/Accel         | *I2C*, *SPI* |
| **MPU6555**                                                  | Gyro/Accel         | *I2C*, *SPI* |
| **[MPU9150](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9150/)** | Gyro/Accel/Compass | *I2C*        |
| **[MPU9250](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9250/)** | Gyro/Accel/Compass | *I2C*, *SPI* |
| **MPU9255**                                                  | Gyro/Accel/Compass | *I2C*, *SPI* |

---

#### 引脚连接

##### I2C 模式

| Pin  | Desc         |
| ---- | ------------ |
| SCL  | I2C_SCL      |
| SDA  | I2C_SDA      |
| AD0  | 地址位       |
| NCS  | 需保持低电平 |

注：已板载 MPU6050（I2C2），地址 0x68。当外部 MPU 接 I2C2 时，需将其 AD0 引脚电平拉高。

##### SPI 模式

| Pin  | Desc                |
| ---- | ------------------- |
| SCL  | SPI_SCLK            |
| SDA  | SPI_MOSI            |
| AD0  | SPI_MISO            |
| NCS  | SPI_CS (高电平有效) |

---

#### 使用建议

1、 MPU9xxx初始化唤醒要加100ms以上延时保证内部初始化完毕（地磁计使用内部iic比较慢）；

2、 地磁计数据读取要加150us以上延时保证数据读取完整；

3、 加速度计和陀螺仪的量程（精度）由实际使用情况配置，人体姿态捕获建议加速度计±8g，陀螺仪设置最大±2000°/s；