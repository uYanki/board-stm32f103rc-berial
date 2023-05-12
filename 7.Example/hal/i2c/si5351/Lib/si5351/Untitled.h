
// https://blog.csdn.net/Haiguozhe/article/details/103601423
// https://blog.csdn.net/qq_43600271/article/details/100554832
// http://www.360doc.com/content/22/0408/15/64556539_1025435474.shtml

#ifndef __SI5351_H__
#define __SI5351_H__

#include "i2cdev/i2cdev.h"

#define SI5351_ADDRESS_LOW                      0x60  // <---
#define SI5351_ADDRESS_HIGH                     0x61  // <---

#define SI5351_DEV                              (SI5351_ADDRESS_LOW << 1)

#define SI5351_I2C                              hi2c1  // <---

////////////////////////////////////////////////////////////////////////////////////////

#define si5351_read_uint8(reg)                  i2c_get_uint8(&SI5351_I2C, SI5351_DEV, reg, 0x00)
#define si5351_write_uint8(reg, dat)            i2c_write_uint8(&SI5351_I2C, SI5351_DEV, reg, dat)

#define si5351_read_bytes(reg, dat, len)        i2c_read(&SI5351_I2C, SI5351_DEV, reg, dat, len)
#define si5351_write_bytes(reg, dat, len)       i2c_write(&SI5351_I2C, SI5351_DEV, reg, dat, len)

#define si5351_write_regs(buff, len)            i2c_writes(&SI5351_I2C, SI5351_DEV, buff, len)

#define si5351_read_bit(reg, idx)               i2c_get_bit(&SI5351_I2C, SI5351_DEV, reg, idx, 0x00)
#define si5351_write_bit(reg, idx, dat)         i2c_write_bit(&SI5351_I2C, SI5351_DEV, reg, idx, dat)

#define si5351_read_bits(reg, start, len)       i2c_get_bits(&SI5351_I2C, SI5351_DEV, reg, start, len, 0x00)
#define si5351_write_bits(reg, start, len, dat) i2c_write_bits(&SI5351_I2C, SI5351_DEV, reg, start, len, dat)

#define si5351_read_mask(reg, mask)             i2c_get_mask(&SI5351_I2C, SI5351_DEV, reg, mask, 0x00)
#define si5351_write_mask(reg, mask, dat)       i2c_write_mask(&SI5351_I2C, SI5351_DEV, reg, mask, dat)

////////////////////////////////////////////////////////////////////////////////////////

#define SI5351_PLL_A_MIN                        15
#define SI5351_PLL_A_MAX                        90
#define SI5351_PLL_B_MIN                        0
#define SI5351_PLL_B_MAX                        1048575
#define SI5351_PLL_C_MIN                        1
#define SI5351_PLL_C_MAX                        1048575  // 0xFFFFF
#define SI5351_MS_A_MIN                         6
#define SI5351_MS_A_MAX                         1800

// 26-41,93-164

////////////////////////////////////////////

// 注：SI5351C 有中断脚 INTR, 低电平有效，可通过配置 mask 位屏蔽掉。

// bit[7]: SYS_INIT, system initialization Status: 0 已就绪、1 正在初始化
// bit[6]: LOL_B, PLL B loss of lock status. ( SI5351A/C only ) PLLB 有来自 CLKIN / XTAL 的有效参考时钟时就会上锁。0: locked, 1:unlocked.
// bit[5]: LOL_A, PLL A loss of lock status. PLLA 有来自 CLKIN / XTAL 的有效参考时钟时就会上锁。0: locked ( PLLA 工作正常), 1:unlocked.
// bit[4]: LOS, CLKIN loss of signal.  (SI5351C Only) 参考时钟是否未能满足最低要求的有效输入信号。0：CLKIN 引脚处的时钟信号有效, 1：CLKIN 引脚处的时钟信号无效

