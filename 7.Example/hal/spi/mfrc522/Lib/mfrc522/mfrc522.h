#ifndef __RC522_H
#define __RC522_H

#include "spi.h"

#define RC522_SPI hspi3  // <---

#define RC522_RST_SET HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, GPIO_PIN_SET)      // <---
#define RC522_RST_RESET HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, GPIO_PIN_RESET)  // <---

#define RC522_CS_SET HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_SET)      // <---
#define RC522_CS_RESET HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, GPIO_PIN_RESET)  // <---

/*** MF522命令字 ***/

#define RC522_IDLE 0x00        // 取消当前命令
#define RC522_AUTHENT 0x0E     // 验证密钥
#define RC522_RECEIVE 0x08     // 接收数据
#define RC522_TRANSMIT 0x04    // 发送数据
#define RC522_TRANSCEIVE 0x0C  // 发送并接收数据
#define RC522_RESETPHASE 0x0F  // 复位
#define RC522_CALCCRC 0x03     // CRC计算

/*** M1卡片命令字 ***/

// M1: mifare one

#define PICC_REQIDL 0x26     //寻天线区内未进入休眠状态
#define PICC_REQALL 0x52     //寻天线区内全部卡
#define PICC_ANTICOLL1 0x93  //防冲撞
#define PICC_ANTICOLL2 0x95  //防冲撞
#define PICC_AUTHENT1A 0x60  //验证A密钥
#define PICC_AUTHENT1B 0x61  //验证B密钥
#define PICC_READ 0x30       //读块
#define PICC_WRITE 0xA0      //写块
#define PICC_DECREMENT 0xC0  //扣款
#define PICC_INCREMENT 0xC1  //充值
#define PICC_RESTORE 0xC2    //调块数据到缓冲区
#define PICC_TRANSFER 0xB0   //保存缓冲区中数据
#define PICC_HALT 0x50       //休眠

/*** MF522 FIFO 长度定义 ***/

#define DEF_FIFO_LENGTH 64  // FIFO size = 64byte
#define MAXRLEN 18

/*** MF522 寄存器 ***/

// PAGE 0
#define RFU00 0x00
#define COMMAND_REGISTER 0x01
#define IE_REGISTER 0x02
#define DIV_IE_REGISTER 0x03
#define IRQ_REGISTER 0x04
#define DIV_IRQ_REGISTER 0x05
#define ERROR_REGISTER 0x06
#define STATUS1_REGISTER 0x07
#define STATUS2_REGISTER 0x08
#define FIFO_DATA_REGISTER 0x09
#define FIFO_LEVEL_REGISTER 0x0A
#define WATER_LEVEL_REGISTER 0x0B
#define CONTROL_REGISTER 0x0C
#define BIT_FRAMING_REGISTER 0x0D
#define COLLISION_REGISTER 0x0E
#define RFU0F 0x0F
// PAGE 1
#define RFU10 0x10  // RC522 mannual 36/95
#define MODE_REGISTER 0x11
#define TX_MODE_REGISTER 0x12
#define RX_MODE_REGISTER 0x13  // RC522 mannual 9.3.2.4
#define TX_CONTROL_REGISTER 0x14
#define TX_ASK_REGISTER 0x15
#define TX_SEL_REGISTER 0x16
#define RX_SEL_REGISTER 0x17
#define RX_THRESHOLD_REGISTER 0x18
#define DEMOD_REGISTER 0x19
#define RFU1A 0x1A
#define RFU1B 0x1B
#define MIFARE_REGISTER 0x1C
#define RFU1D 0x1D
#define RFU1E 0x1E
#define SERIAL_SPEED_REGISTER 0x1F
// PAGE 2
#define RFU20 0x20
#define CRC_RESULT_M_REGISTER 0x21
#define CRC_RESULT_L_REGISTER 0x22
#define RFU23 0x23
#define MOD_WIDTH_REGISTER 0x24
#define RFU25 0x25
#define RF_CONFIG_REGISTER 0x26
#define GSN_REGISTER 0x27
#define CWF_CONFIG_REGISTER 0x28
#define MODGS_CONFIG_REGISTER 0x29
#define TMODE_REGISTER 0x2A
#define TIMER_PRESCALER_REGISTER 0x2B
#define TIMER_RELOAD_H_REGISTER 0x2C
#define TIMER_RELOAD_L_REGISTER 0x2D
#define TIMER_VALUE_H_REGISTER 0x2E
#define TIMER_VALUE_L_REGISTER 0x2F
// PAGE 3
#define RFU30 0x30
#define TEST_SEL_1_REGISTER 0x31
#define TEST_SEL_2_REGISTER 0x32
#define TEST_PIN_EN_REGISTER 0x33
#define TEST_PIN_VALUE_REGISTER 0x34
#define TEST_BUS_REGISTER 0x35
#define AUTO_TEST_REGISTER 0x36
#define VERSION_REGISTER 0x37
#define ANALOG_TEST_REGISTER 0x38
#define TEST_ADC1_REGISTER 0x39
#define TEST_ADC2_REGISTER 0x3A
#define TEST_ADC_REGISTER 0x3B
#define RFU3C 0x3C
#define RFU3D 0x3D
#define RFU3E 0x3E
#define RFU3F 0x3F

#define MI_OK 0
#define MI_NOTAGERR 1
#define MI_ERR 2

uint8_t RC522_Reset(void);  // 复位

void RC522_AntennaOn(void);   // 开启天线
void RC522_AntennaOff(void);  // 关闭天线

uint8_t RC522_Request(uint8_t req_code, uint8_t* pTagType);                              // 寻卡
uint8_t RC522_Anticoll(uint8_t* pSnr);                                                   // 防冲撞
uint8_t RC522_Select(uint8_t* pSnr);                                                     // 选卡
uint8_t RC522_AuthState(uint8_t auth_mode, uint8_t addr, uint8_t* pKey, uint8_t* pSnr);  // 密码校验

uint8_t RC522_Read(uint8_t addr, uint8_t* pData);   // 读数据
uint8_t RC522_Write(uint8_t addr, uint8_t* pData);  // 写数据
uint8_t RC522_Value(uint8_t dd_mode, uint8_t addr, uint8_t* pValue);
uint8_t RC522_BakValue(uint8_t sourceaddr, uint8_t goaladdr);

uint8_t RC522_Halt(void);  // 休眠

uint8_t RC522_ComMF522(uint8_t Command, uint8_t* pInData, uint8_t InLenByte, uint8_t* pOutData, uint16_t* pOutLenBit);
void    CalulateCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData);
void    dumpHex(uint8_t* buffer, int len);

uint8_t RC522_IsDataBlock(uint8_t addr);  // 判断地址是否位数据库

#endif
