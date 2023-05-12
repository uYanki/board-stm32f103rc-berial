#include "apds9960.h"

//////////////////////////////////////////////////////////////////

typedef union {
    uint8_t flags;
    struct {
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

        // uint8_t get() { return (GEN << 6) | (PIEN << 5) | (AIEN << 4) | (WEN << 3) | (PEN << 2) | (AEN << 1) | PON; };
    };
} enable;

typedef union {
    uint8_t flags;
    struct {
        // ALS Interrupt Persistence. Controls rate of Clear channel interrupt to the host processor
        uint8_t APERS : 4;
        // proximity interrupt persistence, controls rate of prox interrupt to host processor
        uint8_t PPERS : 4;

        // uint8_t get() { return (PPERS << 4) | APERS; };
    };
} pers;

typedef union {
    uint8_t flags;
    struct {
        uint8_t _null : 1;
        uint8_t WLONG : 1;
        // uint8_t get() { return WLONG << 1; };
    };
} config1;

typedef union {
    uint8_t flags;
    struct {
        /* Proximity Pulse Count. Specifies the number of proximity pulses to be generated on LDR. Number of pulses is set by PPULSE value plus 1.*/
        uint8_t PPULSE : 6;
        // Proximity Pulse Length. Sets the LED-ON pulse width during a proximity LDR pulse.
        uint8_t PPLEN : 2;

        // uint8_t get() { return (PPLEN << 6) | PPULSE; }
    };
} ppulse;

typedef union {
    uint8_t flags;
    struct {
        // ALS and Color gain control
        uint8_t AGAIN : 2;

        // proximity gain control
        uint8_t PGAIN : 2;

        // null
        uint8_t _null : 2;

        // led drive strength
        uint8_t LDRIVE : 2;

        // uint8_t get(){ return (LDRIVE << 6) | (PGAIN << 2) | AGAIN; }
    };
} control;

typedef union {
    uint8_t flags;
    struct {
        uint8_t _null : 4;
        /* Additional LDR current during proximity and gesture LED pulses. Current value, set by LDRIVE, is increased by the percentage of LED_BOOST. */
        uint8_t LED_BOOST : 2;
        // clear photodiode saturation int enable
        uint8_t CPSIEN : 1;
        // proximity saturation interrupt enable
        uint8_t PSIEN : 1;

        // uint8_t get() { return (PSIEN << 7) | (CPSIEN << 6) | (LED_BOOST << 4) | 1; }
    };
} config2;  // _null = 1

typedef union {
    uint8_t flags;
    struct {
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
    };
} status;

static void set_status(status* v, uint8_t data)
{
    v->flags = data;
    // v->AVALID = data & 0x01;
    // v->PVALID = (data >> 1) & 0x01;
    // v->GINT   = (data >> 2) & 0x01;
    // v->AINT   = (data >> 4) & 0x01;
    // v->PINT   = (data >> 5) & 0x01;
    // v->PGSAT  = (data >> 6) & 0x01;
    // v->CPSAT  = (data >> 7) & 0x01;
}

typedef union {
    uint8_t flags;
    struct {
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

        // uint8_t get() { return (PCMP << 5) | (SAI << 4) | (PMASK_U << 3) | (PMASK_D << 2) | (PMASK_L << 1) | PMASK_R; }
    };
} config3;

typedef union {
    uint8_t flags;
    struct {
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

        // uint8_t get() { return (GFIFOTH << 6) | (GEXMSK << 2) | GEXPERS; }
    };
} gconf1;

typedef union {
    uint8_t flags;
    struct {
        /* Gesture Wait Time. The GWTIME controls the amount of time in a low power mode between
        gesture detection cycles.
        */
        uint8_t GWTIME : 3;

        // Gesture LED Drive Strength. Sets LED Drive Strength in gesture mode.
        uint8_t GLDRIVE : 2;

        // Gesture Gain Control. Sets the gain of the proximity receiver in gesture mode.
        uint8_t GGAIN : 2;

        // uint8_t get() { return (GGAIN << 5) | (GLDRIVE << 3) | GWTIME; }
    };
} gconf2;

typedef union {
    uint8_t flags;
    struct {
        /* Number of Gesture Pulses. Specifies the number of pulses to be generated on LDR. Number of pulses is set by GPULSE value plus 1. */
        uint8_t GPULSE : 6;

        // Gesture Pulse Length. Sets the LED_ON pulse width during a Gesture LDR Pulse.
        uint8_t GPLEN : 2;

        // uint8_t get() { return (GPLEN << 6) | GPULSE; }
    };
} gpulse;

