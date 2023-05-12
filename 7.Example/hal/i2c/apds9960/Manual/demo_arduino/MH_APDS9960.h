/**************************************************************************/
/*!
    @file     MH_APDS9960.h
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
#ifndef _APDS9960_H_
#define _APDS9960_H_

#include <Arduino.h>
#include <Wire.h>

#define I2CDEBUG

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define APDS9960_ADDRESS (0x39)
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/

enum {
    APDS9960_RAM        = 0x00,
    APDS9960_ENABLE     = 0x80,
    APDS9960_ATIME      = 0x81,
    APDS9960_WTIME      = 0x83,
    APDS9960_AILTIL     = 0x84,
    APDS9960_AILTH      = 0x85,
    APDS9960_AIHTL      = 0x86,
    APDS9960_AIHTH      = 0x87,
    APDS9960_PILT       = 0x89,
    APDS9960_PIHT       = 0x8B,
    APDS9960_PERS       = 0x8C,
    APDS9960_CONFIG1    = 0x8D,
    APDS9960_PPULSE     = 0x8E,
    APDS9960_CONTROL    = 0x8F,
    APDS9960_CONFIG2    = 0x90,
    APDS9960_ID         = 0x92,
    APDS9960_STATUS     = 0x93,
    APDS9960_CDATAL     = 0x94,
    APDS9960_CDATAH     = 0x95,
    APDS9960_RDATAL     = 0x96,
    APDS9960_RDATAH     = 0x97,
    APDS9960_GDATAL     = 0x98,
    APDS9960_GDATAH     = 0x99,
    APDS9960_BDATAL     = 0x9A,
    APDS9960_BDATAH     = 0x9B,
    APDS9960_PDATA      = 0x9C,
    APDS9960_POFFSET_UR = 0x9D,
    APDS9960_POFFSET_DL = 0x9E,
    APDS9960_CONFIG3    = 0x9F,
    APDS9960_GPENTH     = 0xA0,
    APDS9960_GEXTH      = 0xA1,
    APDS9960_GCONF1     = 0xA2,
    APDS9960_GCONF2     = 0xA3,
    APDS9960_GOFFSET_U  = 0xA4,
    APDS9960_GOFFSET_D  = 0xA5,
    APDS9960_GOFFSET_L  = 0xA7,
    APDS9960_GOFFSET_R  = 0xA9,
    APDS9960_GPULSE     = 0xA6,
    APDS9960_GCONF3     = 0xAA,
    APDS9960_GCONF4     = 0xAB,
    APDS9960_GFLVL      = 0xAE,
    APDS9960_GSTATUS    = 0xAF,
    APDS9960_IFORCE     = 0xE4,
    APDS9960_PICLEAR    = 0xE5,
    APDS9960_CICLEAR    = 0xE6,
    APDS9960_AICLEAR    = 0xE7,
    APDS9960_GFIFO_U    = 0xFC,
    APDS9960_GFIFO_D    = 0xFD,
    APDS9960_GFIFO_L    = 0xFE,
    APDS9960_GFIFO_R    = 0xFF,
};

/*=========================================================================*/

typedef enum {
    APDS9960_AGAIN_1X  = 0x00, /**<  No gain  */
    APDS9960_AGAIN_4X  = 0x01, /**<  2x gain  */
    APDS9960_AGAIN_16X = 0x02, /**<  16x gain */
    APDS9960_AGAIN_64X = 0x03  /**<  64x gain */
} apds9960AGain_t;

typedef enum {
    APDS9960_PGAIN_1X = 0x00, /**<  1x gain  */
    APDS9960_PGAIN_2X = 0x04, /**<  2x gain  */
    APDS9960_PGAIN_4X = 0x08, /**<  4x gain  */
    APDS9960_PGAIN_8X = 0x0C  /**<  8x gain  */
} apds9960PGain_t;

typedef enum {
    APDS9960_PPULSELEN_4US  = 0x00, /**<  4uS   */
    APDS9960_PPULSELEN_8US  = 0x40, /**<  8uS   */
    APDS9960_PPULSELEN_16US = 0x80, /**<  16uS  */
    APDS9960_PPULSELEN_32US = 0xC0  /**<  32uS  */
} apds9960PPulseLen_t;

typedef enum {
    APDS9960_LEDDRIVE_100MA = 0x00, /**<  100mA   */
    APDS9960_LEDDRIVE_50MA  = 0x40, /**<  50mA    */
    APDS9960_LEDDRIVE_25MA  = 0x80, /**<  25mA    */
    APDS9960_LEDDRIVE_12MA  = 0xC0  /**<  12.5mA  */
} apds9960LedDrive_t;

