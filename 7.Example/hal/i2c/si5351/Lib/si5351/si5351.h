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

/* Define definitions */

#define SI5351_BUS_BASE_ADDR                    0x60
#define SI5351_XTAL_FREQ                        25000000
#define SI5351_PLL_FIXED                        80000000000ULL
// #define SI5351_FREQ_MULT     100ULL
#define SI5351_DEFAULT_CLK                      1000000000ULL

#define SI5351_PLL_VCO_MIN                      600000000
#define SI5351_PLL_VCO_MAX                      900000000
#define SI5351_MULTISYNTH_MIN_FREQ              500000
#define SI5351_MULTISYNTH_DIVBY4_FREQ           150000000
#define SI5351_MULTISYNTH_MAX_FREQ              225000000
#define SI5351_MULTISYNTH_SHARE_MAX             100000000
#define SI5351_MULTISYNTH_SHARE_MIN             1024000
#define SI5351_MULTISYNTH67_MAX_FREQ            SI5351_MULTISYNTH_DIVBY4_FREQ
#define SI5351_CLKOUT_MIN_FREQ                  4000
#define SI5351_CLKOUT_MAX_FREQ                  SI5351_MULTISYNTH_MAX_FREQ
#define SI5351_CLKOUT67_MS_MIN                  SI5351_PLL_VCO_MIN / SI5351_MULTISYNTH67_A_MAX
#define SI5351_CLKOUT67_MIN_FREQ                SI5351_CLKOUT67_MS_MIN / 128
#define SI5351_CLKOUT67_MAX_FREQ                SI5351_MULTISYNTH67_MAX_FREQ

#define SI5351_PLL_A_MIN                        15
#define SI5351_PLL_A_MAX                        90
#define SI5351_PLL_B_MAX                        (SI5351_PLL_C_MAX - 1)
#define SI5351_PLL_C_MAX                        1048575
#define SI5351_MULTISYNTH_A_MIN                 6
#define SI5351_MULTISYNTH_A_MAX                 1800
#define SI5351_MULTISYNTH67_A_MAX               254
#define SI5351_MULTISYNTH_B_MAX                 (SI5351_MULTISYNTH_C_MAX - 1)
#define SI5351_MULTISYNTH_C_MAX                 1048575
#define SI5351_MULTISYNTH_P1_MAX                ((1 << 18) - 1)
#define SI5351_MULTISYNTH_P2_MAX                ((1 << 20) - 1)
#define SI5351_MULTISYNTH_P3_MAX                ((1 << 20) - 1)
#define SI5351_VCXO_PULL_MIN                    30
#define SI5351_VCXO_PULL_MAX                    240
#define SI5351_VCXO_MARGIN                      103

#define SI5351_DEVICE_STATUS                    0
#define SI5351_INTERRUPT_STATUS                 1
#define SI5351_INTERRUPT_MASK                   2
#define SI5351_STATUS_SYS_INIT                  (1 << 7)
#define SI5351_STATUS_LOL_B                     (1 << 6)
#define SI5351_STATUS_LOL_A                     (1 << 5)
#define SI5351_STATUS_LOS                       (1 << 4)
#define SI5351_OUTPUT_ENABLE_CTRL               3
#define SI5351_OEB_PIN_ENABLE_CTRL              9
#define SI5351_PLL_INPUT_SOURCE                 15
#define SI5351_CLKIN_DIV_MASK                   (3 << 6)
#define SI5351_CLKIN_DIV_1                      (0 << 6)
#define SI5351_CLKIN_DIV_2                      (1 << 6)
#define SI5351_CLKIN_DIV_4                      (2 << 6)
#define SI5351_CLKIN_DIV_8                      (3 << 6)
#define SI5351_PLLB_SOURCE                      (1 << 3)
#define SI5351_PLLA_SOURCE                      (1 << 2)

static SI5351_REG_CLK_CTRL = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7}

