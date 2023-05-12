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

#include <Wire.h>

#include "MAX30100.h"

MAX30100::MAX30100()
{
}

void MAX30100::begin()
{
    Wire.begin();
    Wire.setClock(I2C_BUS_SPEED);

    setMode(DEFAULT_MODE);
    setLedsPulseWidth(DEFAULT_PULSE_WIDTH);
    setSamplingRate(DEFAULT_SAMPLING_RATE);
    setLedsCurrent(DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
    setHighresModeEnabled(true);
}

void MAX30100::setMode(Mode mode)
{
    writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100::setLedsPulseWidth(LEDPulseWidth ledPulseWidth)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

void MAX30100::setSamplingRate(SamplingRate samplingRate)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

void MAX30100::setLedsCurrent(LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
    writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

void MAX30100::setHighresModeEnabled(bool enabled)
{
    uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
    if (enabled) {
        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
    } else {
        writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
    }
}

void MAX30100::update()
{
    readFifoData();
}

uint8_t MAX30100::readRegister(uint8_t address)
{
    Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    Wire.write(address);
    Wire.endTransmission(false);
    Wire.requestFrom(MAX30100_I2C_ADDRESS, 1);

    return Wire.read();
}

void MAX30100::writeRegister(uint8_t address, uint8_t data)
{
    Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

void MAX30100::burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length)
{
    Wire.beginTransmission(MAX30100_I2C_ADDRESS);
    Wire.write(baseAddress);
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)MAX30100_I2C_ADDRESS, length);

    uint8_t idx = 0;
    while (Wire.available()) {
        buffer[idx++] = Wire.read();
    }
}

void MAX30100::readFifoData()
{
    uint8_t buffer[4];

    burstRead(MAX30100_REG_FIFO_DATA, buffer, 4);

    // Warning: the values are always left-aligned
    rawIRValue = (buffer[0] << 8) | buffer[1];
    rawRedValue = (buffer[2] << 8) | buffer[3];
}
