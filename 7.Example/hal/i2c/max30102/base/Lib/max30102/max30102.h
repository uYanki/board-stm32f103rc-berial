
#ifndef __MAX30102_H_
#define __MAX30102_H_

#include "i2cdev/i2cdev.h"

// !!!!!!!!! 程序与MAX30100不通用, 还有官方给的计算心率和血氧的算法实测不太行

#define MAX30102_ADDRESS 0x57

static struct i2c_cli max30102 = {
    .bus = &hi2c2,  // <----
    .drv = &hwi2c_drv,
    .dev = MAX30102_ADDRESS << 1,  // <----
    .ops = I2C_DEV_7BIT | I2C_REG_8BIT,
};

//////////////////////////////////////////////////////////////////////////////// registers

// Status
#define MAX30102_REG_INTR_STATUS_1   0x00  // interrupt status 1
#define MAX30102_REG_INTR_STATUS_2   0x01  // interrupt status 2
#define MAX30102_REG_INTR_ENABLE_1   0x02  // interrupt enable 1
#define MAX30102_REG_INTR_ENABLE_2   0x03  // interrupt enable 2

// FIFO
#define MAX30102_REG_FIFO_WR_PTR     0x04  // fifo write pointer
#define MAX30102_REG_OVF_COUNTER     0x05  // overflow counter
#define MAX30102_REG_FIFO_RD_PTR     0x06  // fifo read pointer
#define MAX30102_REG_FIFO_DATA       0x07  // fifo data

// Configuration
#define MAX30102_REG_FIFO_CONFIG     0x08  // fifo config
#define MAX30102_REG_MODE_CONFIG     0x09  // mode config
#define MAX30102_REG_SPO2_CONFIG     0x0A  // spo2 config

// PA = Pulse Amplitude
#define MAX30102_REG_LED1_PA         0x0C  // red led pulse amplitude
#define MAX30102_REG_LED2_PA         0x0D  // iR led pulse amplitude
#define MAX30102_REG_LED3_PA         0x0E  // green led pulse amplitude (max30101)
#define MAX30102_REG_PILOT_PA        0x10  // proximity led pulse amplitude
#define MAX30102_REG_MULTI_LED_CTRL1 0x11
#define MAX30102_REG_MULTI_LED_CTRL2 0x12

// Die Temperature 芯片温度
#define MAX30102_REG_TEMP_INTR       0x1F  // die temperature integer
#define MAX30102_REG_TEMP_FRAC       0x20  // die temperature fraction
#define MAX30102_REG_TEMP_CONFIG     0x21

// Proximity 接近传感器中断阈值
#define MAX30102_REG_PROX_INT_THRESH 0x30

// Part ID
#define MAX30102_REG_REV_ID          0xFE  // revision id
#define MAX30102_REG_PART_ID         0xFF  // part id = 0x15

//////////////////////////////////////////////////////////////////////////////// config

///////////////////////////// reg: REG_INTR_ENABLE_x & MAX30102_REG_INTR_STATUS_x

typedef enum {
    // MAX30102_REG_INTR_STATUS_1:
    MAX30102_INTERRUPT_A_FULL_EN       = 7,
    MAX30102_INTERRUPT_PPG_RDY_EN      = 6,
    MAX30102_INTERRUPT_ALC_OVF_EN      = 5,
    MAX30102_INTERRUPT_PROX_INT_EN     = 4,
    // MAX30102_REG_INTR_STATUS_2:
    MAX30102_INTERRUPT_DIE_TEMP_RDY_EN = 1,
} max30102_interrupt_enable_t;

typedef enum {
    // MAX30102_REG_INTR_STATUS_1:
    MAX30102_INTERRUPT_STATUS_A_FULL       = 7,  // fifo almost full
    MAX30102_INTERRUPT_STATUS_PPG_RDY      = 6,  // new fifo data ready
    MAX30102_INTERRUPT_STATUS_ALC_OVF      = 5,  // ambient light cancellation overflow
    MAX30102_INTERRUPT_STATUS_PROX_INT     = 4,  // proximity threshold triggered 接近传感器
    MAX30102_INTERRUPT_STATUS_PWR_RDY      = 0,  // power ready
    // MAX30102_REG_INTR_STATUS_2:
    MAX30102_INTERRUPT_STATUS_DIE_TEMP_RDY = 1,  // internal temperature ready
} max30102_interrupt_status_t;

