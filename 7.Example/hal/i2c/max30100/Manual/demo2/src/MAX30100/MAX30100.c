/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// #include <Wire.h>

#include "MAX30100.h"
#include "delay.h"
#include "myiic.h"
#include "usart.h"

uint16_t rawIRValue;
uint16_t rawRedValue;

void max_begin()  // void MAX30100::begin()
{
    //	  I2C_GPIO_Config();
    //	  I2C_Mode_Config();
    //    Wire.begin();
    //    Wire.setClock(I2C_BUS_SPEED);
    IIC_Init();
    setMode(DEFAULT_MODE);                                            // 往地址0x06的寄存器写0x02（仅打开心率测量模块）
    setLedsPulseWidth(DEFAULT_PULSE_WIDTH);                           // 往地址0x07的寄存器写0x03（16位AD转换，脉冲宽度1.6ms）
    setSamplingRate(DEFAULT_SAMPLING_RATE);                           // 往地址0x07的寄存器写0x01（设置采样率为100）
    setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);  // 往地址0x09的寄存器写0xff（控制Red_led,IR_led电流各为50ma）
    //    setHighresModeEnabled(true);
    setHighresModeEnabled(1);  // 使能血氧饱和度的ADC的分辨率为16位，1.6ms LED脉冲宽度
}

void setMode(Mode mode)  // void MAX30100::setMode(Mode mode)
{
    //    writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
    Write_One_Byte(MAX30100_REG_MODE_CONFIGURATION, mode);
    //	printf("%d\r\n",Write_One_Byte(MAX30100_REG_MODE_CONFIGURATION, mode));
}

void setLedsPulseWidth(LEDPulseWidth ledPulseWidth)  // void MAX30100::setLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
    //    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    //    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
    uint8_t previous;
    previous = Read_One_Byte(MAX30100_REG_SPO2_CONFIGURATION);  // reg 0x07
    printf("Read_One_Byte(0x07); =  0x%x \n", previous);
    Write_One_Byte(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);  // reg 0x07 w
}

void setSamplingRate(SamplingRate samplingRate)  // void MAX30100::setSamplingRate(SamplingRate samplingRate)
{
    //    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    //    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
    uint8_t previous;
    previous = Read_One_Byte(MAX30100_REG_SPO2_CONFIGURATION);                                 // reg 0x07
    Write_One_Byte(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));  // reg 0x07 w
}

void setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)  // void MAX30100::setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
    //    writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
    Write_One_Byte(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);  // reg 0x09
}

void setHighresModeEnabled(u8 enabled)  // void MAX30100::setHighresModeEnabled(bool enabled)
{
    //    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    //    if (enabled) {
    //        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    //    } else {
    //        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    //    }
    uint8_t previous;
    previous = Read_One_Byte(MAX30100_REG_SPO2_CONFIGURATION);
    if (enabled) {
        Write_One_Byte(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    } else {
        Write_One_Byte(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    }
}

void update()  // void MAX30100::update()
{
    readFifoData();
}

void readFifoData(void)  // void MAX30100::readFifoData()
{
    //	int i;
    uint8_t buffer[4];

    Buff_Read(MAX30100_REG_FIFO_DATA, buffer, 4);

    // Warning: the values are always left-aligned
    rawIRValue  = (buffer[0] << 8) | buffer[1];
    rawRedValue = (buffer[2] << 8) | buffer[3];

    printf("rawIRValue =  %d \n", rawIRValue);
    printf("rawRedValue =  %d \n", rawRedValue);
    //	for(i=0; i<4; i++)
    //	{
    //		printf("buffer[%d] =  %d \n",i, buffer[i]);
    //	}
}

u8 Write_One_Byte(u8 addr, u8 data) {
    IIC_Start();
    IIC_Send_Byte(0xAE);  // 发送地址+写命令
    if (IIC_Wait_Ack())   // 等待ACK
    {
        goto RESTATE;
    }
    IIC_Send_Byte(addr);  // 发送寄存器地址
    if (IIC_Wait_Ack())   // 等待ACK
    {
        goto RESTATE;
    }
    IIC_Send_Byte(data);  // 发送数据
    if (IIC_Wait_Ack())   // 等待ACK
    {
        goto RESTATE;
        ;
    }

    IIC_Stop();
    return 1;

RESTATE:
    IIC_Stop();
    return 0;
}
u8 Read_One_Byte(u8 addr) {
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte(0xAE);  // 发送期间地址+写命令
    if (IIC_Wait_Ack())   // 等待ACK
    {
        goto RESTATE;
    }
    IIC_Send_Byte(addr);  // 发送寄存器地址
    if (IIC_Wait_Ack())   // 等待ACK
    {
        goto RESTATE;
    }
    IIC_Start();
    IIC_Send_Byte(0xAF);  // 发送器件地址+读命令
    if (IIC_Wait_Ack())   // 等待ACK
    {
        goto RESTATE;
    }
    res = IIC_Read_Byte(0);  //
    IIC_Stop();              //
    return res;

RESTATE:
    IIC_Stop();
    return 0;
}

u8 Buff_Read(u8 address, u8* buf, u8 len)  // 读取特定地址寄存器里面特定长度的数据
{
    IIC_Start();
    IIC_Send_Byte(0xae);  // 发送器件地址+写命令
    if (IIC_Wait_Ack())   // 等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(address);  // 写寄存器地址
    IIC_Wait_Ack();          // 等待应答
    IIC_Start();
    IIC_Send_Byte(0xaf);  // 发送器件地址+读命令
    IIC_Wait_Ack();       // 等待应答
    while (len) {
        if (len == 1)
            *buf = IIC_Read_Byte(0);  // 读数据,发送nACK
        else
            *buf = IIC_Read_Byte(1);  // 读数据,发送ACK
        len--;
        buf++;
    }
    IIC_Stop();  // 产生一个停止条件
    return 0;
}