typedef union {
    uint8_t flags;
    struct {
        /* Gesture Dimension Select. Selects which gesture photodiode pairs are enabled to gather results during gesture. */
        uint8_t GDIMS : 2;

        // uint8_t get() { return GDIMS; }
    };
} gconf3;

typedef union {
    uint8_t flags;
    struct {
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

        // uint8_t get() { return (GIEN << 1) | GMODE; }
    };
} gconf4;

static void set_gconf4(gconf4* v, uint8_t data)
{
    v->flags = data & 0b111;
    // v->GIEN  = (data >> 1) & 0x01;
    // v->GMODE = data & 0x01;
}

typedef union {
    uint8_t flags;
    struct {
        /* Gesture FIFO Data. GVALID bit is sent when GFLVL becomes greater than GFIFOTH (i.e. FIFO has
        enough data to set GINT). GFIFOD is reset when GMODE = 0 and the GFLVL=0 (i.e. All FIFO data
        has been read).
        */
        uint8_t GVALID : 1;

        /* Gesture FIFO Overflow. A setting of 1 indicates that the FIFO has filled to capacity and that new
        gesture detector data has been lost.
        */
        uint8_t GFOV : 1;
    };
} gstatus;

static void set_gstatus(gstatus* v, uint8_t data)
{
    v->flags = data & 0b111;
    // v->GFOV   = (data >> 1) & 0x01;
    // v->GVALID = data & 0x01;
}

//////////////////////////////////////////////////////////////////

enable  _enable;
pers    _pers;
config1 _config1;
ppulse  _ppulse;
control _control;
config2 _config2;
status  _status;
config3 _config3;
gconf1  _gconf1;
gconf2  _gconf2;
gpulse  _gpulse;
gconf3  _gconf3;
gconf4  _gconf4;
gstatus _gstatus;

uint8_t gestCnt;
uint8_t UCount;
uint8_t DCount;
uint8_t LCount;
uint8_t RCount;

float powf(const float x, const float y)
{
    return (float)(pow((double)x, (double)y));
}

uint8_t APDS9960_ReadBytes(uint8_t reg, uint8_t* buf, uint8_t num)
{
    HAL_I2C_Mem_Read(&APDS9960_I2C, APDS9960_DEV, reg, I2C_MEMADD_SIZE_8BIT, buf, num, 0xFF);
    return num;
}

void APDS9960_WriteBytes(uint8_t reg, uint8_t* buf, uint8_t num)
{
    HAL_I2C_Mem_Write(&APDS9960_I2C, APDS9960_DEV, reg, I2C_MEMADD_SIZE_8BIT, buf, num, 0xFF);
}

void APDS9960_WriteByte(uint8_t reg, uint8_t value)
{
    APDS9960_WriteBytes(reg, &value, 1);
}

uint8_t APDS9960_ReadByte(uint8_t reg)
{
    uint8_t ret;
    APDS9960_ReadBytes(reg, &ret, 1);
    return ret;
}

uint32_t APDS9960_ReadU32(uint8_t reg)
{
    uint8_t ret[4];
    APDS9960_ReadBytes(reg, ret, 4);
    return (ret[0] << 24) | (ret[1] << 16) | (ret[2] << 8) | ret[3];
}

uint16_t APDS9960_ReadU16(uint8_t reg)
{
    uint8_t ret[2];
    APDS9960_ReadBytes(reg, ret, 2);
    return (ret[0] << 8) | ret[1];
}

uint16_t APDS9960_ReadU16R(uint8_t reg)
{
    uint8_t ret[2];
    APDS9960_ReadBytes(reg, ret, 2);
    return (ret[1] << 8) | ret[0];
}

// Enables / Disables the device (putting it in lower power sleep mode)
void APDS9960_Enable(uint8_t en)
{
    _enable.PON = en;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
}

//////////////////////////////////////////////////////////

// configures the sensor (call this function before doing anything else)
uint8_t APDS9960_Init()
{
    /* Make sure we're actually connected */
    uint8_t x = APDS9960_ReadByte(APDS9960_ID);
    if (x != 0xAB) return 0;

    /* Set default integration time and gain */
    APDS9960_SetADCIntegrationTime(10);
    APDS9960_SetADCGain(APDS9960_AGAIN_4X);

    // disable everything to start
    APDS9960_EnableGesture(0);
    APDS9960_EnableProximity(0);
    APDS9960_EnableColor(0);

    APDS9960_DisableColorInterrupt();
    APDS9960_DisableProximityInterrupt();
    APDS9960_ClearInterrupt();

    /* Note: by default, the device is in power down mode on bootup */
    APDS9960_Enable(0);
    HAL_Delay(10);
    APDS9960_Enable(1);
    HAL_Delay(10);

    // default to all gesture dimensions
    APDS9960_SetGestureDimensions(APDS9960_DIMENSIONS_ALL);
    APDS9960_SetGestureFIFOThreshold(APDS9960_GFIFO_4);
    APDS9960_SetGestureGain(APDS9960_GGAIN_4);
    APDS9960_SetGestureProximityThreshold(50);
    APDS9960_ResetCounts();

    _gpulse.GPLEN  = APDS9960_GPULSE_32US;
    _gpulse.GPULSE = 9;  // 10 pulses
    APDS9960_WriteByte(APDS9960_GPULSE, _gpulse.flags);

    return 1;
}