typedef enum {
    APDS9960_LEDBOOST_100PCNT = 0x00, /**<  100%   */
    APDS9960_LEDBOOST_150PCNT = 0x10, /**<  150%   */
    APDS9960_LEDBOOST_200PCNT = 0x20, /**<  200%   */
    APDS9960_LEDBOOST_300PCNT = 0x30  /**<  300%   */
} apds9960LedBoost_t;

enum {
    APDS9960_DIMENSIONS_ALL        = 0x00,
    APDS9960_DIMENSIONS_UP_DOWM    = 0x01,
    APGS9960_DIMENSIONS_LEFT_RIGHT = 0x02,
};

enum {
    APDS9960_GFIFO_1  = 0x00,
    APDS9960_GFIFO_4  = 0x01,
    APDS9960_GFIFO_8  = 0x02,
    APDS9960_GFIFO_16 = 0x03,
};

enum {
    APDS9960_GGAIN_1 = 0x00,
    APDS9960_GGAIN_2 = 0x01,
    APDS9960_GGAIN_4 = 0x02,
    APDS9960_GGAIN_8 = 0x03,
};

enum {
    APDS9960_GPULSE_4US  = 0x00,
    APDS9960_GPULSE_8US  = 0x01,
    APDS9960_GPULSE_16US = 0x02,
    APDS9960_GPULSE_32US = 0x03,
};

#define APDS9960_TIME_MULT 2.78  // millisec

#define APDS9960_UP 0x01
#define APDS9960_DOWN 0x02
#define APDS9960_LEFT 0x03
#define APDS9960_RIGHT 0x04

class MH_APDS9960 {
public:
    MH_APDS9960(void){};
    ~MH_APDS9960(void){};

    boolean         begin(uint16_t iTimeMS = 10, apds9960AGain_t = APDS9960_AGAIN_4X, uint8_t addr = APDS9960_ADDRESS);
    void            setADCIntegrationTime(uint16_t iTimeMS);
    float           getADCIntegrationTime(void);
    void            setADCGain(apds9960AGain_t gain);
    apds9960AGain_t getADCGain(void);
    void            setLED(apds9960LedDrive_t drive, apds9960LedBoost_t boost);

    // proximity
    void            enableProximity(boolean en = true);
    void            setProxGain(apds9960PGain_t gain);
    apds9960PGain_t getProxGain(void);
    void            setProxPulse(apds9960PPulseLen_t pLen, uint8_t pulses);
    void            enableProximityInterrupt();
    void            disableProximityInterrupt();
    uint8_t         readProximity(void);
    void            setProximityInterruptThreshold(uint8_t low, uint8_t high, uint8_t persistance = 4);
    bool            getProximityInterrupt();

    // gesture
    void    enableGesture(boolean en = true);
    bool    gestureValid();
    void    setGestureDimensions(uint8_t dims);
    void    setGestureFIFOThreshold(uint8_t thresh);
    void    setGestureGain(uint8_t gain);
    void    setGestureProximityThreshold(uint8_t thresh);
    void    setGestureOffset(uint8_t offset_up, uint8_t offset_down, uint8_t offset_left, uint8_t offset_right);
    uint8_t readGesture(void);
    void    resetCounts();

    // light & color
    void     enableColor(boolean en = true);
    bool     colorDataReady();
    void     getColorData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c);
    uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
    uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b);
    void     enableColorInterrupt();
    void     disableColorInterrupt();
    void     clearInterrupt(void);
    void     setIntLimits(uint16_t l, uint16_t h);

    // turn on/off elements
    void enable(boolean en = true);