typedef enum {
    SI5351_STATUS_SYS_INIT    = 7 << 1,  // system initialization, 0:初始化完成, 1:正在初始化。
    SI5351_STATUS_LOCK_PLL_B  = 6 << 1,  // PLLB lock status. 0:已上锁, 1:未上锁。
    SI5351_STATUS_LOCK_PLL_A  = 5 << 1,  // PLLA lock status. 0:正常运行, 1:未上锁。
    SI5351_STATUS_LOSS_CLK_IN = 4 << 1,  // 0:信号正常, 1: 信号无效。si5351c only
} si5351_device_status_t;

typedef si5351_device_status_t si5351_interrupt_status_t;
typedef si5351_device_status_t si5351_interrupt_mask_t;

#define SI5351_REG_DEVICE_STATUS 0  // device status (R). 设备状态
#define SI5351_REG_INT_STATUS    1  // interrupt status sticky (R/W). 中断状态, 0: 未触发, 1: 已触发 (需写0以进行清除)
#define SI5351_REG_INT_MASK      2  // interrupt status mask (R/W). 用于屏蔽中断位, 防止触发中断引脚,  0:不屏蔽, 1:屏蔽。

uint8_t si5351_get_revision_id(void);
uint8_t si5351_get_device_status(si5351_device_status_t type);
uint8_t si5351_get_interrupt_status(si5351_interrupt_status_t type);

////////////////////////////////////////////

#define SI5351_REG_CLK_EN 3  // output enable control (R/W), 时钟输出使能, 0:enable,1:disable.
#define SI5351_REG_OEB_EN 9  // OEB pin enable control (R/W), OEB 引脚可控制时钟使能状态,  0:enable,1:disable.

typedef enum {
    SI5351_CLKOUT_0 = 0,
    SI5351_CLKOUT_1 = 1,
    SI5351_CLKOUT_2 = 2,
    SI5351_CLKOUT_3 = 3,
    SI5351_CLKOUT_4 = 4,
    SI5351_CLKOUT_5 = 5,
    SI5351_CLKOUT_6 = 6,
    SI5351_CLKOUT_7 = 7,
} si5351_clkout_t;

void si5351_set_clkout(si5351_clkout_t clk, uint8_t enable);
void si5351_set_pinctrl(si5351_clkout_t clk, uint8_t enable);

void si5351_set_all_clkout(uint8_t enable);
void si5351_set_all_pinctrl(uint8_t enable);

////////////////////////////////////////////

#define SI5351_REG_PLL_SRC 3  // PLL input source (R/W)

typedef enum {
    SI5351_PLL_B_SRC = 1 << 3,  // 0:XTAL (SI5351A/C), 1:CLKIN (SI5351C)
    SI5351_PLL_A_SRC = 1 << 2,  // 0:XTAL, 1:CLKIN (SI5351C)
} si5351_pll_src_t;

////////////////////////////////////////////

// clock control, clk 0,1,2,3,4,5,6,7
static const SI5351_REG_CLK_CTRL[] = {16, 17, 18, 19, 20, 21, 22, 23};

// pwer down (pdn) bit[7] 允许输出未使用时关闭 CLK 输出驱动器以节省功率
typedef enum {
    SI5351_CLK_POWER_UP   = 0,
    SI5351_CLK_POWER_DOWM = 1,
} si5351_clk_power_t;

// multisynth integer mode (ms_int) bit[6]
// 强制MS进入整数模式, 以提高抖动性能。若指定了延迟偏移, 需配置为分数模式。
typedef enum {
    // clk 0,1,2,3,4,5
    SI5351_MS_MODE_FRAC = 0,  // fractional division mode 分数
    SI5351_MS_MODE_INT  = 1,  // integer mode 整数
} si5351_ms_mode_t;

// multisynth source (ms_src) bit[5]
typedef enum {
    SI5351_MS_SRC_PLL_A = 0,  // PLLA
    SI5351_MS_SRC_PLL_B = 1,  // PLLB (SI5351A/C) or VCXO (SI5351B)
} si5351_ms_src_t;

// output clock invert (clk_inv) bit[4]
typedef enum {
    SI5351_CLK_INVERT_DISABLE = 0,  // not-invert
    SI5351_CLK_INVERT_ENABLE  = 1,
} si5351_clk_invert_t;

