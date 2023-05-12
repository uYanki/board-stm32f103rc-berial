/**
 * @file mpu/registers.h
 * Define registers' for all MPU models.
 */

#ifndef _MPU_REGISTERS_H_
#define _MPU_REGISTERS_H_

#include "config.h"

/*******************************************************************************
 * MPU commom registers for all models
 ******************************************************************************/
#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18
#define SMPLRT_DIV  0x19  // [7:0]
//------------------------------------------------------------------------------
#define CONFIG                     0x1A
#define CONFIG_FIFO_MODE_BIT       6
#define CONFIG_EXT_SYNC_SET_BIT    5  // [5:3]
#define CONFIG_EXT_SYNC_SET_LENGTH 3
#define CONFIG_DLPF_CFG_BIT        2  // [2:0]
#define CONFIG_DLPF_CFG_LENGTH     3
//------------------------------------------------------------------------------
#define GYRO_CONFIG              0x1B
#define GCONFIG_XG_ST_BIT        7
#define GCONFIG_YG_ST_BIT        6
#define GCONFIG_ZG_ST_BIT        5
#define GCONFIG_FS_SEL_BIT       4  // [4:3]
#define GCONFIG_FS_SEL_LENGTH    2
#define GCONFIG_FCHOICE_B        1  // [1:0]
#define GCONFIG_FCHOICE_B_LENGTH 2
//------------------------------------------------------------------------------
#define ACCEL_CONFIG          0x1C
#define ACONFIG_XA_ST_BIT     7
#define ACONFIG_YA_ST_BIT     6
#define ACONFIG_ZA_ST_BIT     5
#define ACONFIG_FS_SEL_BIT    4  // [4:3]
#define ACONFIG_FS_SEL_LENGTH 2
#define ACONFIG_HPF_BIT       2  // [2:0]
#define ACONFIG_HPF_LENGTH    3
//------------------------------------------------------------------------------
#define FF_THR       0x1D
#define FF_DUR       0x1E
#define MOTION_THR   0x1F  // [7:0] // MPU9250_REG_WOM_THR
#define MOTION_DUR   0x20
#define ZRMOTION_THR 0x21
#define ZRMOTION_DUR 0x22
//------------------------------------------------------------------------------
#define FIFO_EN           0x23
#define FIFO_TEMP_EN_BIT  7
#define FIFO_XGYRO_EN_BIT 6
#define FIFO_YGYRO_EN_BIT 5
#define FIFO_ZGYRO_EN_BIT 4
#define FIFO_ACCEL_EN_BIT 3
#define FIFO_SLV_2_EN_BIT 2
#define FIFO_SLV_1_EN_BIT 1
#define FIFO_SLV_0_EN_BIT 0
//------------------------------------------------------------------------------
#define I2C_MST_CTRL                   0x24
#define I2C_MST_CTRL_MULT_EN_BIT       7
#define I2C_MST_CTRL_WAIT_FOR_ES_BIT   6
#define I2C_MST_CTRL_SLV_3_FIFO_EN_BIT 5
#define I2C_MST_CTRL_P_NSR_BIT         4
#define I2C_MST_CTRL_CLOCK_BIT         3  // [3:0]
#define I2C_MST_CTRL_CLOCK_LENGTH      4
//------------------------------------------------------------------------------
#define I2C_SLV0_ADDR     0x25
#define I2C_SLV_RNW_BIT   7  // same for all I2C_SLV registers
#define I2C_SLV_ID_BIT    6  // [6:0]
#define I2C_SLV_ID_LENGTH 7
//------------------------------------------------------------------------------
#define I2C_SLV0_REG 0x26  // [7:0]
//------------------------------------------------------------------------------
#define I2C_SLV0_CTRL       0x27
#define I2C_SLV_EN_BIT      7  // same for all I2C_SLV registers
#define I2C_SLV_BYTE_SW_BIT 6
#define I2C_SLV_REG_DIS_BIT 5
#define I2C_SLV_GRP_BIT     4
#define I2C_SLV_LEN_BIT     3  // [3:0]
#define I2C_SLV_LEN_LENGTH  4
//------------------------------------------------------------------------------
#define I2C_SLV1_ADDR 0x28  // see SLV0 for bit defines
#define I2C_SLV1_REG  0x29
#define I2C_SLV1_CTRL 0x2A
#define I2C_SLV2_ADDR 0x2B  // see SLV0 for bit defines
#define I2C_SLV2_REG  0x2C
#define I2C_SLV2_CTRL 0x2D
#define I2C_SLV3_ADDR 0x2E  // see SLV0 for bit defines
#define I2C_SLV3_REG  0x2F
#define I2C_SLV3_CTRL 0x30
#define I2C_SLV4_ADDR 0x31  // see SLV0 for bit defines
#define I2C_SLV4_REG  0x32
#define I2C_SLV4_DO   0x33  // [7:0]
//------------------------------------------------------------------------------
#define I2C_SLV4_CTRL             0x34
#define I2C_SLV4_EN_BIT           7
#define I2C_SLV4_DONE_INT_BIT     6
#define I2C_SLV4_REG_DIS_BIT      5
#define I2C_SLV4_MST_DELAY_BIT    4  // [4:0]
#define I2C_SLV4_MST_DELAY_LENGTH 5
//------------------------------------------------------------------------------
#define I2C_SLV4_DI 0x35  // [7:0]
//------------------------------------------------------------------------------
#define I2C_MST_STATUS                0x36
#define I2C_MST_STAT_PASS_THROUGH_BIT 7
#define I2C_MST_STAT_SLV4_DONE_BIT    6
#define I2C_MST_STAT_LOST_ARB_BIT     5
#define I2C_MST_STAT_SLV4_NACK_BIT    4
#define I2C_MST_STAT_SLV3_NACK_BIT    3
#define I2C_MST_STAT_SLV2_NACK_BIT    2
#define I2C_MST_STAT_SLV1_NACK_BIT    1
#define I2C_MST_STAT_SLV0_NACK_BIT    0
//------------------------------------------------------------------------------
#define INT_PIN_CONFIG                0x37
#define INT_CFG_LEVEL_BIT             7
#define INT_CFG_OPEN_BIT              6
#define INT_CFG_LATCH_EN_BIT          5
#define INT_CFG_ANYRD_2CLEAR_BIT      4
#define INT_CFG_FSYNC_LEVEL_BIT       3
#define INT_CFG_FSYNC_INT_MODE_EN_BIT 2
#define INT_CFG_I2C_BYPASS_EN_BIT     1
#define INT_CFG_CLOCKOUT_EN_BIT       0
//------------------------------------------------------------------------------
#define INT_ENABLE                   0x38
#define INT_ENABLE_FREEFALL_BIT      7
#define INT_ENABLE_MOTION_BIT        6
#define INT_ENABLE_ZEROMOT_BIT       5
#define INT_ENABLE_FIFO_OFLOW_BIT    4
#define INT_ENABLE_I2C_MST_FSYNC_BIT 3
#define INT_ENABLE_PLL_RDY_BIT       2
#define INT_ENABLE_DMP_RDY_BIT       1
#define INT_ENABLE_RAW_DATA_RDY_BIT  0
//------------------------------------------------------------------------------
#define DMP_INT_STATUS   0x39
#define DMP_INT_STATUS_0 0
#define DMP_INT_STATUS_1 1
#define DMP_INT_STATUS_2 2
#define DMP_INT_STATUS_3 3
#define DMP_INT_STATUS_4 4
#define DMP_INT_STATUS_5 5
//------------------------------------------------------------------------------
#define INT_STATUS                  0x3A
#define INT_STATUS_FREEFALL_BIT     7
#define INT_STATUS_MOTION_BIT       6
#define INT_STATUS_ZEROMOT_BIT      5
#define INT_STATUS_FIFO_OFLOW_BIT   4
#define INT_STATUS_I2C_MST_BIT      3
#define INT_STATUS_PLL_RDY_BIT      2
#define INT_STATUS_DMP_RDY_BIT      1
#define INT_STATUS_RAW_DATA_RDY_BIT 0
//------------------------------------------------------------------------------
#define ACCEL_XOUT_H     0x3B  // [15:0]
#define ACCEL_XOUT_L     0x3C
#define ACCEL_YOUT_H     0x3D  // [15:0]
#define ACCEL_YOUT_L     0x3E
#define ACCEL_ZOUT_H     0x3F  // [15:0]
#define ACCEL_ZOUT_L     0x40
#define TEMP_OUT_H       0x41  // [15:0]
#define TEMP_OUT_L       0x42
#define GYRO_XOUT_H      0x43  // [15:0]
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45  // [15:0]
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47  // [15:0]
#define GYRO_ZOUT_L      0x48
#define EXT_SENS_DATA_00 0x49  // Stores data read from Slave 0, 1, 2, and 3
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define I2C_SLV0_DO      0x63
#define I2C_SLV1_DO      0x64
#define I2C_SLV2_DO      0x65
#define I2C_SLV3_DO      0x66
//------------------------------------------------------------------------------
#define I2C_MST_DELAY_CRTL        0x67
#define I2C_MST_DLY_ES_SHADOW_BIT 7
#define I2C_MST_DLY_SLV4_EN_BIT   4
#define I2C_MST_DLY_SLV3_EN_BIT   3
#define I2C_MST_DLY_SLV2_EN_BIT   2
#define I2C_MST_DLY_SLV1_EN_BIT   1
#define I2C_MST_DLY_SLV0_EN_BIT   0
//------------------------------------------------------------------------------
#define SIGNAL_PATH_RESET   0x68
#define SPATH_GYRO_RST_BIT  2
#define SPATH_ACCEL_RST_BIT 1
#define SPATH_TEMP_RST_BIT  0
//------------------------------------------------------------------------------
#define USER_CTRL                   0x6A
#define USERCTRL_DMP_EN_BIT         7
#define USERCTRL_FIFO_EN_BIT        6
#define USERCTRL_I2C_MST_EN_BIT     5
#define USERCTRL_I2C_IF_DIS_BIT     4
#define USERCTRL_DMP_RESET_BIT      3
#define USERCTRL_FIFO_RESET_BIT     2
#define USERCTRL_I2C_MST_RESET_BIT  1
#define USERCTRL_SIG_COND_RESET_BIT 0
//------------------------------------------------------------------------------
#define PWR_MGMT1             0x6B
#define PWR1_DEVICE_RESET_BIT 7
#define PWR1_SLEEP_BIT        6
#define PWR1_CYCLE_BIT        5
#define PWR1_GYRO_STANDBY_BIT 4
#define PWR1_TEMP_DIS_BIT     3
#define PWR1_CLKSEL_BIT       2
#define PWR1_CLKSEL_LENGTH    3
//------------------------------------------------------------------------------
#define PWR_MGMT2                0x6C
#define PWR2_LP_WAKE_CTRL_BIT    7
#define PWR2_LP_WAKE_CTRL_LENGTH 2
#define PWR2_STBY_XA_BIT         5
#define PWR2_STBY_YA_BIT         4
#define PWR2_STBY_ZA_BIT         3
#define PWR2_STBY_XG_BIT         2
#define PWR2_STBY_YG_BIT         1
#define PWR2_STBY_ZG_BIT         0
#define PWR2_STBY_XYZA_BITS      (1 << PWR2_STBY_XA_BIT | 1 << PWR2_STBY_YA_BIT | 1 << PWR2_STBY_ZA_BIT)
#define PWR2_STBY_XYZG_BITS      (1 << PWR2_STBY_XG_BIT | 1 << PWR2_STBY_YG_BIT | 1 << PWR2_STBY_ZG_BIT)
//------------------------------------------------------------------------------
#define BANK_SEL                  0x6D
#define BANKSEL_PRFTCH_EN_BIT     6
#define BANKSEL_CFG_USER_BANK_BIT 5
#define BANKSEL_MEM_SEL_BIT       4
#define BANKSEL_MEM_SEL_LENGTH    5
//------------------------------------------------------------------------------
#define MEM_START_ADDR 0x6E
#define MEM_R_W        0x6F
#define PRGM_START_H   0x70
#define PRGM_START_L   0x71
#define FIFO_COUNT_H   0x72  // [15:0]
#define FIFO_COUNT_L   0x73
#define FIFO_R_W       0x74
#define WHO_AM_I       0x75

