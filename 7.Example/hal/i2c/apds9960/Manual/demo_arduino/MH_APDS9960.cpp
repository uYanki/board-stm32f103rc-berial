/**************************************************************************/
/*!
    @file     MH_APDS9960.cpp
    @author   Ladyada, Dean Miller (MH Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2017, MH Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
#include <math.h>
#include <stdlib.h>

#include "MH_APDS9960.h"

/*========================================================================*/
/*                          PRIVATE FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    @brief  Implements missing powf function
*/
/**************************************************************************/
float powf(const float x, const float y) {
    return (float)(pow((double)x, (double)y));
}

/**************************************************************************/
/*!
    Enables the device
    Disables the device (putting it in lower power sleep mode)
*/
/**************************************************************************/
void MH_APDS9960::enable(boolean en) {
    _enable.PON = en;
    this->write8(APDS9960_ENABLE, _enable.get());
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    Initializes I2C and configures the sensor (call this function before
    doing anything else)
*/
/**************************************************************************/
boolean MH_APDS9960::begin(uint16_t iTimeMS, apds9960AGain_t aGain, uint8_t addr) {
    _i2c_init();
    _i2caddr = addr;

    /* Make sure we're actually connected */
    uint8_t x = read8(APDS9960_ID);
    if (x != 0xAB) {
        return false;
    }

    /* Set default integration time and gain */
    setADCIntegrationTime(iTimeMS);
    setADCGain(aGain);

    // disable everything to start
    enableGesture(false);
    enableProximity(false);
    enableColor(false);

    disableColorInterrupt();
    disableProximityInterrupt();
    clearInterrupt();

    /* Note: by default, the device is in power down mode on bootup */
    enable(false);
    delay(10);
    enable(true);
    delay(10);

    // default to all gesture dimensions
    setGestureDimensions(APDS9960_DIMENSIONS_ALL);
    setGestureFIFOThreshold(APDS9960_GFIFO_4);
    setGestureGain(APDS9960_GGAIN_4);
    setGestureProximityThreshold(50);
    resetCounts();

    _gpulse.GPLEN  = APDS9960_GPULSE_32US;
    _gpulse.GPULSE = 9;  // 10 pulses
    this->write8(APDS9960_GPULSE, _gpulse.get());

    return true;
}

/**************************************************************************/
/*!
    Sets the integration time for the ADC of the APDS9960, in millis
*/
/**************************************************************************/
void MH_APDS9960::setADCIntegrationTime(uint16_t iTimeMS) {
    float temp;

    // convert ms into 2.78ms increments
    temp = iTimeMS;
    temp /= 2.78;
    temp = 256 - temp;
    if (temp > 255) temp = 255;
    if (temp < 0) temp = 0;

    /* Update the timing register */
    write8(APDS9960_ATIME, (uint8_t)temp);
}

float MH_APDS9960::getADCIntegrationTime(void) {
    float temp;

    temp = read8(APDS9960_ATIME);

    // convert to units of 2.78 ms
    temp = 256 - temp;
    temp *= 2.78;
    return temp;
}

/**************************************************************************/
/*!
    Adjusts the color/ALS gain on the APDS9960 (adjusts the sensitivity to light)
*/
/**************************************************************************/
void MH_APDS9960::setADCGain(apds9960AGain_t aGain) {
    _control.AGAIN = aGain;

    /* Update the timing register */
    write8(APDS9960_CONTROL, _control.get());
}

apds9960AGain_t MH_APDS9960::getADCGain(void) {
    return (apds9960AGain_t)(read8(APDS9960_CONTROL) & 0x03);
}

/**************************************************************************/
/*!
    Adjusts the Proximity gain on the APDS9960
*/
/**************************************************************************/
void MH_APDS9960::setProxGain(apds9960PGain_t pGain) {
    _control.PGAIN = pGain;

    /* Update the timing register */
    write8(APDS9960_CONTROL, _control.get());
}

apds9960PGain_t MH_APDS9960::getProxGain(void) {
    return (apds9960PGain_t)(read8(APDS9960_CONTROL) & 0x0C);
}

void MH_APDS9960::setProxPulse(apds9960PPulseLen_t pLen, uint8_t pulses) {
    if (pulses < 1) pulses = 1;
    if (pulses > 64) pulses = 64;
    pulses--;

    _ppulse.PPLEN  = pLen;
    _ppulse.PPULSE = pulses;

    write8(APDS9960_PPULSE, _ppulse.get());
}

/**************************************************************************/
/*!
    Enable proximity readings on APDS9960
*/
/**************************************************************************/
void MH_APDS9960::enableProximity(boolean en) {
    _enable.PEN = en;

    write8(APDS9960_ENABLE, _enable.get());
}

void MH_APDS9960::enableProximityInterrupt() {
    _enable.PIEN = 1;
    write8(APDS9960_ENABLE, _enable.get());
    clearInterrupt();
}

void MH_APDS9960::disableProximityInterrupt() {
    _enable.PIEN = 0;
    write8(APDS9960_ENABLE, _enable.get());
}

void MH_APDS9960::setProximityInterruptThreshold(uint8_t low, uint8_t high, uint8_t persistance) {
    write8(APDS9960_PILT, low);
    write8(APDS9960_PIHT, high);

    if (persistance > 7) persistance = 7;
    _pers.PPERS = persistance;
    write8(APDS9960_PERS, _pers.get());
}

bool MH_APDS9960::getProximityInterrupt() {
    _status.set(this->read8(APDS9960_STATUS));
    return _status.PINT;
};

/**************************************************************************/
/*!
    Read proximity data
*/
/**************************************************************************/
uint8_t MH_APDS9960::readProximity(void) {
    return read8(APDS9960_PDATA);
}

bool MH_APDS9960::gestureValid() {
    _gstatus.set(this->read8(APDS9960_GSTATUS));
    return _gstatus.GVALID;
}

void MH_APDS9960::setGestureDimensions(uint8_t dims) {
    _gconf3.GDIMS = dims;
    this->write8(APDS9960_GCONF3, _gconf3.get());
}

void MH_APDS9960::setGestureFIFOThreshold(uint8_t thresh) {
    _gconf1.GFIFOTH = thresh;
    this->write8(APDS9960_GCONF1, _gconf1.get());
}

void MH_APDS9960::setGestureGain(uint8_t gain) {
    _gconf2.GGAIN = gain;
    this->write8(APDS9960_GCONF2, _gconf2.get());
}

void MH_APDS9960::setGestureProximityThreshold(uint8_t thresh) {
    this->write8(APDS9960_GPENTH, thresh);
}

void MH_APDS9960::setGestureOffset(uint8_t offset_up, uint8_t offset_down, uint8_t offset_left, uint8_t offset_right) {
    this->write8(APDS9960_GOFFSET_U, offset_up);
    this->write8(APDS9960_GOFFSET_D, offset_down);
    this->write8(APDS9960_GOFFSET_L, offset_left);
    this->write8(APDS9960_GOFFSET_R, offset_right);
}

/**************************************************************************/
/*!
    Enable gesture readings on APDS9960
*/
/**************************************************************************/
void MH_APDS9960::enableGesture(boolean en) {
    if (!en) {
        _gconf4.GMODE = 0;
        write8(APDS9960_GCONF4, _gconf4.get());
    }
    _enable.GEN = en;
    write8(APDS9960_ENABLE, _enable.get());
    resetCounts();
}

void MH_APDS9960::resetCounts() {
    gestCnt = 0;
    UCount  = 0;
    DCount  = 0;
    LCount  = 0;
    RCount  = 0;
}

uint8_t MH_APDS9960::readGesture(void) {
    uint8_t       toRead, bytesRead;
    uint8_t       buf[256];
    unsigned long t;
    uint8_t       gestureReceived;
    while (1) {
        int up_down_diff    = 0;
        int left_right_diff = 0;
        gestureReceived     = 0;
        if (!gestureValid()) return 0;

        delay(30);
        toRead = this->read8(APDS9960_GFLVL);

        bytesRead = this->read(APDS9960_GFIFO_U, buf, toRead);

        if (abs((int)buf[0] - (int)buf[1]) > 13)
            up_down_diff += (int)buf[0] - (int)buf[1];

        if (abs((int)buf[2] - (int)buf[3]) > 13)
            left_right_diff += (int)buf[2] - (int)buf[3];

        if (up_down_diff != 0) {
            if (up_down_diff < 0) {
                if (DCount > 0) {
                    gestureReceived = APDS9960_UP;
                } else
                    UCount++;
            } else if (up_down_diff > 0) {
                if (UCount > 0) {
                    gestureReceived = APDS9960_DOWN;
                } else
                    DCount++;
            }
        }

        if (left_right_diff != 0) {
            if (left_right_diff < 0) {
                if (RCount > 0) {
                    gestureReceived = APDS9960_LEFT;
                } else
                    LCount++;
            } else if (left_right_diff > 0) {
                if (LCount > 0) {
                    gestureReceived = APDS9960_RIGHT;
                } else
                    RCount++;
            }
        }

        if (up_down_diff != 0 || left_right_diff != 0) t = millis();

        if (gestureReceived || millis() - t > 300) {
            resetCounts();
            return gestureReceived;
        }
    }
}

/**************************************************************************/
/*!
    Set LED brightness for proximity/gesture
*/
/**************************************************************************/
void MH_APDS9960::setLED(apds9960LedDrive_t drive, apds9960LedBoost_t boost) {
    // set BOOST
    _config2.LED_BOOST = boost;
    write8(APDS9960_CONFIG2, _config2.get());

    _control.LDRIVE = drive;
    write8(APDS9960_CONTROL, _control.get());
}

/**************************************************************************/
/*!
    Enable proximity readings on APDS9960
*/
/**************************************************************************/
void MH_APDS9960::enableColor(boolean en) {
    _enable.AEN = en;
    write8(APDS9960_ENABLE, _enable.get());
}

bool MH_APDS9960::colorDataReady() {
    _status.set(this->read8(APDS9960_STATUS));
    return _status.AVALID;
}

/**************************************************************************/
/*!
    @brief  Reads the raw red, green, blue and clear channel values
*/
/**************************************************************************/
void MH_APDS9960::getColorData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c) {
    *c = read16R(APDS9960_CDATAL);
    *r = read16R(APDS9960_RDATAL);
    *g = read16R(APDS9960_GDATAL);
    *b = read16R(APDS9960_BDATAL);
}