// clock source for clk (clk_src) bit[3:2]
typedef enum {
    SI5351_CLK_SRC_XTAL  = 0,
    SI5351_CLK_SRC_CLKIN = 1,
    SI5351_CLK_SRC_MS0   = 3,
} si5351_clk_src_t;

// rise and fall time / drive strength control. (clk_idrv) bit[1:0]
// 上升沿和下降沿时间 / 驱动强度
typedef enum {
    SI5351_CLK_IDRV_2MA = 0,
    SI5351_CLK_IDRV_4MA = 1,
    SI5351_CLK_IDRV_6MA = 2,
    SI5351_CLK_IDRV_8MA = 3,
} si5351_clk_idrv_t;

////////////////////////////////////////////

// clock state when disable 禁用时的时钟输出状态

static const SI5351_REG_CLK_DIS_STATE[] = {24, 25};

typedef enum {
    // clk 3,2,1,0: <<3,<<2,<<1,<<0
    // clk 7,6,5,4: <<3,<<2,<<1,<<0
    SI5351_CLK_DIS_STATE_LOW            = 0,
    SI5351_CLK_DIS_STATE_HIGH           = 1,
    SI5351_CLK_DIS_STATE_HIGH_IMPEDANCE = 2,
    SI5351_CLK_DIS_STATE_NEVER          = 3,
} si5351_clk_disable_state_t;

////////////////////////////////////////////

// multi synth: 0,1,2,3,4,5,6,7 (P/W)
static const SI5351_REG_MS[] = {42, 50, 58, 66, 74, 82, 90, 91};

// output divider (r_div), 输出时钟的分频系数
typedef enum {
    SI5351_CLKOUT_DIV_1   = 0,
    SI5351_CLKOUT_DIV_2   = 1,
    SI5351_CLKOUT_DIV_4   = 2,
    SI5351_CLKOUT_DIV_8   = 3,
    SI5351_CLKOUT_DIV_16  = 4,
    SI5351_CLKOUT_DIV_32  = 5,
    SI5351_CLKOUT_DIV_64  = 6,
    SI5351_CLKOUT_DIV_128 = 7,
} si5351_clkout_div_t;

/**
 * multisynth parameters
 *
 * si5351_ms_mode?
 * ms0,ms1,ms2,ms3,ms4,ms5:
 * - p1(18bit):整数部分
 * - p2(20bit):分数分母部分
 * - p3(20bit):分数小数部分
 *
 * ms6,ms7:
 * - p1(8bit):分频比(≥6)
 */

void si5351_set_multisynth(si5351_clkout_t clock, uint32_t multisynth, si5351_clkout_div_t divider)
{
}

////////////////////////////////////////////

#define SI5351_REG_PLL_RST 177

// 复位锁相环 PLL (phase locked loop)
typedef enum {
    SI5351_PLL_RST_A = 5,
    SI5351_PLL_RST_B = 7,
} si5351_pll_rst_t;

void si5351_pll_rst(si5351_pll_rst_t pll) { si5351_write_bit(SI5351_REG_PLL_RST, (uint8_t)pll, 1); }

////////////////////////////////////////////

#define SI5351_REG_CRYSTAL_LOAD 183  // XTAL_CL

// 输出时钟的负载电容 crystal load capacitance
typedef enum {  // bit[7,6]
    SI5351_CRYSTAL_LOAD_6_PF  = 0,
    SI5351_CRYSTAL_LOAD_8_PF  = 1,
    SI5351_CRYSTAL_LOAD_10_PF = 2,
} si5351_crystal_load_t;

////////////////////////////////////////////

// clock phase offset, clk 0,1,2,3,4,5
static const SI5351_REG_CLK_PHOFF[] = {165, 166, 167, 168, 189, 170};

////////////////////////////////////////////////////////////////////////////////////////