/*******************************************************************************
 * MPU6000, MPU6050 and MPU9150 registers
 ******************************************************************************/
#if defined CONFIG_MPU6050
#define XG_OTP_OFFSET_TC 0x00  // [7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
//------------------------------------------------------------------------------
#define YG_OTP_OFFSET_TC 0x01  // [7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define TC_PWR_MODE_BIT  7     // note: TC  temperature compensation, i think
//------------------------------------------------------------------------------
#define ZG_OTP_OFFSET_TC 0x02  // [7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define X_FINE_GAIN      0x03  // [7:0] X_FINE_GAIN
#define Y_FINE_GAIN      0x04  // [7:0] Y_FINE_GAIN
#define Z_FINE_GAIN      0x05  // [7:0] Z_FINE_GAIN
#define XA_OFFSET_H      0x06  // [15:1] XA_OFFS
#define XA_OFFSET_L      0x07  // note: TC: bit [0]
#define YA_OFFSET_H      0x08  // [15:1] YA_OFFS
#define YA_OFFSET_L      0x09  // note: TC: bit [0]
#define ZA_OFFSET_H      0x0A  // [15:1] ZA_OFFS
#define ZA_OFFSET_L      0x0B  // note: TC: bit [0]
#define SELF_TEST_X      0x0D
#define SELF_TEST_Y      0x0E
#define SELF_TEST_Z      0x0F
#define SELF_TEST_A      0x10
//------------------------------------------------------------------------------
#define MOTION_DETECT_STATUS 0x61
#define MOT_STATUS_X_NEG_BIT 7
#define MOT_STATUS_X_POS_BIT 6
#define MOT_STATUS_Y_NEG_BIT 5
#define MOT_STATUS_Y_POS_BIT 4
#define MOT_STATUS_Z_NEG_BIT 3
#define MOT_STATUS_Z_POS_BIT 2
#define MOT_STATUS_ZRMOT_BIT 0
//------------------------------------------------------------------------------
#define MOTION_DETECT_CTRL            0x69
#define MOTCTRL_ACCEL_ON_DELAY_BIT    5  // [5:4]
#define MOTCTRL_ACCEL_ON_DELAY_LENGTH 2
#define MOTCTRL_FF_COUNT_BIT          3  // [3:2]
#define MOTCTRL_FF_COUNT_LENGTH       2
#define MOTCTRL_MOT_COUNT_BIT         1  // [1:0]
#define MOTCTRL_MOT_COUNT_LENGTH      2
//------------------------------------------------------------------------------
#endif