#define SI5351_CLK0_CTRL                16
#define SI5351_CLK1_CTRL                17
#define SI5351_CLK2_CTRL                18
#define SI5351_CLK3_CTRL                19
#define SI5351_CLK4_CTRL                20
#define SI5351_CLK5_CTRL                21
#define SI5351_CLK6_CTRL                22
#define SI5351_CLK7_CTRL                23
#define SI5351_CLK_POWERDOWN            (1 << 7)
#define SI5351_CLK_INTEGER_MODE         (1 << 6)
#define SI5351_CLK_PLL_SELECT           (1 << 5)
#define SI5351_CLK_INVERT               (1 << 4)
#define SI5351_CLK_INPUT_MASK           (3 << 2)
#define SI5351_CLK_INPUT_XTAL           (0 << 2)
#define SI5351_CLK_INPUT_CLKIN          (1 << 2)
#define SI5351_CLK_INPUT_MULTISYNTH_0_4 (2 << 2)
#define SI5351_CLK_INPUT_MULTISYNTH_N   (3 << 2)
#define SI5351_CLK_DRIVE_STRENGTH_MASK  (3 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_2MA   (0 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_4MA   (1 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_6MA   (2 << 0)
#define SI5351_CLK_DRIVE_STRENGTH_8MA   (3 << 0)

#define SI5351_CLK3_0_DISABLE_STATE     24
#define SI5351_CLK7_4_DISABLE_STATE     25
#define SI5351_CLK_DISABLE_STATE_MASK   3
#define SI5351_CLK_DISABLE_STATE_LOW    0
#define SI5351_CLK_DISABLE_STATE_HIGH   1
#define SI5351_CLK_DISABLE_STATE_FLOAT  2
#define SI5351_CLK_DISABLE_STATE_NEVER  3

#define SI5351_PARAMETERS_LENGTH        8
#define SI5351_PLLA_PARAMETERS          26
#define SI5351_PLLB_PARAMETERS          34
#define SI5351_CLK0_PARAMETERS          42
#define SI5351_CLK1_PARAMETERS          50
#define SI5351_CLK2_PARAMETERS          58
#define SI5351_CLK3_PARAMETERS          66
#define SI5351_CLK4_PARAMETERS          74
#define SI5351_CLK5_PARAMETERS          82
#define SI5351_CLK6_PARAMETERS          90
#define SI5351_CLK7_PARAMETERS          91
#define SI5351_CLK6_7_OUTPUT_DIVIDER    92
#define SI5351_OUTPUT_CLK_DIV_MASK      (7 << 4)
#define SI5351_OUTPUT_CLK6_DIV_MASK     (7 << 0)
#define SI5351_OUTPUT_CLK_DIV_SHIFT     4
#define SI5351_OUTPUT_CLK_DIV6_SHIFT    0
#define SI5351_OUTPUT_CLK_DIV_1         0
#define SI5351_OUTPUT_CLK_DIV_2         1
#define SI5351_OUTPUT_CLK_DIV_4         2
#define SI5351_OUTPUT_CLK_DIV_8         3
#define SI5351_OUTPUT_CLK_DIV_16        4
#define SI5351_OUTPUT_CLK_DIV_32        5
#define SI5351_OUTPUT_CLK_DIV_64        6
#define SI5351_OUTPUT_CLK_DIV_128       7
#define SI5351_OUTPUT_CLK_DIVBY4        (3 << 2)

#define SI5351_SSC_PARAM0               149
#define SI5351_SSC_PARAM1               150
#define SI5351_SSC_PARAM2               151
#define SI5351_SSC_PARAM3               152
#define SI5351_SSC_PARAM4               153
#define SI5351_SSC_PARAM5               154
#define SI5351_SSC_PARAM6               155
#define SI5351_SSC_PARAM7               156
#define SI5351_SSC_PARAM8               157
#define SI5351_SSC_PARAM9               158
#define SI5351_SSC_PARAM10              159
#define SI5351_SSC_PARAM11              160
#define SI5351_SSC_PARAM12              161

#define SI5351_VXCO_PARAMETERS_LOW      162
#define SI5351_VXCO_PARAMETERS_MID      163
#define SI5351_VXCO_PARAMETERS_HIGH     164