///////////////////////////// reg: REG_FIFO_CONFIG

// 3bit[7:5]
typedef enum {
    MAX30102_SAMPLE_AVERAGE_1  = 0,
    MAX30102_SAMPLE_AVERAGE_2  = 1,
    MAX30102_SAMPLE_AVERAGE_4  = 2,
    MAX30102_SAMPLE_AVERAGE_8  = 3,
    MAX30102_SAMPLE_AVERAGE_16 = 4,
    MAX30102_SAMPLE_AVERAGE_32 = 5,
} max30102_sample_average_t;

// 1bit[4]
// fifo roll over enable

// 4bit[3:0]
// fifo almost full

///////////////////////////// reg: REG_SPO2_CONFIG

// 2bit[6:5]
typedef enum {
    MAX30102_ADC_RANGE_2048  = 0,  // 7.81pA per LSB
    MAX30102_ADC_RANGE_4096  = 1,  // 15.63pA per LSB
    MAX30102_ADC_RANGE_8192  = 2,  // 31.25pA per LSB
    MAX30102_ADC_RANGE_16384 = 3,  // 62.5pA per LSB
} max30102_adc_range_t;

// 3bit[4:2] 采样率
typedef enum {
    MAX30102_SAMPLE_RATE_50_HZ   = 0,
    MAX30102_SAMPLE_RATE_100_HZ  = 1,
    MAX30102_SAMPLE_RATE_200_HZ  = 2,
    MAX30102_SAMPLE_RATE_400_HZ  = 3,
    MAX30102_SAMPLE_RATE_800_HZ  = 4,
    MAX30102_SAMPLE_RATE_1000_HZ = 5,
    MAX30102_SAMPLE_RATE_1600_HZ = 6,
    MAX30102_SAMPLE_RATE_3200_HZ = 7,
} max30102_sample_rate_t;

// 2bit[1:0] 脉宽/分辨率
typedef enum {
    MAX30102_ADC_RESOLUTION_15_BIT = 0,  // pulse width =  69( 68.95) us , adc resolution = 15 bits
    MAX30102_ADC_RESOLUTION_16_BIT = 1,  // pulse width = 117(117.78) us , adc resolution = 16 bits
    MAX30102_ADC_RESOLUTION_17_BIT = 2,  // pulse width = 215(215.44) us , adc resolution = 17 bits
    MAX30102_ADC_RESOLUTION_18_BIT = 3,  // pulse width = 410(411.74) us , adc resolution = 18 bits
} max30102_adc_resolution_t;

// 注：ADC的最大采样率取决于所选脉宽，而脉宽又决定了ADC分辨率。（手册 Table 11 & Table 12）
// - SpO2 Mode: MAX30102_ADC_RESOLUTION_18_BIT -> MAX30102_SAMPLE_RATE_400_HZ(max)
// - Heart Rate Mode: MAX30102_ADC_RESOLUTION_18_BIT -> MAX30102_SAMPLE_RATE_1000_HZ(max)

// - The longer the pulse width the longer range of detection you'll have, 脉冲宽度越长，检测范围越长
// - At 69us and 0.4mA it's about 2 inches
// - At 411us and 0.4mA it's about 6 inches

///////////////////////////// reg: REG_MODE_CONFIG

// 3bit[2:0]
typedef enum {
    MAX30102_MODE_HEART_RATE = 2,  // heart rate mode, red only
    MAX30102_MODE_SPO2       = 3,  // spo2 mode, red + ir
    MAX30102_MODE_MULTI_LED  = 7,  // multi-led mode , red + ir
} max30102_mode_t;

///////////////////////////// reg: MAX30102_REG_xxx_PA

typedef enum {
    MAX30102_LED_NONE  = 0,
    MAX30102_LED_RED   = 1,
    MAX30102_LED_IR    = 2,
    MAX30102_LED_PILOT = 3,  // 接近传感器
} max30102_led_t;