void si5351_calc_param(uint8_t buffer[8], uint8_t integer, uint32_t numerator, uint32_t denominator)
{
    /**
     * fVCO: PLL output (600..900MHz), fVCO = fXTAL * (a+(b/c))
     *  - fXTAL = crystal input frequency 晶振频率
     *  - a (18-bit) = integer multiplier (15..90) 整数
     *  - b (20-bit) = fractional numerator (0..1,048,575) 小数分子
     *  - c (20-bit) = fractional denominator (1..1,048,575) 小数分母
     * fOUT = fVCO / MSx
     * - a (18-bit) = integer mode (4,6,8) 整数模式, fractional mode (8...900) 小数模式
     * - b (20-bit) = fractional numerator (0..1,048,575) 小数分子
     * - c (20-bit) = fractional denominator (1..1,048,575) 小数分母
     * Feedback Multisynth Divider Equation:
     *  - P1[17:0] = 128 * a + floor(128 * ( b / c )) - 512
     *  - P2[19:0] = 128 * b - c * floor(128 * ( b / c ))
     *  - P3[19:0] = c
     */

    uint32_t p1, p2, p3;

    if (integer < SI5351_PLL_A_MIN) {
        integer = SI5351_PLL_A_MIN;
    } else if (integer > SI5351_PLL_A_MAX) {
        integer = SI5351_PLL_A_MAX;
    }

    if (numerator > SI5351_PLL_B_MAX) {  // 分子
        numerator = SI5351_PLL_B_MAX;
    }

    if (denominator < SI5351_PLL_C_MIN) {  // 分母
        denominator = SI5351_PLL_C_MIN;
    } else if (denominator > SI5351_PLL_C_MAX) {
        denominator = SI5351_PLL_C_MAX;
    }

    if (numerator == 0) {
        // integer mode
        p1 = (integer - 4) << 7;
        p2 = 0;
        p3 = denominator;
    } else {
        // fractional mode
        p1 = (numerator << 7) / (float)denominator;
        p2 = (numerator << 7) - denominator * p1;
        p1 += (integer - 4) << 7;
        p3 = denominator;
    }

    buffer[0] = (p3 >> 8) & 0xFF;
    buffer[1] = (p3 & 0xFF);
    buffer[2] = (p1 >> 16) & 0x03;
    buffer[3] = (p1 >> 8) & 0xFF;
    buffer[4] = (p1 & 0xFF);
    buffer[5] = ((p3 >> 12) & 0xF0) | ((p2 >> 16) & 0x0F);
    buffer[6] = (p2 >> 8) & 0xFF;
    buffer[7] = (p2 & 0xFF);
}

static const uint8_t SI5351_REG_PLL[2] = {26, 34};

typedef enum {
    SI5351_PLL_A = 0,
    SI5351_PLL_B = 1,
} si5351_pll_t;

/*!
    @brief set multiplier for pll

    NOTE: try to use integers whenever possible to avoid clock jitter 尽量使用整数以避免时钟抖动
    (only use the a part, setting b to '0' and c to '1').

    See: AN619.pdf
*/
void si5351_set_pll(si5351_pll_t pll, uint8_t integer, uint32_t numerator, uint32_t denominator)
{
    // set the main PLL config registers

    uint8_t buffer[8];
    si5351_set_param(buffer, integer, numerator, denominator);
    si5351_write_bytes(SI5351_REG_PLL[pll], buffer, 8);

    // reset both PLLs

    si5351_write_uint8(SI5351_REG_PLL_RST, (1 << SI5351_PLL_RST_A) | (1 << SI5351_PLL_RST_B));

    // calc pll freq

    // float    fvco     = SI5351_XTAL * (integer + ((float)numerator / (float)denominator));
    // uint32_t pll_freq = floor(fvco);
}

void si5351_set_pll_int(si5351_pll_t pll, uint8_t integer)
{
    si5351_set_pll(pll, integer, 0, 1);
}

typedef enum {
    SI5351_MULTISYNTH_DIV_4 = 4,
    SI5351_MULTISYNTH_DIV_6 = 6,
    SI5351_MULTISYNTH_DIV_8 = 8
} si5351_multisynth_div_t;

