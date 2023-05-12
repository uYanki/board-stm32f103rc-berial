#ifndef _AXP173_H_
#define _AXP173_H_

#include "axp173.h"
#include "i2c.h"

#define AXP173_I2C hi2c2        // <------
#define AXP173_DEV (0x34 << 1)  // <------

static uint16_t clamp(uint16_t v, uint16_t min, uint16_t max)
{
    if (min > max) {
        uint16_t t;
        t   = max;
        max = min;
        min = t;
    }

    if (v < min)
        v = min;
    else if (v > max)
        v = max;

    return v;
}

typedef enum {
    AXP173_OP_DCDC1 = 0,
    AXP173_OP_LDO4,
    AXP173_OP_LDO2,
    AXP173_OP_LDO3,
    AXP173_OP_DCDC2,
} OUTPUT_CHANNEL;

typedef enum {
    AXP173_ADC_TS = 0,
    AXP173_ADC_APS_V,
    AXP173_ADC_VBUS_C,
    AXP173_ADC_VBUS_V,
    AXP173_ADC_ACIN_C,
    AXP173_ADC_ACIN_V,
    AXP173_ADC_BAT_C,
    AXP173_ADC_BAT_V,
} ADC_CHANNEL;

typedef enum {
    AXP173_CHG_100mA = 0,
    AXP173_CHG_190mA,
    AXP173_CHG_280mA,
    AXP173_CHG_360mA,
    AXP173_CHG_450mA,
    AXP173_CHG_550mA,
    AXP173_CHG_630mA,
    AXP173_CHG_700mA,
    AXP173_CHG_780mA,
    AXP173_CHG_880mA,
    AXP173_CHG_960mA,
    AXP173_CHG_1000mA,
    AXP173_CHG_1080mA,
    AXP173_CHG_1160mA,
    AXP173_CHG_1240mA,
    AXP173_CHG_1320mA,
} CHARGE_CURRENT;

typedef enum {
    AXP173_COULOMETER_RESET = 5,
    AXP173_COULOMETER_PAUSE,
    AXP173_COULOMETER_ENABLE,
} COULOMETER_CTRL;

typedef enum {
    AXP173_POWEROFF_4S = 0,
    AXP173_POWEROFF_6S,
    AXP173_POWEROFF_8S,
    AXP173_POWEROFF_10S,
} POWEROFF_TIME;

typedef enum {
    AXP173_HIGH_RES = 0,
    AXP173_CHARGE_LED_1HZ,
    AXP173_CHARGE_LED_4HZ,
    AXP173_LOW_LEVEL,
} CHARGE_LED_FRE;

uint8_t AXP173_Init();
uint8_t AXP173_IsACINExist();
uint8_t AXP173_IsACINAvl();
uint8_t AXP173_IsVBUSExist();
uint8_t AXP173_IsVBUSAvl();
uint8_t AXP173_GetBatCurrentDir();
uint8_t AXP173_IsAXP173OverTemp();
uint8_t AXP173_IsCharging();
uint8_t AXP173_IsBatExist();
uint8_t AXP173_IsChargeCsmaller();
/* Power output control */
void AXP173_SetOutputEnable(OUTPUT_CHANNEL channel, uint8_t state);
void AXP173_SetOutputVoltage(OUTPUT_CHANNEL channel, uint16_t voltage);
/* Basic control */
void AXP173_PowerOFF();
void AXP173_SetPowerOffTime(POWEROFF_TIME time);
/* Charge control */
void AXP173_SetChargeEnable(uint8_t state);
void AXP173_SetChargeCurrent(CHARGE_CURRENT current);
void AXP173_SetChargeLedFre(CHARGE_LED_FRE frequency);
/* ADC control */
void AXP173_SetADCEnable(ADC_CHANNEL channel, uint8_t state);
/* Coulometer control */
void AXP173_SetCoulometer(COULOMETER_CTRL option, uint8_t state);
/* Coulometer data */
uint32_t AXP173_GetCoulometerChargeData();
uint32_t AXP173_GetCoulometerDischargeData();
float    getCoulometerData();
/* BAT data */
float AXP173_GetBatVoltage();
float AXP173_GetBatCurrent();
float AXP173_GetBatLevel();
float AXP173_GetBatPower();
/* VBUS data */
float AXP173_GetVBUSVoltage();
float AXP173_GetVBUSCurrent();
/* Temperature data */
float AXP173_GetAXP173Temp();
float AXP173_GetTSTemp();

#endif