#define SI5351_CLK0_PHASE_OFFSET        165
#define SI5351_CLK1_PHASE_OFFSET        166
#define SI5351_CLK2_PHASE_OFFSET        167
#define SI5351_CLK3_PHASE_OFFSET        168
#define SI5351_CLK4_PHASE_OFFSET        169
#define SI5351_CLK5_PHASE_OFFSET        170

#define SI5351_PLL_RESET                177
#define SI5351_PLL_RESET_B              (1 << 7)
#define SI5351_PLL_RESET_A              (1 << 5)

#define SI5351_CRYSTAL_LOAD             183
#define SI5351_CRYSTAL_LOAD_MASK        (3 << 6)
#define SI5351_CRYSTAL_LOAD_0PF         (0 << 6)
#define SI5351_CRYSTAL_LOAD_6PF         (1 << 6)
#define SI5351_CRYSTAL_LOAD_8PF         (2 << 6)
#define SI5351_CRYSTAL_LOAD_10PF        (3 << 6)

#define SI5351_FANOUT_ENABLE            187
#define SI5351_CLKIN_ENABLE             (1 << 7)
#define SI5351_XTAL_ENABLE              (1 << 6)
#define SI5351_MULTISYNTH_ENABLE        (1 << 4)

/* Macro definitions */

// #define RFRAC_DENOM ((1L << 20) - 1)
#define RFRAC_DENOM                     1000000ULL

/*
 * Based on former asm-ppc/div64.h and asm-m68knommu/div64.h
 *
 * The semantics of do_div() are:
 *
 * uint32_t do_div(uint64_t *n, uint32_t base)
 * {
 *      uint32_t remainder = *n % base;
 *      *n = *n / base;
 *      return remainder;
 * }
 *
 * NOTE: macro parameter n is evaluated multiple times,
 *       beware of side effects!
 */

#define do_div(n, base)                 ({ \
    uint64_t __base = (base);              \
    uint64_t __rem;                        \
    __rem = ((uint64_t)(n)) % __base;      \
    (n)   = ((uint64_t)(n)) / __base;      \
    __rem;                                 \
})

/* Enum definitions */

/*
 * enum si5351_variant - SiLabs Si5351 chip variant
 * @SI5351_VARIANT_A: Si5351A (8 output clocks, XTAL input)
 * @SI5351_VARIANT_A3: Si5351A MSOP10 (3 output clocks, XTAL input)
 * @SI5351_VARIANT_B: Si5351B (8 output clocks, XTAL/VXCO input)
 * @SI5351_VARIANT_C: Si5351C (8 output clocks, XTAL/CLKIN input)
 */
/*
enum si5351_variant {
    SI5351_VARIANT_A = 1,
    SI5351_VARIANT_A3 = 2,
    SI5351_VARIANT_B = 3,
    SI5351_VARIANT_C = 4,
};
*/

enum si5351_clock {
    SI5351_CLK0,
    SI5351_CLK1,
    SI5351_CLK2,
    SI5351_CLK3,
    SI5351_CLK4,
    SI5351_CLK5,
    SI5351_CLK6,
    SI5351_CLK7
};

enum si5351_pll {
    SI5351_PLLA,
    SI5351_PLLB
};

enum si5351_drive {
    SI5351_DRIVE_2MA = 0,
    SI5351_DRIVE_4MA = 1,
    SI5351_DRIVE_6MA = 2,
    SI5351_DRIVE_8MA = 3
};

enum si5351_clock_source {
    SI5351_CLK_SRC_XTAL,
    SI5351_CLK_SRC_CLKIN,
    SI5351_CLK_SRC_MS0,
    SI5351_CLK_SRC_MS
};

enum si5351_clock_disable {
    SI5351_CLK_DISABLE_LOW,
    SI5351_CLK_DISABLE_HIGH,
    SI5351_CLK_DISABLE_HI_Z,
    SI5351_CLK_DISABLE_NEVER
};

enum si5351_clock_fanout {
    SI5351_FANOUT_CLKIN,
    SI5351_FANOUT_XO,
    SI5351_FANOUT_MS
};