void si5351_set_multisynth(si5351_clkout_t         clk,
                           si5351_pll_t            pll,
                           si5351_multisynth_div_t ms_div,
                           si5351_clkout_div_t     clk_div,
                           uint32_t                numerator,
                           uint32_t                denominator)
{
    switch (clk) {
        case SI5351_CLKOUT_0:
        case SI5351_CLKOUT_1:
        case SI5351_CLKOUT_2:
        case SI5351_CLKOUT_3:
        case SI5351_CLKOUT_4:
        case SI5351_CLKOUT_5:
            uint8_t buffer[8];
            si5351_set_param(buffer, ms_div, numerator, denominator);
            buffer[2] |= ms_div << 4;
            si5351_write_bytes(SI5351_REG_MS[clk], buffer, 8);
            break;
        case SI5351_CLKOUT_6:  // todo
        case SI5351_CLKOUT_7:
            // si5351_write_uint8(SI5351_REG_MS[clk], );
            switch (clk) {
                case SI5351_CLKOUT_6:
                    si5351_write_bits(92, 0, 3, clk_div);
                    break;
                case SI5351_CLKOUT_7:
                    si5351_write_bits(92, 4, 3, clk_div);
                    break;
            }
        default:
            break;
    }

    uint8_t byte = 0x00;

    byte |= SI5351_CLK_IDRV_8MA << 0;

    byte |= SI5351_CLK_SRC_MS0 << 2;

    byte |= SI5351_CLK_INVERT_DISABLE << 4;

    switch (pll) {
        default:
        case SI5351_PLL_A:
            byte |= SI5351_MS_SRC_PLL_A << 5;
            break;
        case SI5351_PLL_B:
            byte |= SI5351_MS_SRC_PLL_B << 5;
            break;
    }

    if (numerator == 0) {
        byte |= SI5351_MS_MODE_INT << 6;
    } else {
        byte |= SI5351_MS_MODE_FRAC << 6;
    }

    byte |= SI5351_CLK_POWER_DOWM << 7;

    si5351_write_uint8(SI5351_REG_CLK_CTRL[clk], byte);
}

#define SI5351_XTAL_HZ       25000000ULL   // 输入时钟频率
#define SI5351_PLL_MIN_HZ    600000000ULL  // 锁相环最小时钟
#define SI5351_PLL_MAX_HZ    900000000ULL  // 锁相环最大时钟
#define SI5351_CLKOUT_MIN_HZ 8000ULL       // 输出的最小频率
#define SI5351_CLKOUT_MAX_HZ 150000000ULL  // 输出的最大频率

void si5351_set_freq(si5351_clkout_t clk, uint64_t freq, uint8_t pll_rst)
{
    // 输出时钟分频系数

    si5351_clkout_div_t clk_div;

    if (freq < 8000ULL) {  // < 8kHz
        clk_div = SI5351_CLKOUT_DIV_128;
    } else if (freq < 16000ULL) {  // 8kHz ~ 16kHz
        clk_div = SI5351_CLKOUT_DIV_64;
    } else if (freq < 32000ULL) {  // 16kHz ~ 32kHz
        clk_div = SI5351_CLKOUT_DIV_32;
    } else if (freq < 64000ULL) {  // 32kHz ~ 64kHz
        clk_div = SI5351_CLKOUT_DIV_16;
    } else if (freq < 128000ULL) {  // 64kHz ~ 128kHz
        clk_div = SI5351_CLKOUT_DIV_8;
    } else if (freq < 256000ULL) {  // 128kHz ~ 256kHz
        clk_div = SI5351_CLKOUT_DIV_4;
    } else if (freq < 512000ULL) {  // 256kHz ~ 512kHz
        clk_div = SI5351_CLKOUT_DIV_2;
    } else {  // > 512kHz
        clk_div = SI5351_CLKOUT_DIV_1;
    }

    freq <<= (uint8_t)clk_div;

    // 频率校验

    if (freq < SI5351_CLKOUT_MIN_HZ) {
        freq = SI5351_CLKOUT_MIN_HZ;
    } else if (freq > SI5351_CLKOUT_MAX_HZ) {
        freq = SI5351_CLKOUT_MAX_HZ;
    }

    // 锁相环分频数 Fvco  (锁相环频率 900mhz)

    uint32_t pll_div = 900000000ULL / freq;

    // 限制分频数偶数

    if (pll_div & 1) --pll_div;

    // 边界校验

    if (pll_div < SI5351_MS_A_MIN) {
        pll_div = SI5351_MS_A_MIN;
    } else if (pll_div > SI5351_MS_A_MAX) {
        pll_div = SI5351_MS_A_MAX;
    }

    // 重新计算 PLL

    uint64_t pll_freq = pll_div * freq;

    uint32_t a        = pll_freq / SI5351_XTAL_HZ,
             b        = (pll_freq % SI5351_XTAL_HZ) * SI5351_PLL_C_MAX / SI5351_XTAL_HZ,
             c        = SI5351_PLL_C_MAX;

    if (b > SI5351_PLL_B_MAX) {
        b -= SI5351_PLL_C_MAX;
        ++a;
    }

    // 校验边界

    if (a < SI5351_PLL_A_MIN) {
        a = SI5351_PLL_A_MIN;
    } else if (a > SI5351_PLL_A_MAX) {
        a = SI5351_PLL_A_MAX;
    }

    // 配置 PLL

    si5351_set_msn(SI5351_PLL_A, a, b, c);
    si5351_set_ms(clk, pll_div, clk_div);

    if (pll_rst) si5351_pll_rst(SI5351_PLL_RST_A);  // 复位 PLL

    si5351_set_clkout(clk, 1);  // 使能通道
}

