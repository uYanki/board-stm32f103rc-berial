/**
 ******************************************************************************
 * @file    MAX30100.c
 * @author  张东
 * @version V1.0.0
 * @date    2019-12-28
 * @brief   传感器基础驱动程序
 ******************************************************************************
 */
/*--Include-start-------------------------------------------------------------*/
#include "MAX30100.h"
#include "IICcom.h"

/*define ---------------------------------------------------------------------*/
#define max30100_WR_address 0xAE

#define I2C_WR 0 /* 写控制bit */
#define I2C_RD 1 /* 读控制bit */

/*Global data space ----------------------------------------------------------*/

uint8_t max30100_Bus_Write(uint8_t Register_Address, uint8_t Word_Data) {
    /* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

    /* 第1步：发起I2C总线启动信号 */
    I2C_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    I2C_SendByte(max30100_WR_address | I2C_WR); /* 此处是写指令 */

    /* 第3步：发送ACK */
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址 */
    I2C_SendByte(Register_Address);
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第5步：开始写入数据 */
    I2C_SendByte(Word_Data);

    /* 第6步：发送ACK */
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 发送I2C总线停止信号 */
    I2C_Stop();
    return 1; /* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    I2C_Stop();
    return 0;
}

uint8_t max30100_Bus_Read(uint8_t Register_Address) {
    uint8_t data;

    /* 第1步：发起I2C总线启动信号 */
    I2C_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    I2C_SendByte(max30100_WR_address | I2C_WR); /* 此处是写指令 */

    /* 第3步：发送ACK */
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址， */
    I2C_SendByte((uint8_t)Register_Address);
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    I2C_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    I2C_SendByte(max30100_WR_address | I2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第9步：读取数据 */
    {
        data = I2C_RadeByte(); /* 读1个字节 */

        I2C_NoAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
    }
    /* 发送I2C总线停止信号 */
    I2C_Stop();
    return data; /* 执行成功 返回data值 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    I2C_Stop();
    return 0;
}

void max30100_FIFO_Read(uint8_t Register_Address, uint16_t Word_Data[][2], uint8_t count) {
    uint8_t i  = 0;
    uint8_t no = count;
    uint8_t data1, data2;
    /* 第1步：发起I2C总线启动信号 */
    I2C_Start();

    /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    I2C_SendByte(max30100_WR_address | I2C_WR); /* 此处是写指令 */

    /* 第3步：发送ACK */
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址， */
    I2C_SendByte((uint8_t)Register_Address);
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    I2C_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    I2C_SendByte(max30100_WR_address | I2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (I2C_WaitAck() != 0) {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第9步：读取数据 */
    while (no) {
        data1 = I2C_RadeByte();
        I2C_Ack();
        data2 = I2C_RadeByte();
        I2C_Ack();
        Word_Data[i][0] = (((uint16_t)data1 << 8) | data2);  //

        data1 = I2C_RadeByte();
        I2C_Ack();
        data2 = I2C_RadeByte();
        if (1 == no)
            I2C_NoAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        else
            I2C_Ack();
        Word_Data[i][1] = (((uint16_t)data1 << 8) | data2);

        no--;
        i++;
    }
    /* 发送I2C总线停止信号 */
    I2C_Stop();

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    I2C_Stop();
}

void max30100_init(void) {
    max30100_Bus_Write(0x06, 0x0b);  // mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
    // max30100_Bus_Write(0x06, 0x0a);  //MODE[2:0]=010 HR only enabled     when used is mode ,the red led is not used.
    max30100_Bus_Write(0x01, 0xF0);           // open all of interrupt
    max30100_Bus_Write(INTERRUPT_REG, 0x00);  // all interrupt clear
    max30100_Bus_Write(0x09, 0x33);           // r_pa=3,ir_pa=3

#if (SAMPLES_PER_SECOND == 50)
    max30100_Bus_Write(0x07, 0x43);  // SPO2_SR[4:2]=000   50 per second    LED_PW[1:0]=11  16BITS
#elif (SAMPLES_PER_SECOND == 100)
    max30100_Bus_Write(0x07, 0x47);  // SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS
#elif (SAMPLES_PER_SECOND == 200)
    max30100_Bus_Write(0x07, 0x4F);
#elif (SAMPLES_PER_SECOND == 400)
    max30100_Bus_Write(0x07, 0x53);
#endif

    max30100_Bus_Write(0x02, 0x00);  // set FIFO write Pointer reg = 0x00 for clear it
    max30100_Bus_Write(0x03, 0x00);  // set Over Flow Counter  reg = 0x00 for clear it
    max30100_Bus_Write(0x04, 0x0F);  // set FIFO Read Pointer  reg = 0x0f for
                                     // waitting  write pointer eq read pointer   to   interrupts  INTERRUPT_REG_A_FULL
}