/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
uint16_t MH_APDS9960::calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b) {
    float X, Y, Z; /* RGB to XYZ correlation      */
    float xc, yc;  /* Chromaticity co-ordinates   */
    float n;       /* McCamy's formula            */
    float cct;

    /* 1. Map RGB values to their XYZ counterparts.    */
    /* Based on 6500K fluorescent, 3000K fluorescent   */
    /* and 60W incandescent values for a wide range.   */
    /* Note: Y = Illuminance or lux                    */
    X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
    Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
    Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

    /* 2. Calculate the chromaticity co-ordinates      */
    xc = (X) / (X + Y + Z);
    yc = (Y) / (X + Y + Z);

    /* 3. Use McCamy's formula to determine the CCT    */
    n = (xc - 0.3320F) / (0.1858F - yc);

    /* Calculate the final CCT */
    cct = (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

    /* Return the results in degrees Kelvin */
    return (uint16_t)cct;
}

/**************************************************************************/
/*!
    @brief  Calculate ambient light values
*/
/**************************************************************************/

uint16_t MH_APDS9960::calculateLux(uint16_t r, uint16_t g, uint16_t b) {
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}

void MH_APDS9960::enableColorInterrupt() {
    _enable.AIEN = 1;
    write8(APDS9960_ENABLE, _enable.get());
}

void MH_APDS9960::disableColorInterrupt() {
    _enable.AIEN = 0;
    write8(APDS9960_ENABLE, _enable.get());
}

void MH_APDS9960::clearInterrupt(void) {
    this->write(APDS9960_AICLEAR, NULL, 0);
}

void MH_APDS9960::setIntLimits(uint16_t low, uint16_t high) {
    write8(APDS9960_AILTIL, low & 0xFF);
    write8(APDS9960_AILTH, low >> 8);
    write8(APDS9960_AIHTL, high & 0xFF);
    write8(APDS9960_AIHTH, high >> 8);
}

void MH_APDS9960::write8(byte reg, byte value) {
    this->write(reg, &value, 1);
}

uint8_t MH_APDS9960::read8(byte reg) {
    uint8_t ret;
    this->read(reg, &ret, 1);

    return ret;
}

uint32_t MH_APDS9960::read32(uint8_t reg) {
    uint8_t ret[4];
    this->read(reg, ret, 4);

    return (ret[0] << 24) | (ret[1] << 16) | (ret[2] << 8) | ret[3];
}

uint16_t MH_APDS9960::read16(uint8_t reg) {
    uint8_t ret[2];
    this->read(reg, ret, 2);

    return (ret[0] << 8) | ret[1];
}

uint16_t MH_APDS9960::read16R(uint8_t reg) {
    uint8_t ret[2];
    this->read(reg, ret, 2);

    return (ret[1] << 8) | ret[0];
}

void MH_APDS9960::_i2c_init() {
    Wire.begin();
}

uint8_t MH_APDS9960::read(uint8_t reg, uint8_t* buf, uint8_t num) {
    uint8_t value;
    uint8_t pos = 0;
    bool    eof = false;

    // on arduino we need to read in 32 byte chunks
    while (pos < num && !eof) {
        uint8_t read_now = min(32, num - pos);
        Wire.beginTransmission((uint8_t)_i2caddr);
        Wire.write((uint8_t)reg + pos);
        Wire.endTransmission();

        Wire.requestFrom((uint8_t)_i2caddr, read_now);

        for (int i = 0; i < read_now; i++) {
            if (!Wire.available()) {
                eof = true;
                break;
            }
            buf[pos] = Wire.read();
            pos++;
        }
    }
    return pos;
}

void MH_APDS9960::write(uint8_t reg, uint8_t* buf, uint8_t num) {
    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t*)buf, num);
    Wire.endTransmission();
}