// void si5351_init(si5351_crystal_load_t cap);

extern const uint8_t map_15to92_149to170[100][2];

void si5351_set_clock_builder_data(uint8_t map[100][2]);

void si5351_init()
{
}

void si5351_reset(void)
{
    // disable all

    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_0], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_1], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_2], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_3], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_4], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_5], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_6], 0x80);
    si5351_write_uint8(SI5351_REG_CLK_CTRL[SI5351_CLKOUT_7], 0x80);
}

#endif

#include "si5351.h"

uint8_t si5351_get_revision_id(void)
{
    return si5351_read_bits(SI5351_REG_DEVICE_STATUS, 0, 2);
}

uint8_t si5351_get_device_status(si5351_device_status_t type) { return si5351_read_mask(SI5351_REG_DEVICE_STATUS, (uint8_t)type); }

uint8_t si5351_get_interrupt_status(si5351_interrupt_status_t type) { return si5351_read_mask(SI5351_REG_INT_STATUS, (uint8_t)type); }

void si5351_set_clkout(si5351_clkout_t clk, uint8_t enable) { si5351_write_bit(SI5351_REG_CLK_EN, (uint8_t)clk, enable ? 0 : 1); }

void si5351_set_pinctrl(si5351_clkout_t clk, uint8_t enable) { si5351_write_bit(SI5351_REG_OEB_EN, (uint8_t)clk, enable ? 0 : 1); }

void si5351_set_all_clkout(uint8_t enable) { si5351_write_uint8(SI5351_REG_CLK_EN, enable ? 0x00 : 0xFF); }

void si5351_set_all_pinctrl(uint8_t enable) { si5351_write_uint8(SI5351_REG_OEB_EN, enable ? 0x00 : 0xFF); }

void si5351_init(si5351_crystal_load_t cap)
{
    // disable all output clocks

    si5351_write_uint8(SI5351_REG_CLK_EN, 0xFF);

    // power down all output drivers

    si5351_write_bit(SI5351_REG_CLK_CTRL[0], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[1], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[2], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[3], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[4], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[5], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[6], 7, 1);
    si5351_write_bit(SI5351_REG_CLK_CTRL[8], 7, 1);

    // set the load capacitance for the XTAL

    si5351_write_bits(SI5351_REG_CRYSTAL_LOAD, 6, 2, (uint8_t)cap);
}

/* Test setup from SI5351 ClockBuilder
 * -----------------------------------
 * XTAL:      25     MHz
 * Channel 0: 120.00 MHz
 * Channel 1: 12.00  MHz
 * Channel 2: 13.56  MHz
 */