// LED Current（以下值在手册中可找到）
// - 0x02, 0.4mA - Presence detection of ~4 inch
// - 0x1F, 6.4mA - Presence detection of ~8 inch (default)
// - 0x7F, 25.4mA - Presence detection of ~8 inch
// - 0xFF, 50.0mA - Presence detection of ~12 inch (typical)
typedef enum {
    MAX30102_LED_CURRENT_0MA    = 0x00,
    MAX30102_LED_CURRENT_0_2MA  = 0x01,  // 0.4ma
    MAX30102_LED_CURRENT_0_4MA  = 0x02,  // 0.4ma
    MAX30102_LED_CURRENT_3_1MA  = 0x0F,  // 3.1ma
    MAX30102_LED_CURRENT_6_4MA  = 0x1F,  // 6.4ma
    MAX30102_LED_CURRENT_12_5MA = 0x3F,  // 12.5ma
    MAX30102_LED_CURRENT_25_4MA = 0x7F,  // 25.4ma
    MAX30102_LED_CURRENT_50MA   = 0xFF,  // 50ma
} max30102_led_current_t;

#define MAX30102_LED_CURRENT(ma) (uint8_t)((float)ma / 50 * 255)  // 0ma <ma < 50ma

///////////////////////////// reg: REG_MULTI_LED_CTRL1, REG_MULTI_LED_CTRL2

// 采样值储存到FIFO的顺序

typedef enum {
    MAX30102_SLOT_NUM_1 = 0,
    MAX30102_SLOT_NUM_2 = 1,
    MAX30102_SLOT_NUM_3 = 2,
    MAX30102_SLOT_NUM_4 = 3,
} max30102_slot_num_t;

typedef enum {
    MAX30102_SLOT_LED_NONE      = 0,
    MAX30102_SLOT_LED_RED       = 1,
    MAX30102_SLOT_LED_IR        = 2,
    MAX30102_SLOT_LED_RED_PILOT = 5,
    MAX30102_SLOT_LED_IR_PILOT  = 6,
} max30102_slot_led_t;

//////////////////////////////////////////////////////////////////////////////// read / write

typedef struct {
    uint8_t   rshift;    // right shift amount for raw value
    uint8_t   unitsize;  // size of unit
    uint8_t   length;    // effective length
    uint8_t   capacity;  // maximum readable length
    uint8_t   buffsize;  // buffer size
    uint32_t* buffaddr;  //  buffer addr
} max30102_fifo_params_t;

//////////////////////////////////////////////////////////////////////////////// read / write

#define max30102_read_bytes(reg, buff, len)        i2cdev_read_block(&max30102, reg, buff, len)
#define max30102_read_byte(reg)                    i2cdev_get_byte(&max30102, reg, false)
#define max30102_write_byte(reg, data)             i2cdev_write_byte(&max30102, reg, data)
#define max30102_read_bit(reg, index)              i2cdev_get_bit(&max30102, reg, index, false)
#define max30102_write_bit(reg, index, data)       i2cdev_write_bit(&max30102, reg, index, data)
#define max30102_read_bits(reg, start, len)        i2cdev_get_bits(&max30102, reg, start, len, false)
#define max30102_write_bits(reg, start, len, data) i2cdev_write_bits(&max30102, reg, start, len, data)

//////////////////////////////////////////////////////////////////////////////// functions

// Mode
max30102_mode_t max30102_get_mode(void);
void            max30102_set_mode(max30102_mode_t mode);

// Pull IC out of low power mode 唤醒
void max30102_wakeup(void);
// Poll for bit to clear, reset is then complete (soft reset) 软复位
void max30102_reset(void);
// Put IC into low power mode 休眠
// During shutdown the IC will continue to respond to I2C commands but will
// not update with or take new readings (such as temperature)
void max30102_shutdown(void);

///////////////////////////// Chip ID

uint8_t max30102_get_revision_id(void);
uint8_t max30102_get_part_id(void);
bool    max30102_check_device(void);

///////////////////////////// Interrupt

bool max30102_get_interrupt_enable(max30102_interrupt_enable_t type);
void max30102_set_interrupt_enable(max30102_interrupt_enable_t type, bool enable);
bool max30102_get_interrupt_status(max30102_interrupt_status_t type);
bool max30102_wait_interrupt_status(max30102_interrupt_status_t type, bool status, uint32_t timeout /* ms */);