/*******************************************************************************
 * MPU6500 and MPU9250 registers
 ******************************************************************************/
#if defined CONFIG_MPU6500
#define SELF_TEST_X_GYRO  0x00  // XG_ST_DATA[7:0]
#define SELF_TEST_Y_GYRO  0x01  // YG_ST_DATA[7:0]
#define SELF_TEST_Z_GYRO  0x02  // ZG_ST_DATA[7:0]
#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F
//------------------------------------------------------------------------------
#define ACCEL_CONFIG2                0x1D
#define ACONFIG2_FIFO_SIZE_BIT       7  // [7:6]
#define ACONFIG2_FIFO_SIZE_LENGTH    2
#define ACONFIG2_ACCEL_FCHOICE_B_BIT 3
#define ACONFIG2_A_DLPF_CFG_BIT      2  // [2:0]
#define ACONFIG2_A_DLPF_CFG_LENGTH   3
//------------------------------------------------------------------------------
#define LP_ACCEL_ODR          0x1E
#define LPA_ODR_CLKSEL_BIT    3  // [3:0]
#define LPA_ODR_CLKSEL_LENGTH 4
//------------------------------------------------------------------------------
#define ACCEL_INTEL_CTRL     0x69
#define ACCEL_INTEL_EN_BIT   7
#define ACCEL_INTEL_MODE_BIT 6
//------------------------------------------------------------------------------
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7A
#define YA_OFFSET_L 0x7B
#define ZA_OFFSET_H 0x7D
#define ZA_OFFSET_L 0x7E
#endif

