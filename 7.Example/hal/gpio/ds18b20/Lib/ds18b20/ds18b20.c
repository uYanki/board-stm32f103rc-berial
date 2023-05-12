#include "ds18b20.h"
GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin   = DS18B20_Pin,
    .Pull  = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_LOW,
};

void DQ_OUT(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

#define DQ_H() HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET)
#define DQ_L() HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET)

void DQ_IN(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

#define DQ_GET()  HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)
#define DQ_IS_L() (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) == GPIO_PIN_RESET)
#define DQ_IS_H() (HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin) == GPIO_PIN_SET)

void HAL_Delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

#define delay_ms(ms) HAL_Delay(ms)
#define delay_us(us) HAL_Delay_us(us)

/*********************************************************************************************************/

#include "stdio.h"

// reutrn 0:器件不存在 1:器件存在
uint8_t DS18B20_Reset()
{
    // reset

    DQ_OUT();

    DQ_L();
    delay_us(520);  // 480~960us
    DQ_H();
    delay_us(15);  // 15~60us

    // check

    uint8_t retry;
    DQ_IN();

    // wait for low level
    retry = 0;
    while (DQ_IS_H()) {
        delay_us(10);
        if (++retry > 20)
            return 0;
    }

    // wait for high level
    retry = 0;
    while (DQ_IS_L()) {
        delay_us(10);
        if (++retry > 24)
            return 0;
    }

    return 1;
}

uint8_t DS18b20_ReadBit(void)
{
    uint8_t dat;
    DQ_OUT();
    DQ_L();
    delay_us(15);  // 拉低总线 ≥15us
    DQ_IN();
    dat = DQ_GET();
    delay_us(40);  // 读取周期 60us
    return dat;
}

uint8_t DS18B20_ReadByte(void)
{
    uint8_t data = 0x00, mask;
    for (mask = 0x01; mask != 0; mask <<= 1)
        if (DS18b20_ReadBit() == GPIO_PIN_SET)
            data |= mask;
    return data;
}

void DS18B20_WriteByte(uint8_t data)
{
    DQ_OUT();
    for (uint8_t mask = 0x01; mask != 0; mask <<= 1) {
        if ((data & mask) == GPIO_PIN_RESET) {
            // write 0
            DQ_L();
            delay_us(70);  // 拉低总线 ≥60us
            DQ_H();
            delay_us(2);  // 恢复时间, >1us
        } else {
            // write 1
            DQ_L();
            delay_us(9);  // 拉低总线 1~15us
            DQ_H();
            delay_us(55);  // 拉高总线 ≥60us
        }
    }
}

// 读温度寄存器的值（原始数据）
int16_t DS18B20_ReadTempReg(void)
{
    uint8_t temp1, temp2;

    if (DS18B20_Reset() == 0) return 0;  // 总线复位

    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0x44);  // 温度转换命令

    DS18B20_Reset();  // 总线复位

    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0xbe);  // 读暂存器

    temp1 = DS18B20_ReadByte();  // 读温度值低字节 LSB
    temp2 = DS18B20_ReadByte();  // 读温度值高字节 MSB

    return (temp2 << 8) | temp1;
}

int16_t DS18B20_ReadTempRegByID(uint8_t* id)
{
    uint8_t temp1, temp2;
    uint8_t i;

    if (DS18B20_Reset() == 0) return 0;

    // Match Rom
    DS18B20_WriteByte(0x55);
    for (i = 0; i < 8; i++)
        DS18B20_WriteByte(id[i]);

    DS18B20_WriteByte(0x44);  // 温度转换命令

    DS18B20_Reset();

    DS18B20_WriteByte(0x55);
    for (i = 0; i < 8; i++)
        DS18B20_WriteByte(id[i]);

    DS18B20_WriteByte(0xbe);

    temp1 = DS18B20_ReadByte();  // 读温度值低字节 LSB
    temp2 = DS18B20_ReadByte();  // 读温度值高字节 MSB

    return ((temp2 << 8) | temp1); /* 返回16位寄存器值 */
}

float DS18B20_ConvertTemp(int16_t temp /*eg: DS18B20_ReadTempReg()*/)
{
    return ((temp < 0) ? (~temp + 1) : temp) * 0.0625;  // 0.0625*16=1
}

// 读DS18B20的ROM里的ID
// return: 0 表示失败， 1表示检测到正确ID
uint8_t DS18B20_ReadID(uint8_t* id)
{
    uint8_t i;

    if (DS18B20_Reset() == 0) return 0;

    // ReadRom
    DS18B20_WriteByte(0x33);

    for (i = 0; i < 8; i++)
        id[i] = DS18B20_ReadByte();

    DS18B20_Reset();
    return 1;
}

void DS18B20_SetResolution(uint8_t res /*9`12*/)
{  // 精度设置
    switch (res) {
        case 9: res = 0; break;
        case 10: res = 1; break;
        case 11: res = 2; break;
        case 12: res = 3; break;
        default: return;
    }

    if (DS18B20_Reset() == 0) return;

    // 写暂存器指令
    DS18B20_WriteByte(0x4e);

    DS18B20_WriteByte(0xff);  // TH
    DS18B20_WriteByte(0xff);  // TL

    // 设置精度 0 res[1-0] 11111
    DS18B20_WriteByte(0x1f | (res << 5));
}

uint8_t dallas_crc8(uint8_t* data, uint8_t size)
{
    uint8_t  crc = 0, inbyte, j, mix;
    uint32_t i;

    for (i = 0; i < size; ++i) {
        inbyte = data[i];
        for (j = 0; j < 8; ++j) {
            mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }

    return crc;
}

// 成功率不高，可能是改变GPIO方向时调用了HAL_GPIO_Init函数，占用了大部分延时
uint8_t DS18B20_ReadTempRegCRC(int16_t* _temp)
{
    uint8_t i;
    uint8_t temp[10], crc;

    for (i = 0; i < 3; i++) {
        if (DS18B20_Reset() == 0) continue;

        DS18B20_WriteByte(0xcc); /* 发命令 */
        DS18B20_WriteByte(0x44); /* 发转换命令 */

        DS18B20_Reset(); /* 总线复位 */

        DS18B20_WriteByte(0xcc); /* 发命令 */
        DS18B20_WriteByte(0xbe);

        temp[0] = DS18B20_ReadByte(); /* 读温度值低字节 */
        temp[1] = DS18B20_ReadByte(); /* 读温度值高字节 */

        temp[2] = DS18B20_ReadByte(); /* Alarm High Byte    */
        temp[3] = DS18B20_ReadByte(); /* Alarm Low Byte     */
        temp[4] = DS18B20_ReadByte(); /* Reserved Byte 1    */
        temp[5] = DS18B20_ReadByte(); /* Reserved Byte 2    */
        temp[6] = DS18B20_ReadByte(); /* Count Remain Byte  */
        temp[7] = DS18B20_ReadByte(); /* Count Per degree C */
        crc     = DS18B20_ReadByte();

        /* 最高的125°对应0x07D0，这里返回0x0800表示错误*/
        if (crc != dallas_crc8(temp, 8)) continue;

        *_temp = (temp[1] << 8) | temp[0]; /* 返回16位寄存器值 */

        return 1; /* 正确 */
    }

    return 0; /* 出错 */
}
