/* =========================================================================
I2Cbus library is placed under the MIT License
Copyright 2017 Natanael Josue Rabello. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
 ========================================================================= */

#ifndef _I2CBUS_HPP_
#define _I2CBUS_HPP_

#include <stdint.h>
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_err.h"

/* ^^^^^^
 * I2Cbus
 * ^^^^^^ */
namespace i2cbus
{
constexpr uint32_t kDefaultClockSpeed = 100000; /*!< Clock speed in Hz, default: 100KHz */
constexpr uint32_t kDefaultTimeout    = 1000;   /*!< Timeout in milliseconds, default: 1000ms */
class I2C;
}  // namespace i2cbus

// I2Cbus type
using I2C_t = i2cbus::I2C;

// Default Objects
extern i2c_t* bus0; /*!< port: I2C_NUM_0 */
extern i2c_t* bus1; /*!< port: I2C_NUM_1 */

// I2C class definition
namespace i2cbus
{
class I2C {
private:
    i2c_port_t port;        /*!< I2C port: I2C_NUM_0 or I2C_NUM_1 */
    uint32_t   ticksToWait; /*!< Timeout in ticks for read and write */

public:
    explicit I2C(i2c_port_t port);
    ~I2C();

    /** *** I2C Begin ***
     * @brief  Config I2C bus and Install Driver
     * @param  sda_io_num    [GPIO number for SDA line]
     * @param  scl_io_num    [GPIO number for SCL line]
     * @param  sda_pullup_en [Enable internal pullup on SDA line]
     * @param  scl_pullup_en [Enable internal pullup on SCL line]
     * @param  clk_speed     [I2C clock frequency for master mode, (no higher than 1MHz for now), Default 100KHz]
     *                       @see "driver/i2c.h"
     * @return               - ESP_OK   Success
     *                       - ESP_ERR_INVALID_ARG Parameter error
     *                       - ESP_FAIL Driver install error
     */
    esp_err_t begin(gpio_num_t sda_io_num, gpio_num_t scl_io_num, uint32_t clk_speed = kDefaultClockSpeed);

    esp_err_t begin(gpio_num_t sda_io_num, gpio_num_t scl_io_num, gpio_pullup_t sda_pullup_en, gpio_pullup_t scl_pullup_en, uint32_t clk_speed = kDefaultClockSpeed);

    /**
     * Stop I2C bus and unninstall driver
     */
    esp_err_t close();

    /**
     * Timeout read and write in milliseconds
     */
    void setTimeout(uint32_t ms);

    /**
     * *** WRITING interface ***
     * @brief  I2C commands for writing to a 8-bit slave device register.
     *         All of them returns standard esp_err_t codes. So it can be used
     *         with ESP_ERROR_CHECK();
     * @param  devAddr   [I2C slave device register]
     * @param  regAddr   [Register address to write to]
     * @param  bitNum    [Bit position number to write to (bit 7~0)]
     * @param  bitStart  [Start bit number when writing a bit-sequence (MSB)]
     * @param  data      [Value(s) to be write to the register]
     * @param  length    [Number of bytes to write (should be within the data buffer size)]
     *                   [writeBits() -> Number of bits after bitStart (including)]
     * @param  timeout   [Custom timeout for the particular call]
     * @return  - ESP_OK Success
     *          - ESP_ERR_INVALID_ARG Parameter error
     *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
     */
    esp_err_t writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data, int32_t timeout = -1);
    esp_err_t writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data, int32_t timeout = -1);
    esp_err_t writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data, int32_t timeout = -1);
    esp_err_t writeBytes(uint8_t devAddr, uint8_t regAddr, size_t length, const uint8_t* data, int32_t timeout = -1);

    /**
     * *** READING interface ***
     * @breif  I2C commands for reading a 8-bit slave device register.
     *         All of them returns standard esp_err_t codes.So it can be used
     *         with ESP_ERROR_CHECK();
     * @param  devAddr   [I2C slave device register]
     * @param  regAddr   [Register address to read from]
     * @param  bitNum    [Bit position number to write to (bit 7~0)]
     * @param  bitStart  [Start bit number when writing a bit-sequence (MSB)]
     * @param  data      [Buffer to store the read value(s)]
     * @param  length    [Number of bytes to read (should be within the data buffer size)]
     * @param  timeout   [Custom timeout for the particular call]
     * @return  - ESP_OK Success
     *          - ESP_ERR_INVALID_ARG Parameter error
     *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.]
     */
    esp_err_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t* data, int32_t timeout = -1);
    esp_err_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t* data, int32_t timeout = -1);
    esp_err_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t* data, int32_t timeout = -1);
    esp_err_t readBytes(uint8_t devAddr, uint8_t regAddr, size_t length, uint8_t* data, int32_t timeout = -1);

    /**
     * @brief  Quick check to see if a slave device responds.
     * @param  devAddr   [I2C slave device register]
     * @param  timeout   [Custom timeout for the particular call]
     * @return  - ESP_OK Success
     *          - ESP_ERR_INVALID_ARG Parameter error
     *          - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
     *          - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
     *          - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.]
     */
    esp_err_t testConnection(uint8_t devAddr, int32_t timeout = -1);

    /**
     * I2C scanner utility, prints out all device addresses found on this I2C bus.
     */
    void scanner();
};

}  // namespace i2cbus

/* Get default objects */
constexpr I2C_t& getI2C(i2c_port_t port)
{
    return port == 0 ? i2c0 : i2c1;
}

#endif /* end of include guard: _I2CBUS_H_ */
