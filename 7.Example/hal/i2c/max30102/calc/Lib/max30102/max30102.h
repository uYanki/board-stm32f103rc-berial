
#ifndef __MAX30102_H_
#define __MAX30102_H_

#include <stdio.h>
#include "i2c.h"

// !!!!!!!!! 程序与MAX30100不通用, 还有官方给的计算心率和血氧的算法实测不太行

#define MAX30102_I2C hi2c2        // <----
#define MAX30102_DEV (0x57 << 1)  // <----

/************************** register addresses **************************/

// Status Registers
#define REG_INTR_STATUS_1 0x00  // INT1 status (main interrupt group)
#define REG_INTR_STATUS_2 0x01  // INT2 status (temp ready interrupt)
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03

// FIFO Registers
#define REG_FIFO_WR_PTR 0x04  // WRITE PTR
#define REG_OVF_COUNTER 0x05  // OVER FLOW
#define REG_FIFO_RD_PTR 0x06  // READ PTR
#define REG_FIFO_DATA 0x07

// Configuration Registers
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A  // particle-sensing mode

// PA = Pulse Amplitude
#define REG_LED1_PA 0x0C  // Red
#define REG_LED2_PA 0x0D  // IR
#define REG_LED3_PA 0x0E  // Green
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12

// Die Temperature Registers 芯片温度
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21

// Proximity Function Registers
#define REG_PROX_INT_THRESH 0x30

// Part ID Registers
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF  // Should always be 0x15

/*********************************************/

// configurations for REG_INTR_ENABLE_1:

#define INT_A_FULL 0x80    // bit7
#define INT_DATA_RDY 0x40  // bit6
#define INT_ALC_OVF 0x20   // bit5
#define INT_PROX_INT 0x00  // bit4

// configurations for REG_INTR_ENABLE_2:
#define INT_DIE_TEMP_RDY 0x02  // bit1

/*********************************************/
// configurations for REG_FIFO_CONFIG

// 3bit[7:5]
#define SAMPLEAVG_MASK 0xE0
typedef enum SampleAverage {
    SAMPLE_AVERAGE_1  = 0x00,
    SAMPLE_AVERAGE_2  = 0x20,
    SAMPLE_AVERAGE_4  = 0x40,
    SAMPLE_AVERAGE_8  = 0x60,
    SAMPLE_AVERAGE_16 = 0x80,
    SAMPLE_AVERAGE_32 = 0xA0,
} SampleAverage;

// 1bit[4]
#define FIFO_ROLLOVER 0x10

// 4bit[3:0]
#define A_FULL_MASK 0x0F

/*********************************************/
// configurations for REG_SPO2_CONFIG

// 2bit[6:5]
#define ADCRANGE_MASK 0x60
typedef enum ADCRange {
    ADC_RANGE_2048  = 0x00,  // 7.81pA per LSB
    ADC_RANGE_4096  = 0x20,  // 15.63pA per LSB
    ADC_RANGE_8192  = 0x40,  // 31.25pA per LSB
    ADC_RANGE_16384 = 0x60,  // 62.5pA per LSB
} ADCRange;

// 3bit[4:2] 采样率
#define SAMPLERATE_MASK 0x1C
typedef enum SampleRate {
    SAMPLE_RATE_50   = 0x00,
    SAMPLE_RATE_100  = 0x04,
    SAMPLE_RATE_200  = 0x08,
    SAMPLE_RATE_400  = 0x0C,
    SAMPLE_RATE_800  = 0x10,
    SAMPLE_RATE_1000 = 0x14,
    SAMPLE_RATE_1600 = 0x18,
    SAMPLE_RATE_3200 = 0x1C,
} SampleRate;

