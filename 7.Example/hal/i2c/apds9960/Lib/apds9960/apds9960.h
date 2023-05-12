#ifndef __APDS9960_H_
#define __APDS9960_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"

#define APDS9960_I2C hi2c2        // <----
#define APDS9960_DEV (0x39 << 1)  // <----

///////////////////////////////////////////////////////////////////////

// registers

#define APDS9960_RAM        0x00
#define APDS9960_ENABLE     0x80
#define APDS9960_ATIME      0x81
#define APDS9960_WTIME      0x83
#define APDS9960_AILTIL     0x84
#define APDS9960_AILTH      0x85
#define APDS9960_AIHTL      0x86
#define APDS9960_AIHTH      0x87
#define APDS9960_PILT       0x89
#define APDS9960_PIHT       0x8B
#define APDS9960_PERS       0x8C
#define APDS9960_CONFIG1    0x8D
#define APDS9960_PPULSE     0x8E
#define APDS9960_CONTROL    0x8F
#define APDS9960_CONFIG2    0x90
#define APDS9960_ID         0x92
#define APDS9960_STATUS     0x93
#define APDS9960_CDATAL     0x94
#define APDS9960_CDATAH     0x95
#define APDS9960_RDATAL     0x96
#define APDS9960_RDATAH     0x97
#define APDS9960_GDATAL     0x98
#define APDS9960_GDATAH     0x99
#define APDS9960_BDATAL     0x9A
#define APDS9960_BDATAH     0x9B
#define APDS9960_PDATA      0x9C
#define APDS9960_POFFSET_UR 0x9D
#define APDS9960_POFFSET_DL 0x9E
#define APDS9960_CONFIG3    0x9F
#define APDS9960_GPENTH     0xA0
#define APDS9960_GEXTH      0xA1
#define APDS9960_GCONF1     0xA2
#define APDS9960_GCONF2     0xA3
#define APDS9960_GOFFSET_U  0xA4
#define APDS9960_GOFFSET_D  0xA5
#define APDS9960_GOFFSET_L  0xA7
#define APDS9960_GOFFSET_R  0xA9
#define APDS9960_GPULSE     0xA6
#define APDS9960_GCONF3     0xAA
#define APDS9960_GCONF4     0xAB
#define APDS9960_GFLVL      0xAE
#define APDS9960_GSTATUS    0xAF
#define APDS9960_IFORCE     0xE4
#define APDS9960_PICLEAR    0xE5
#define APDS9960_CICLEAR    0xE6
#define APDS9960_AICLEAR    0xE7
#define APDS9960_GFIFO_U    0xFC
#define APDS9960_GFIFO_D    0xFD
#define APDS9960_GFIFO_L    0xFE
#define APDS9960_GFIFO_R    0xFF

///////////////////////////////////////////////////////////////////////

typedef enum {
    APDS9960_AGAIN_1X  = 0x00,  // No gain
    APDS9960_AGAIN_4X  = 0x01,  // 2x gain
    APDS9960_AGAIN_16X = 0x02,  // 16x gain
    APDS9960_AGAIN_64X = 0x03   // 64x gain
} APDS9960_AGain;

typedef enum {
    APDS9960_PGAIN_1X = 0x00,  // 1x gain
    APDS9960_PGAIN_2X = 0x04,  // 2x gain
    APDS9960_PGAIN_4X = 0x08,  // 4x gain
    APDS9960_PGAIN_8X = 0x0C   // 8x gain
} APDS9960_PGain;

typedef enum {
    APDS9960_PPULSELEN_4US  = 0x00,  // 4uS
    APDS9960_PPULSELEN_8US  = 0x40,  // 8uS
    APDS9960_PPULSELEN_16US = 0x80,  // 16uS
    APDS9960_PPULSELEN_32US = 0xC0   // 32uS
} APDS9960_PPulseLen;

typedef enum {
    APDS9960_LEDDRIVE_100MA = 0x00,  // 100mA
    APDS9960_LEDDRIVE_50MA  = 0x40,  // 50mA
    APDS9960_LEDDRIVE_25MA  = 0x80,  // 25mA
    APDS9960_LEDDRIVE_12MA  = 0xC0   // 12.5mA
} APDS9960_LedDrive;

typedef enum {
    APDS9960_LEDBOOST_100PCNT = 0x00,  // 100%
    APDS9960_LEDBOOST_150PCNT = 0x10,  // 150%
    APDS9960_LEDBOOST_200PCNT = 0x20,  // 200%
    APDS9960_LEDBOOST_300PCNT = 0x30   // 300%
} APDS9960_LedBoost;

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

#define APDS9960_UP    0x01
#define APDS9960_DOWN  0x02
#define APDS9960_LEFT  0x03
#define APDS9960_RIGHT 0x04

////////////////////////////////////////////////

uint8_t        APDS9960_Init(void);
void           APDS9960_SetADCIntegrationTime(uint16_t iTimeMS);
float          APDS9960_GetADCIntegrationTime(void);
void           APDS9960_SetADCGain(APDS9960_AGain gain);
APDS9960_AGain APDS9960_GetADCGain(void);
void           APDS9960_SetLED(APDS9960_LedDrive drive, APDS9960_LedBoost boost);

// proximity
void           APDS9960_EnableProximity(uint8_t enable);
void           APDS9960_SetProxGain(APDS9960_PGain gain);
APDS9960_PGain APDS9960_GetProxGain(void);
void           APDS9960_SetProxPulse(APDS9960_PPulseLen pLen, uint8_t pulses);
void           APDS9960_EnableProximityInterrupt();
void           APDS9960_DisableProximityInterrupt();
uint8_t        APDS9960_ReadProximity(void);
void           APDS9960_SetProximityInterruptThreshold(uint8_t low, uint8_t high, uint8_t persistance /*4*/);
uint8_t        APDS9960_GetProximityInterrupt();

// gesture
void    APDS9960_EnableGesture(uint8_t enable);
uint8_t APDS9960_GestureValid();
void    APDS9960_SetGestureDimensions(uint8_t dims);
void    APDS9960_SetGestureFIFOThreshold(uint8_t thresh);
void    APDS9960_SetGestureGain(uint8_t gain);
void    APDS9960_SetGestureProximityThreshold(uint8_t thresh);
void    APDS9960_SetGestureOffset(uint8_t offset_up, uint8_t offset_down, uint8_t offset_left, uint8_t offset_right);
uint8_t APDS9960_ReadGesture(void);
void    APDS9960_ResetCounts();

// light & color
void     APDS9960_EnableColor(uint8_t enable);
uint8_t  APDS9960_ColorDataReady();
void     APDS9960_GetColorData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* c);
uint16_t APDS9960_CalculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
uint16_t APDS9960_CalculateLux(uint16_t r, uint16_t g, uint16_t b);
void     APDS9960_EnableColorInterrupt();
void     APDS9960_DisableColorInterrupt();
void     APDS9960_ClearInterrupt(void);
void     APDS9960_SetIntLimits(uint16_t l, uint16_t h);

// turn on/off elements
void APDS9960_Enable(uint8_t enable);

#endif