enum si5351_pll_input {
    SI5351_PLL_INPUT_XO,
    SI5351_PLL_INPUT_CLKIN
};

typedef enum {
    SI5351_MS_MODE_FRAC = 0,  // fractional division mode 分数
    SI5351_MS_MODE_INT  = 1,  // integer mode 整数
} si5351_ms_mode_t;

/* Struct definitions */

struct Si5351RegSet {
    uint32_t p1;
    uint32_t p2;
    uint32_t p3;
};

struct Si5351Status {
    uint8_t SYS_INIT;
    uint8_t LOL_B;
    uint8_t LOL_A;
    uint8_t LOS;
    uint8_t REVID;
};

struct Si5351IntStatus {
    uint8_t SYS_INIT_STKY;
    uint8_t LOL_B_STKY;
    uint8_t LOL_A_STKY;
    uint8_t LOS_STKY;
};

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

// Si5351(uint8_t i2c_addr = SI5351_BUS_BASE_ADDR);

bool                   init(uint8_t, uint32_t, int32_t);
void                   reset(void);
uint8_t                set_freq(uint64_t, enum si5351_clock);
uint8_t                set_freq_manual(uint64_t, uint64_t, enum si5351_clock);
void                   set_pll(uint64_t, enum si5351_pll);
void                   set_ms(enum si5351_clock, struct Si5351RegSet, uint8_t, uint8_t, uint8_t);
void                   output_enable(enum si5351_clock, uint8_t);
void                   drive_strength(enum si5351_clock, enum si5351_drive);
void                   update_status(void);
void                   set_correction(int32_t, enum si5351_pll_input);
void                   set_phase(enum si5351_clock, uint8_t);
int32_t                get_correction(enum si5351_pll_input);
void                   pll_reset(enum si5351_pll);
void                   set_ms_source(enum si5351_clock, enum si5351_pll);
void                   set_int(enum si5351_clock, si5351_ms_mode_t);
void                   set_clock_pwr(enum si5351_clock, uint8_t);
void                   set_clock_invert(enum si5351_clock, uint8_t);
void                   set_clock_source(enum si5351_clock, enum si5351_clock_source);
void                   set_clock_disable(enum si5351_clock, enum si5351_clock_disable);
void                   set_clock_fanout(enum si5351_clock_fanout, uint8_t);
void                   set_pll_input(enum si5351_pll, enum si5351_pll_input);
void                   set_vcxo(uint64_t, uint8_t);
void                   set_ref_freq(uint32_t, enum si5351_pll_input);
uint8_t                si5351_write_bulk(uint8_t, uint8_t, uint8_t*);
uint8_t                si5351_write(uint8_t, uint8_t);
uint8_t                si5351_read(uint8_t);
struct Si5351Status    dev_status     = {.SYS_INIT = 0, .LOL_B = 0, .LOL_A = 0, .LOS = 0, .REVID = 0};
struct Si5351IntStatus dev_int_status = {.SYS_INIT_STKY = 0, .LOL_B_STKY = 0, .LOL_A_STKY = 0, .LOS_STKY = 0};
enum si5351_pll        pll_assignment[8];
uint64_t               clk_freq[8];
uint64_t               plla_freq;
uint64_t               pllb_freq;
enum si5351_pll_input  plla_ref_osc;
enum si5351_pll_input  pllb_ref_osc;
uint32_t               xtal_freq[2];

uint64_t pll_calc(enum si5351_pll, uint64_t, struct Si5351RegSet*, int32_t, uint8_t);
uint64_t multisynth_calc(uint64_t, uint64_t, struct Si5351RegSet*);
uint64_t multisynth67_calc(uint64_t, uint64_t, struct Si5351RegSet*);
void     update_sys_status(struct Si5351Status*);
void     update_int_status(struct Si5351IntStatus*);
void     ms_div(enum si5351_clock, uint8_t, uint8_t);
uint8_t  select_r_div(uint64_t*);
uint8_t  select_r_div_ms67(uint64_t*);
int32_t  ref_correction[2];
uint8_t  clkin_div;
uint8_t  i2c_bus_addr;
bool     clk_first_set[8];

#endif