// 2bit[1:0] 采样宽度
#define PULSEWIDTH_MASK 0x03
typedef enum LEDPulseWidth {
    PULSE_WIDTH_69  = 0x00,  // 15 bit resolution
    PULSE_WIDTH_118 = 0x01,  // 16 bit resolution
    PULSE_WIDTH_215 = 0x02,  // 17 bit resolution
    PULSE_WIDTH_411 = 0x03,  // 18 bit resolution
} LEDPulseWidth;

/*********************************************/
// configurations for REG_MODE_CONFIG

// bit[2:0]
#define LEDMODE_MASK 0x07
typedef enum LedMode {
    LED_MODE_REDONLY   = 0x02,  // Red only
    LED_MODE_REDIRONLY = 0x03,  // Red + IR
    LED_MODE_MULTILED  = 0x07,  // Red + IR + Green
} LedMode;

// config Pulse Amplitude / LED Current

typedef enum LEDCurrent {
    // 其他：i = 50ma / 255 * n
    LED_CURRENT_0MA    = 0x00,
    LED_CURRENT_0_4MA  = 0x02,  // 0.4ma
    LED_CURRENT_6_4MA  = 0x1F,  // 6.4ma
    LED_CURRENT_25_4MA = 0x7F,  // 25.4ma
    LED_CURRENT_50MA   = 0xFF,  // 50ma
} LEDCurrent;

/*********************************************/
// configurations for REG_MULTI_LED_CTRL1 & REG_MULTI_LED_CTRL2

#define SLOT1_MASK 0x07
#define SLOT2_MASK 0x70

// 采样值存到FIFO的顺序
typedef enum Slot {
    SLOT_NONE        = 0x00,
    SLOT_RED_LED     = 0x01,
    SLOT_IR_LED      = 0x02,
    SLOT_GREEN_LED   = 0x03,
    SLOT_NONE_PILOT  = 0x04,
    SLOT_RED_PILOT   = 0x05,
    SLOT_IR_PILOT    = 0x06,
    SLOT_GREEN_PILOT = 0x07,
} Slot;

/*********************************************/

typedef struct max30102_fifo_t {
    uint32_t IR;
    uint32_t Red;
    uint32_t Green;
} max30102_fifo_t;

/*************** Read / Write Function ***************/

uint8_t MAX30102_ReadReg(uint8_t reg);
void    MAX30102_WriteReg(uint8_t reg, uint8_t data);
void    MAX30102_MaskBits(uint8_t reg, uint8_t mask, uint8_t newval);
uint8_t MAX30102_WaitBits(uint8_t reg, uint8_t mask, uint32_t timeout);

/*************** Main Function ***************/

void MAX30102_Init(
    uint8_t powerLevel,  // 0=Off, 255=50mA, I≈50.0mA/powerLevel
    uint8_t sampleAverage,
    uint8_t ledMode,
    int     sampleRate,
    int     pulseWidth,
    int     adcRange);

#define MAX30102_ReadPartID() MAX30102_ReadReg(REG_PART_ID)  // return 0x15
#define MAX30102_ReadRevisionID() MAX30102_ReadReg(REG_REV_ID)

// Pull IC out of low power mode 唤醒
#define MAX30102_WakeUp() MAX30102_WriteReg(REG_MODE_CONFIG, 0x00)  // 0x00, bit0
// Poll for bit to clear, reset is then complete (soft reset) 软复位
#define MAX30102_Reset() MAX30102_WriteReg(REG_MODE_CONFIG, 0x40)  // soft reset, 0x40 = 1<<6, bit6
// Put IC into low power mode 休眠
// During shutdown the IC will continue to respond to I2C commands but will
// not update with or take new readings (such as temperature)
#define MAX30102_ShutDown() MAX30102_WriteReg(REG_MODE_CONFIG, 0x80)  // 0x80 = 1<<7, bit7

