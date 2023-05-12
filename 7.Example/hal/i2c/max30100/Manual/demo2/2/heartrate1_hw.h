/****************************************************************************
* Title                 :   Heart rate click Hardware Layer
* Filename              :   heartrate1_hw.h
* Author                :   Viktor Milovanovic
* Origin Date           :   31/05/2016
* Notes                 :   None
*****************************************************************************/
/**************************CHANGE LIST **************************************
*
*    Date    Software Version    Initials    	Description
*  31/05/2016         1.0          VM         Interface Created
*
*****************************************************************************/
/** @file heartrate1_hw.h
 *  @brief Hardware layer for working with the Heart Rate Click
 *
 *  @date 31 May 2016
 *  @author Viktor Milovanovic
 *  @copyright MikroelEktronika
 *
 *  @version .1 - Initial testing and verification
 *
 *  @note Test configuration:
 *   MCU:             STM32F107VC
 *   Dev.Board:       EasyMx Pro v7
 *   Oscillator:      72 Mhz internal
 *   Ext. Modules:    Heartrate1_Click
 *   SW:              ARM 4.5.1
 *
 */


#include <stdint.h>
#include <stdbool.h>
#include "heartrate1_hal.h"

#define INT_STATUS       0x00
#define INT_ENABLE       0x01
#define FIFO_WRITE_PTR   0x02
#define OVER_FLOW_CNT    0x03
#define FIFO_READ_PTR    0x04
#define FIFO_DATA_REG    0x05
#define MODE_CONFIG      0x06
#define SPO2_CONFIG      0x07
#define LED_CONFIG       0x09
#define TEMP_INTEGER     0x16
#define TEMP_FRACTION    0x17

/**
 * @brief hr_shut_down
 * Enables/disables power saving mode.  While in power-save mode, all registers retain their values, and write/read operations function as normal. 
 * All interrupts are cleared to zero in this mode.
 * @param enable
 * True - enables shut down, false - disables
 */
void hr_shut_down(bool enable);
/**
 * @brief hr_reset
 * All configuration, threshold, and data registers are reset to their power-on-state.
 */
void hr_reset();
/**
 * @brief hr_temp_enable
 * Initiates a single temperature reading from the temperature sensor. This bit is
 * cleared automatically back to zero at the conclusion of the temperature reading when the bit is set to one in heart rate or SpO2 mode.
 * @param enable
 * True - enables the reading, false - disables
 */
void hr_temp_enable(bool enable);
/**
 * @brief hr_set_mode
 * Sets the operating state of the MAX30100
 * @param mode
 * Desired mode as stated in the datasheet (https://datasheets.maximintegrated.com/en/ds/MAX30100.pdf) page 15, table 3. "Mode Control"
 * @return
 * -1 if failed, 0 if successful
 */
int8_t hr_set_mode(uint8_t mode);
/**
 * @brief hr_spo2_high_res_enable
 * Sets the SpO2 ADC resolution to 16-bit with 1.6ms LED pulse width.
 * @param enable
 * True - enables SpO2 high resolution, False - disables
 */
void hr_spo2_high_res_enable(bool enable);
/**
 * @brief hr_spo2_set_sample_rate
 * Sets the operating state of the MAX30100
 * @param sample_rate
 * Desired sample rate as stated in the datasheet (https://datasheets.maximintegrated.com/en/ds/MAX30100.pdf) page 16, table 4.
 * @return
 * -1 if failed, 0 if successful
 */
int8_t hr_spo2_set_sample_rate(uint8_t sample_rate);
/**
 * @brief hr_set_led_pulse_width
 * Sets the LED pulse width (the IR and RED have the same pulse width)
 * @param _value
 * Desired pulse width as stated in the datasheet (https://datasheets.maximintegrated.com/en/ds/MAX30100.pdf) page 17, table 5.
 * @return
 * -1 if failed, 0 if successful
 */
int8_t hr_set_led_pulse_width(uint8_t _value);
/**
 * @brief hr_set_red_current_level
 * Sets the current level of the Red LED.
 * @param level
 * Desired current level as stated in the datasheet (https://datasheets.maximintegrated.com/en/ds/MAX30100.pdf) page 17, table 6.
 * @return
 * -1 if failed, 0 if successful
 */
int8_t hr_set_red_current_level(uint8_t level);
/**
 * @brief hr_set_ir_current_level
 * Sets the current level of the IR LED.
 * @param level
 * Desired current level as stated in the datasheet (https://datasheets.maximintegrated.com/en/ds/MAX30100.pdf) page 17, table 6.
 * @return
 * -1 if failed, 0 if successful
 */
int8_t hr_set_ir_current_level(uint8_t level);
/**
 * @brief hr_fifo_almost_full_int_enable
 * Enables/disables the FIFO almost full interrupt
 * @param enable
 * True - interrupt enabled, false - interrupt disabled
 */
void hr_fifo_almost_full_int_enable(bool enable);
/**
 * @brief hr_temp_ready_int_enable
 * Enables/disables the temperature data ready interrupt
 * @param enable
 * True - interrupt enabled, false - interrupt disabled
 */
void hr_temp_ready_int_enable(bool enable);
/**
 * @brief hr_heartrate_data_ready_int_enable
 * Enables/disables heart rate data ready interrupt
 * @param enable
 * True - interrupt enabled, false - interrupt disabled
 */
void hr_heartrate_data_ready_int_enable(bool enable);
/**
 * @brief hr_heartrate_spo2_data_ready_int_enable
 * Enables/disables the SpO2 data ready
 * @param enable
 * True - interrupt enabled, false - interrupt disabled
 */
void hr_heartrate_spo2_data_ready_int_enable(bool enable);
/**
 * @brief hr_read_diodes
 * Reads the ADC values of the IR and RED LEDs from the FIFO buffer, and stores them into the given buffers.
 * @param ir_buffer
 * Buffer to which to store the IR LED values
 * @param red_buffer
 * Buffer to which to store the RED LED values
 * @return
 * Returns the number of samples
 */
uint8_t hr_read_diodes(uint16_t* ir_buffer, uint16_t* red_buffer);
/**
 * @brief hr_get_status
 * Returns the value read from the interrupt status register. The value shows which interrupts have been trigerred
 * @return
 * Returns the status value
 */
uint8_t hr_get_status();
/**
 * @brief hr_init
 * Initializes the i2c communication, activates the LEDs, enables heartrate data ready, SpO2 data ready.
 */
void hr_init( uint8_t slave_address );