///////////////////////////// LED config

// Led Current

uint8_t max30102_get_led_current(max30102_led_t led);
void    max30102_set_led_current(max30102_led_t led, uint8_t current);

// FIFO data order 数据储存顺序 ( mode = MAX30102_MODE_MULTI_LED )

// The slots should be enabled in order  (i.e., SLOT1 should not be disabled if SLOT2 is enabled).

max30102_led_t max30102_get_slot(max30102_slot_num_t slot);
void           max30102_set_slot(max30102_slot_num_t slot, max30102_slot_led_t led);
void           max30102_clear_slot(void);

///////////////////////////// SpO2 config

// ADC 范围
max30102_adc_range_t max30102_get_spo2_adc_range(void);
void                 max30102_set_spo2_adc_range(max30102_adc_range_t adc_range);

// 采样率
max30102_sample_rate_t max30102_get_spo2_sample_rate(void);
void                   max30102_set_spo2_sample_rate(max30102_sample_rate_t sample_rate);

// ADC 分辨率
max30102_adc_resolution_t max30102_get_spo2_adc_resolution(void);
void                      max30102_set_spo2_adc_resolution(max30102_adc_resolution_t pulse_width);

///////////////////////////// FIFO

// 每次采集n个值，取其平均值作为采样值
max30102_sample_average_t max30102_get_fifo_sample_average(void);
void                      max30102_set_fifo_sample_average(max30102_sample_average_t sample);

// Enable/Disable roll over if FIFO over flows
bool max30102_get_fifo_roll_over(void);
void max30102_set_fifo_roll_over(bool enable);

// Set number of samples to trigger the almost full interrupt 设置每次采集多少个值时，才会触发 full interrupt 中断
// Power on default is 32 samples, Note it is reverse: 0x00 is 32 samples, 0x0F is 17 samples 设置 0x00 时就是，采集32个值时触发1次中断
uint8_t max30102_get_fifo_almost_full(void);
void    max30102_set_fifo_almost_full(uint8_t value);

// When the FIFO is full, samples are not pushed on to the FIFO, samples are lost. OVF_COUNTER counts the number
// of samples lost. It saturates at 0xF. When a complete sample is “popped” (i.e., removal of old FIFO data and shifting the
// samples down) from the FIFO (when the read pointer advances), OVF_COUNTER is reset to zero.
uint8_t max30102_get_fifo_overflow_counter(void);
void    max30102_set_fifo_overflow_counter(uint8_t counter);

uint8_t max30102_get_fifo_read_pointer(void);
void    max30102_set_fifo_read_pointer(uint8_t pointer /* <= 32 */);  // use when data communication error

uint8_t max30102_get_fifo_write_pointer(void);
void    max30102_set_fifo_write_pointer(uint8_t pointer /* <= 32 */);

// read raw
typedef struct {
    uint32_t red;
    uint32_t ir;
    uint32_t red_pilot;
    uint32_t ir_pilot;
} max30102_fifo_data;

max30102_fifo_params_t max30102_generate_fifo_params(uint8_t buffsize);

uint8_t max30102_read_fifo(uint32_t* buff, max30102_fifo_params_t* params);

#define MAX30102_CHECK_DATA_READY 0  // 0:寄存器方式 1:引脚电平检测方式
bool max30102_is_ppg_data_ready(uint32_t timeout /* ms */);

void max30102_clear_fifo(void);

///////////////////////////// temperature

// threshold = 0x01 -> 17-bit ADC value of 1023(decimal) or higher triggers the PROX_INT interrupt.  (1<<17)/1024=128, (1<<17)/256=512
// threshold = 0xFF -> only a saturated ADC triggers the interrupt. 饱和才触发

uint8_t max30102_get_proximity_threshold(void);
void    max30102_set_proximity_threshold(uint8_t threshold);

///////////////////////////// temperature

// 读取温度（摄氏度）
float max30102_read_die_temperature_c(void);

////////////////////////////////////////////////////////////////// test

void max30102_test_ppg_mini(void);  // 脉搏
void max30102_test_ppg_full(void);

void max30102_test_die_temperature_c(void);  // 温度

void max30102_test_calc_sample_frequency(void);  // 计算实际采样频率

#endif