// 中断使能
#define MAX30102_SetIntEnable1(INTs, enable) MAX30102_MaskBits(REG_INTR_ENABLE_1, INTs, enable ? (INTs) : 0x00)
#define MAX30102_SetIntEnable2(INTs, enable) MAX30102_MaskBits(REG_INTR_ENABLE_2, INTs, enable ? (INTs) : 0x00)
// 读取并清除中断寄存器
#define MAX30102_GetIntState1() MAX30102_ReadReg(REG_INTR_STATUS_1)
#define MAX30102_GetIntState2() MAX30102_ReadReg(REG_INTR_STATUS_2)
// 等待中断寄存器被置1
#define MAX30102_WaitIntState1(INTs) MAX30102_WaitBits(REG_INTR_STATUS_1, INTs, 100)
#define MAX30102_WaitIntState2(INTs) MAX30102_WaitBits(REG_INTR_STATUS_2, INTs, 100)

// 采集n个值，取其平均值作为采样值
#define MAX30102_SetSampleAverage(count) MAX30102_MaskBits(REG_FIFO_CONFIG, SAMPLEAVG_MASK, count)
// Enable/Disable roll over if FIFO over flows
#define MAX30102_SetRollOverEnable(enable) MAX30102_MaskBits(REG_FIFO_CONFIG, FIFO_ROLLOVER, enable ? FIFO_ROLLOVER : 0x00)
// Set number of samples to trigger the almost full interrupt 设置每次采集多少个值时，才会触发 full interrupt 中断
// Power on default is 32 samples, Note it is reverse: 0x00 is 32 samples, 0x0F is 17 samples 设置 0x00 时就是，采集32个值时触发1次中断
#define MAX30102_SetAlmostFull(count) MAX30102_MaskBits(REG_FIFO_CONFIG, A_FULL_MASK, count)

#define MAX30102_GetWritePointer() MAX30102_ReadReg(REG_FIFO_WR_PTR)
#define MAX30102_GetReadPointer() MAX30102_ReadReg(REG_FIFO_RD_PTR)

// Led Mode
extern uint8_t activeLEDs;
#define MAX30102_SetLedMode(mode) MAX30102_MaskBits(REG_MODE_CONFIG, LEDMODE_MASK, mode)
// Led Current
#define MAX30102_SetPulseAmplitude(reg, curret) MAX30102_WriteReg(reg, curret)
#define MAX30102_SetRedCurrent(curret) MAX30102_WriteReg(REG_LED1_PA, curret)
#define MAX30102_SetIRCurrent(curret) MAX30102_WriteReg(REG_LED2_PA, curret)
#define MAX30102_SetGreenCurrent(curret) MAX30102_WriteReg(REG_LED3_PA, curret)
// slot
void MAX30102_SetSlot(uint8_t index /*1~4*/, uint8_t device);
void MAX30102_ClearSlot(void);

// temperature
float MAX30102_ReadTemperature();  // 读取摄氏度

// SpO2
#define MAX30102_SetADCRange(adc_range) MAX30102_MaskBits(REG_SPO2_CONFIG, ADCRANGE_MASK, adc_range)
#define MAX30102_SetSampleRate(sample_rate) MAX30102_MaskBits(REG_SPO2_CONFIG, SAMPLERATE_MASK, sample_rate)
#define MAX30102_SetPulseWidth(pulse_width) MAX30102_MaskBits(REG_SPO2_CONFIG, PULSEWIDTH_MASK, pulse_width)

// fifo
uint8_t MAX30102_CheckFIFO();  // is fifo ready
uint8_t MAX30102_ReadFIFO(uint32_t* red /*out*/, uint32_t* ir /*out*/, uint32_t* green);
uint8_t MAX30102_ReadFIFO2(max30102_fifo_t* fifo);
void    MAX30102_ClearFIFO(void);

//////////////////////////////////////////////////////////////////

void MAX30102Test_PPG(void);  // 脉搏
void MAX30102Test_PPG2(void);
void MAX30102Test_TEMP(void);  // 温度

void MAX30102Test_CalcSampleFreq(void);  // 计算实际采样频率

#endif