/*******************************************************************************
 * MPU9150 and MPU9250 Magnetometer registers AK89xx
 ******************************************************************************/
#if defined CONFIG_MPU_AK89xx
/*! Magnetometer Registers */
#define WHO_I_AM 0x00
#define INFO     0x01
//------------------------------------------------------------------------------
#define STATUS1              0x02
#define STATUS1_DATA_RDY_BIT 0
//------------------------------------------------------------------------------
#define HXL 0x03
#define HXH 0x04
#define HYL 0x05
#define HYH 0x06
#define HZL 0x07
#define HZH 0x08
//------------------------------------------------------------------------------
#define STATUS2              0x09
#define STATUS2_OVERFLOW_BIT 3
//------------------------------------------------------------------------------
#define CONTROL1             0x0A
#define CONTROL1_MODE_BIT    3
#define CONTROL1_MODE_LENGTH 4
//------------------------------------------------------------------------------
#define ASTC               0x0C
#define ASTC_SELF_TEST_BIT 6
//------------------------------------------------------------------------------
#define TEST1 0x0D
#define TEST2 0x0E
//------------------------------------------------------------------------------
#define I2CDIS               0x0F
#define I2CDIS_DISABLE_VALUE 0x1B
//------------------------------------------------------------------------------
#define ASAX 0x10
#define ASAY 0x11
#define ASAZ 0x12

/*******************************************************************************
 * MPU9150 Magnetometer AK8975
 ******************************************************************************/
#if defined CONFIG_MPU_AK8975
#define STATUS2_DATA_ERROR_BIT 2
#endif

/*******************************************************************************
 * MPU9250 Magnetometer AK8963
 ******************************************************************************/
#if defined CONFIG_MPU_AK8963
#define STATUS1_DATA_OVERRUN_BIT 1
#define STATUS2_BIT_OUTPUT_M_BIT 4
#define CONTROL1_BIT_OUTPUT_BIT  4
//------------------------------------------------------------------------------
#define CONTROL2                0x0B
#define CONTROL2_SOFT_RESET_BIT 0
//------------------------------------------------------------------------------
#endif

#endif  // defined AK89xx

#endif /* end of include guard: _MPU_REGISTERS_HPP_ */
