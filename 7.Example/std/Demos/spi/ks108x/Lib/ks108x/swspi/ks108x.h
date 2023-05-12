#ifndef __KS108X_H__
#define __KS108X_H__

#include "board.h"

// ADC
#define PIN_KS108X_VO1
#define PIN_KS108X_VO2

// SPI ( MSB First, CPOL = 0, CPHA = 1  )
#define PIN_KS108X_MISO    PAin(6)   // SDO
#define PIN_KS108X_MOSI    PAout(7)  // SDI
#define PIN_KS108X_SCLK    PAout(5)
#define PIN_KS108X_NSS     PBout(11)

#define CONFIG_PRINF_RXDAT 1

typedef enum {  // bit[5]
    KS108X_CHANNEL1_POWER_UP   = 1,
    KS108X_CHANNEL1_POWER_DOWN = 0,
} ks108x_chl1en_e;

typedef enum {  // bit[5]
    KS108X_CHANNEL2_POWER_UP   = 0,
    KS108X_CHANNEL2_POWER_DOWN = 1,
} ks108x_chl2en_e;

typedef enum {  // bit[4:2]
    KS108X_GAIN2_40 = 0,
    KS108X_GAIN2_10 = 1,
    KS108X_GAIN2_20 = 2,
    KS108X_GAIN2_30 = 3,
    KS108X_GAIN2_60 = 4,
    KS108X_GAIN2_80 = 5,
} ks108x_gain2_e;

typedef enum {  // bit[1:0]
    KS108X_GAIN1_9 = 0,
    KS108X_GAIN1_5 = 1,
} ks108x_gain1_e;

void ks108x_init(void);
void ks108x_reset(void);
void ks108x_set_ch1(ks108x_chl1en_e enable, ks108x_gain1_e gain1, ks108x_gain2_e gain2);
void ks108x_set_ch2(ks108x_chl2en_e enable, ks108x_gain1_e gain1, ks108x_gain2_e gain2);

#endif