// Sets the integration time for the ADC of the APDS9960, in mills
void APDS9960_SetADCIntegrationTime(uint16_t iTimeMS)
{
    float temp;

    // convert ms into 2.78ms increments
    temp = iTimeMS;
    temp /= 2.78;
    temp = 256 - temp;
    if (temp > 255) temp = 255;
    if (temp < 0) temp = 0;

    /* Update the timing register */
    APDS9960_WriteByte(APDS9960_ATIME, (uint8_t)temp);
}

float APDS9960_GetADCIntegrationTime(void)
{
    float temp;

    temp = APDS9960_ReadByte(APDS9960_ATIME);

    // convert to units of 2.78 ms
    temp = 256 - temp;
    temp *= 2.78;
    return temp;
}

// Adjusts the color/ALS gain on the APDS9960 (adjusts the sensitivity to light)
void APDS9960_SetADCGain(APDS9960_AGain aGain)
{
    _control.AGAIN = aGain;
    /* Update the timing register */
    APDS9960_WriteByte(APDS9960_CONTROL, _control.flags);
}

APDS9960_AGain APDS9960_GetADCGain(void)
{
    return (APDS9960_AGain)(APDS9960_ReadByte(APDS9960_CONTROL) & 0x03);
}

// Adjusts the Proximity gain on the APDS9960
void APDS9960_SetProxGain(APDS9960_PGain pGain)
{
    _control.PGAIN = pGain;
    /* Update the timing register */
    APDS9960_WriteByte(APDS9960_CONTROL, _control.flags);
}

APDS9960_PGain APDS9960_GetProxGain(void)
{
    return (APDS9960_PGain)(APDS9960_ReadByte(APDS9960_CONTROL) & 0x0C);
}

void APDS9960_SetProxPulse(APDS9960_PPulseLen pLen, uint8_t pulses)
{
    if (pulses < 1) pulses = 1;
    if (pulses > 64) pulses = 64;
    --pulses;

    _ppulse.PPLEN  = pLen;
    _ppulse.PPULSE = pulses;

    APDS9960_WriteByte(APDS9960_PPULSE, _ppulse.flags);
}

// Enable proximity readings on APDS9960
void APDS9960_EnableProximity(uint8_t en)
{
    _enable.PEN = en;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
}

void APDS9960_EnableProximityInterrupt()
{
    _enable.PIEN = 1;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
    APDS9960_ClearInterrupt();
}

void APDS9960_DisableProximityInterrupt()
{
    _enable.PIEN = 0;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
}

void APDS9960_SetProximityInterruptThreshold(uint8_t low, uint8_t high, uint8_t persistance)
{
    APDS9960_WriteByte(APDS9960_PILT, low);
    APDS9960_WriteByte(APDS9960_PIHT, high);

    if (persistance > 7) persistance = 7;
    _pers.PPERS = persistance;
    APDS9960_WriteByte(APDS9960_PERS, _pers.flags);
}

uint8_t APDS9960_GetProximityInterrupt()
{
    set_status(&_status, APDS9960_ReadByte(APDS9960_STATUS));
    return _status.PINT;
};

// Read proximity data
uint8_t APDS9960_ReadProximity(void)
{
    return APDS9960_ReadByte(APDS9960_PDATA);
}

uint8_t APDS9960_GestureValid()
{
    set_gstatus(&_gstatus, APDS9960_ReadByte(APDS9960_GSTATUS));
    return _gstatus.GVALID;
}

void APDS9960_SetGestureDimensions(uint8_t dims)
{
    _gconf3.GDIMS = dims;
    APDS9960_WriteByte(APDS9960_GCONF3, _gconf3.flags);
}

void APDS9960_SetGestureFIFOThreshold(uint8_t thresh)
{
    _gconf1.GFIFOTH = thresh;
    APDS9960_WriteByte(APDS9960_GCONF1, _gconf1.flags);
}