private:
    uint8_t _i2caddr;

    uint32_t read32(uint8_t reg);
    uint16_t read16(uint8_t reg);
    uint16_t read16R(uint8_t reg);

    void    write8(byte reg, byte value);
    uint8_t read8(byte reg);

    uint8_t gestCnt;

    uint8_t UCount;
    uint8_t DCount;

    uint8_t LCount;
    uint8_t RCount;

    uint8_t read(uint8_t reg, uint8_t* buf, uint8_t num);
    void    write(uint8_t reg, uint8_t* buf, uint8_t num);
    void    _i2c_init();

    struct enable {
        // power on
        uint8_t PON : 1;

        // ALS enable
        uint8_t AEN : 1;

        // Proximity detect enable
        uint8_t PEN : 1;

        // wait timer enable
        uint8_t WEN : 1;

        // ALS interrupt enable
        uint8_t AIEN : 1;

        // proximity interrupt enable
        uint8_t PIEN : 1;

        // gesture enable
        uint8_t GEN : 1;

        uint8_t get() {
            return (GEN << 6) | (PIEN << 5) | (AIEN << 4) | (WEN << 3) | (PEN << 2) | (AEN << 1) | PON;
        };
    };
    struct enable _enable;

    struct pers {
        // ALS Interrupt Persistence. Controls rate of Clear channel interrupt to the host processor
        uint8_t APERS : 4;

        // proximity interrupt persistence, controls rate of prox interrupt to host processor
        uint8_t PPERS : 4;

        uint8_t get() { return (PPERS << 4) | APERS; };
    };
    pers _pers;

    struct config1 {
        uint8_t WLONG : 1;

        uint8_t get() {
            return WLONG << 1;
        };
    };
    config1 _config1;

    struct ppulse {
        /*Proximity Pulse Count. Specifies the number of proximity pulses to be generated on LDR.
        Number of pulses is set by PPULSE value plus 1.
        */
        uint8_t PPULSE : 6;

        // Proximity Pulse Length. Sets the LED-ON pulse width during a proximity LDR pulse.
        uint8_t PPLEN : 2;

        uint8_t get() { return (PPLEN << 6) | PPULSE; }
    };
    ppulse _ppulse;

    struct control {
        // ALS and Color gain control
        uint8_t AGAIN : 2;

        // proximity gain control
        uint8_t PGAIN : 2;

        // led drive strength
        uint8_t LDRIVE : 2;

        uint8_t get() {
            return (LDRIVE << 6) | (PGAIN << 2) | AGAIN;
        }
    };
    control _control;

    struct config2 {
        /* Additional LDR current during proximity and gesture LED pulses. Current value, set by LDRIVE,
        is increased by the percentage of LED_BOOST.
        */
        uint8_t LED_BOOST : 2;

        // clear photodiode saturation int enable
        uint8_t CPSIEN : 1;

        // proximity saturation interrupt enable
        uint8_t PSIEN : 1;

        uint8_t get() {
            return (PSIEN << 7) | (CPSIEN << 6) | (LED_BOOST << 4) | 1;
        }
    };
    config2 _config2;

    struct status {
        /* ALS Valid. Indicates that an ALS cycle has completed since AEN was asserted or since a read
        from any of the ALS/Color data registers.
        */
        uint8_t AVALID : 1;

        /* Proximity Valid. Indicates that a proximity cycle has completed since PEN was asserted or since
        PDATA was last read. A read of PDATA automatically clears PVALID.
        */
        uint8_t PVALID : 1;

        /* Gesture Interrupt. GINT is asserted when GFVLV becomes greater than GFIFOTH or if GVALID
        has become asserted when GMODE transitioned to zero. The bit is reset when FIFO is
        completely emptied (read).
        */
        uint8_t GINT : 1;

        // ALS Interrupt. This bit triggers an interrupt if AIEN in ENABLE is set.
        uint8_t AINT : 1;

        // Proximity Interrupt. This bit triggers an interrupt if PIEN in ENABLE is set.
        uint8_t PINT : 1;

        /* Indicates that an analog saturation event occurred during a previous proximity or gesture
        cycle. Once set, this bit remains set until cleared by clear proximity interrupt special function
        command (0xE5 PICLEAR) or by disabling Prox (PEN=0). This bit triggers an interrupt if PSIEN
        is set.
        */
        uint8_t PGSAT : 1;

        /* Clear Photodiode Saturation. When asserted, the analog sensor was at the upper end of its
        dynamic range. The bit can be de-asserted by sending a Clear channel interrupt command
        (0xE6 CICLEAR) or by disabling the ADC (AEN=0). This bit triggers an interrupt if CPSIEN is set.
        */
        uint8_t CPSAT : 1;

        void set(uint8_t data) {
            AVALID = data & 0x01;
            PVALID = (data >> 1) & 0x01;
            GINT   = (data >> 2) & 0x01;
            AINT   = (data >> 4) & 0x01;
            PINT   = (data >> 5) & 0x01;
            PGSAT  = (data >> 6) & 0x01;
            CPSAT  = (data >> 7) & 0x01;
        }
    };
    status _status;

    struct config3 {
        // proximity mask
        uint8_t PMASK_R : 1;
        uint8_t PMASK_L : 1;
        uint8_t PMASK_D : 1;
        uint8_t PMASK_U : 1;

        /* Sleep After Interrupt. When enabled, the device will automatically enter low power mode
        when the INT pin is asserted and the state machine has progressed to the SAI decision block.
        Normal operation is resumed when INT pin is cleared over I2C.
        */
        uint8_t SAI : 1;

        /* Proximity Gain Compensation Enable. This bit provides gain compensation when proximity
        photodiode signal is reduced as a result of sensor masking. If only one diode of the diode pair
        is contributing, then only half of the signal is available at the ADC; this results in a maximum
        ADC value of 127. Enabling PCMP enables an additional gain of 2X, resulting in a maximum
        ADC value of 255.
        */
        uint8_t PCMP : 1;

        uint8_t get() {
            return (PCMP << 5) | (SAI << 4) | (PMASK_U << 3) | (PMASK_D << 2) | (PMASK_L << 1) | PMASK_R;
        }
    };
    config3 _config3;

    struct gconf1 {
        /* Gesture Exit Persistence. When a number of consecutive �gesture end� occurrences become
        equal or greater to the GEPERS value, the Gesture state machine is exited.
        */
        uint8_t GEXPERS : 2;

        /* Gesture Exit Mask. Controls which of the gesture detector photodiodes (UDLR) will be included
        to determine a �gesture end� and subsequent exit of the gesture state machine. Unmasked
        UDLR data will be compared with the value in GTHR_OUT. Field value bits correspond to UDLR
        detectors.
        */
        uint8_t GEXMSK : 4;

        /* Gesture FIFO Threshold. This value is compared with the FIFO Level (i.e. the number of UDLR
        datasets) to generate an interrupt (if enabled).
        */
        uint8_t GFIFOTH : 2;

        uint8_t get() {
            return (GFIFOTH << 6) | (GEXMSK << 2) | GEXPERS;
        }
    };
    gconf1 _gconf1;

    struct gconf2 {
        /* Gesture Wait Time. The GWTIME controls the amount of time in a low power mode between
        gesture detection cycles.
        */
        uint8_t GWTIME : 3;

        // Gesture LED Drive Strength. Sets LED Drive Strength in gesture mode.
        uint8_t GLDRIVE : 2;

        // Gesture Gain Control. Sets the gain of the proximity receiver in gesture mode.
        uint8_t GGAIN : 2;

        uint8_t get() {
            return (GGAIN << 5) | (GLDRIVE << 3) | GWTIME;
        }
    };
    gconf2 _gconf2;

    struct gpulse {
        /* Number of Gesture Pulses. Specifies the number of pulses to be generated on LDR.
        Number of pulses is set by GPULSE value plus 1.
        */
        uint8_t GPULSE : 6;

        // Gesture Pulse Length. Sets the LED_ON pulse width during a Gesture LDR Pulse.
        uint8_t GPLEN : 2;

        uint8_t get() {
            return (GPLEN << 6) | GPULSE;
        }
    };
    gpulse _gpulse;

    struct gconf3 {
        /* Gesture Dimension Select. Selects which gesture photodiode pairs are enabled to gather
        results during gesture.
        */
        uint8_t GDIMS : 2;

        uint8_t get() {
            return GDIMS;
        }
    };
    gconf3 _gconf3;

    struct gconf4 {
        /* Gesture Mode. Reading this bit reports if the gesture state machine is actively running, 1
        = Gesture, 0= ALS, Proximity, Color. Writing a 1 to this bit causes immediate entry in to the
        gesture state machine (as if GPENTH had been exceeded). Writing a 0 to this bit causes exit of
        gesture when current analog conversion has finished (as if GEXTH had been exceeded).
        */
        uint8_t GMODE : 1;

        /* Gesture interrupt enable. Gesture Interrupt Enable. When asserted, all gesture related
        interrupts are unmasked.
        */
        uint8_t GIEN : 2;

        uint8_t get() {
            return (GIEN << 1) | GMODE;
        }
        void set(uint8_t data) {
            GIEN  = (data >> 1) & 0x01;
            GMODE = data & 0x01;
        }
    };
    gconf4 _gconf4;

    struct gstatus {
        /* Gesture FIFO Data. GVALID bit is sent when GFLVL becomes greater than GFIFOTH (i.e. FIFO has
        enough data to set GINT). GFIFOD is reset when GMODE = 0 and the GFLVL=0 (i.e. All FIFO data
        has been read).
        */
        uint8_t GVALID : 1;

        /* Gesture FIFO Overflow. A setting of 1 indicates that the FIFO has filled to capacity and that new
        gesture detector data has been lost.
        */
        uint8_t GFOV : 1;

        void set(uint8_t data) {
            GFOV   = (data >> 1) & 0x01;
            GVALID = data & 0x01;
        }
    };
    gstatus _gstatus;
};

#endif