const uint8_t map_15to92_149to170[100][2] = {
    { 15, 0x00}, /* Input source = crystal for PLLA and PLLB */
    { 16, 0x4F}, /* CLK0 Control: 8mA drive, Multisynth 0 as CLK0 source, Clock not inverted, Source = PLLA, Multisynth 0 in integer mode, clock powered up */
    { 17, 0x4F}, /* CLK1 Control: 8mA drive, Multisynth 1 as CLK1 source, Clock not inverted, Source = PLLA, Multisynth 1 in integer mode, clock powered up */
    { 18, 0x6F}, /* CLK2 Control: 8mA drive, Multisynth 2 as CLK2 source, Clock not inverted, Source = PLLB, Multisynth 2 in integer mode, clock powered up */
    { 19, 0x80}, /* CLK3 Control: Not used ... clock powered down */
    { 20, 0x80}, /* CLK4 Control: Not used ... clock powered down */
    { 21, 0x80}, /* CLK5 Control: Not used ... clock powered down */
    { 22, 0x80}, /* CLK6 Control: Not used ... clock powered down */
    { 23, 0x80}, /* CLK7 Control: Not used ... clock powered down */
    { 24, 0x00}, /* Clock disable state 0..3 (low when disabled) */
    { 25, 0x00}, /* Clock disable state 4..7 (low when disabled) */
  /* PLL_A Setup */
    { 26, 0x00},
    { 27, 0x05},
    { 28, 0x00},
    { 29, 0x0C},
    { 30, 0x66},
    { 31, 0x00},
    { 32, 0x00},
    { 33, 0x02},
 /* PLL_B Setup */
    { 34, 0x02},
    { 35, 0x71},
    { 36, 0x00},
    { 37, 0x0C},
    { 38, 0x1A},
    { 39, 0x00},
    { 40, 0x00},
    { 41, 0x86},
 /* Multisynth Setup */
    { 42, 0x00},
    { 43, 0x01},
    { 44, 0x00},
    { 45, 0x01},
    { 46, 0x00},
    { 47, 0x00},
    { 48, 0x00},
    { 49, 0x00},
    { 50, 0x00},
    { 51, 0x01},
    { 52, 0x00},
    { 53, 0x1C},
    { 54, 0x00},
    { 55, 0x00},
    { 56, 0x00},
    { 57, 0x00},
    { 58, 0x00},
    { 59, 0x01},
    { 60, 0x00},
    { 61, 0x18},
    { 62, 0x00},
    { 63, 0x00},
    { 64, 0x00},
    { 65, 0x00},
    { 66, 0x00},
    { 67, 0x00},
    { 68, 0x00},
    { 69, 0x00},
    { 70, 0x00},
    { 71, 0x00},
    { 72, 0x00},
    { 73, 0x00},
    { 74, 0x00},
    { 75, 0x00},
    { 76, 0x00},
    { 77, 0x00},
    { 78, 0x00},
    { 79, 0x00},
    { 80, 0x00},
    { 81, 0x00},
    { 82, 0x00},
    { 83, 0x00},
    { 84, 0x00},
    { 85, 0x00},
    { 86, 0x00},
    { 87, 0x00},
    { 88, 0x00},
    { 89, 0x00},
    { 90, 0x00},
    { 91, 0x00},
    { 92, 0x00},
 /* Misc Config Register */
    {149, 0x00},
    {150, 0x00},
    {151, 0x00},
    {152, 0x00},
    {153, 0x00},
    {154, 0x00},
    {155, 0x00},
    {156, 0x00},
    {157, 0x00},
    {158, 0x00},
    {159, 0x00},
    {160, 0x00},
    {161, 0x00},
    {162, 0x00},
    {163, 0x00},
    {164, 0x00},
    {165, 0x00},
    {166, 0x00},
    {167, 0x00},
    {168, 0x00},
    {169, 0x00},
    {170, 0x00}
};

void si5351_set_clock_builder_data(uint8_t map[100][2])
{
    // disable all output
    si5351_set_all_clkout(0);

    // writes configuration data to device using the register map contents generated by ClockBuilder Desktop (registers 15-92 + 149-170)
    si5351_write_regs((uint8_t*)map, 100);

    // reset pll
    si5351_write_uint8(SI5351_REG_PLL_RST, (1 << SI5351_PLL_RST_A) | (1 << SI5351_PLL_RST_B));

    // Enabled desired outputs
    si5351_set_all_clkout(1);
}