void APDS9960_SetGestureGain(uint8_t gain)
{
    _gconf2.GGAIN = gain;
    APDS9960_WriteByte(APDS9960_GCONF2, _gconf2.flags);
}

void APDS9960_SetGestureProximityThreshold(uint8_t thresh)
{
    APDS9960_WriteByte(APDS9960_GPENTH, thresh);
}

void APDS9960_SetGestureOffset(uint8_t offset_up, uint8_t offset_down, uint8_t offset_left, uint8_t offset_right)
{
    APDS9960_WriteByte(APDS9960_GOFFSET_U, offset_up);
    APDS9960_WriteByte(APDS9960_GOFFSET_D, offset_down);
    APDS9960_WriteByte(APDS9960_GOFFSET_L, offset_left);
    APDS9960_WriteByte(APDS9960_GOFFSET_R, offset_right);
}

// Enable gesture readings on APDS9960
void APDS9960_EnableGesture(uint8_t en)
{
    if (!en) {
        _gconf4.GMODE = 0;
        APDS9960_WriteByte(APDS9960_GCONF4, _gconf4.flags);
    }
    _enable.GEN = en;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
    APDS9960_ResetCounts();
}

void APDS9960_ResetCounts()
{
    gestCnt = 0;
    UCount  = 0;
    DCount  = 0;
    LCount  = 0;
    RCount  = 0;
}

uint8_t APDS9960_ReadGesture(void)
{
    uint8_t       toRead, uint8_tsRead;
    uint8_t       buf[256];
    unsigned long t;
    uint8_t       gestureReceived;
    while (1) {
        int up_down_diff    = 0;
        int left_right_diff = 0;
        gestureReceived     = 0;
        if (!APDS9960_GestureValid()) return 0;

        HAL_Delay(30);
        toRead = APDS9960_ReadByte(APDS9960_GFLVL);

        uint8_tsRead = APDS9960_ReadBytes(APDS9960_GFIFO_U, buf, toRead);

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

        if (up_down_diff != 0 || left_right_diff != 0) t = HAL_GetTick();

        if (gestureReceived || HAL_GetTick() - t > 300) {
            APDS9960_ResetCounts();
            return gestureReceived;
        }
    }
}

// Set LED brightness for proximity/gesture
void APDS9960_SetLED(APDS9960_LedDrive drive, APDS9960_LedBoost boost)
{
    // set BOOST
    _config2.LED_BOOST = boost;
    APDS9960_WriteByte(APDS9960_CONFIG2, _config2.flags + 1);
    _control.LDRIVE = drive;
    APDS9960_WriteByte(APDS9960_CONTROL, _control.flags);
}

// Enable proximity readings on APDS9960
void APDS9960_EnableColor(uint8_t en)
{
    _enable.AEN = en;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
}

uint8_t APDS9960_ColorDataReady()
{
    set_status(&_status, APDS9960_ReadByte(APDS9960_STATUS));
    return _status.AVALID;
}

// Reads the raw red, green, blue and clear channel values
void APDS9960_GetColorData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c)
{
    *c = APDS9960_ReadU16R(APDS9960_CDATAL);
    *r = APDS9960_ReadU16R(APDS9960_RDATAL);
    *g = APDS9960_ReadU16R(APDS9960_GDATAL);
    *b = APDS9960_ReadU16R(APDS9960_BDATAL);
}

// Converts the raw R/G/B values to color temperature in degrees Kelvin
uint16_t APDS9960_CalculateColorTemperature(uint16_t r, uint16_t g, uint16_t b)
{
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

// Calculate ambient light values
uint16_t APDS9960_CalculateLux(uint16_t r, uint16_t g, uint16_t b)
{
    float illuminance;

    /* This only uses RGB ... how can we integrate clear or calculate lux */
    /* based exclusively on clear since this might be more reliable?      */
    illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

    return (uint16_t)illuminance;
}

void APDS9960_EnableColorInterrupt()
{
    _enable.AIEN = 1;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
}

void APDS9960_DisableColorInterrupt()
{
    _enable.AIEN = 0;
    APDS9960_WriteByte(APDS9960_ENABLE, _enable.flags);
}

void APDS9960_ClearInterrupt(void)
{
    APDS9960_WriteBytes(APDS9960_AICLEAR, NULL, 0);
}

void APDS9960_SetIntLimits(uint16_t low, uint16_t high)
{
    APDS9960_WriteByte(APDS9960_AILTIL, low & 0xFF);
    APDS9960_WriteByte(APDS9960_AILTH, low >> 8);
    APDS9960_WriteByte(APDS9960_AIHTL, high & 0xFF);
    APDS9960_WriteByte(APDS9960_AIHTH, high >> 8);
}
