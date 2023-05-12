/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * BUZZER(+) - VCC(3.3V)
 * BUZZER(-) - S9012(E)
 * S9012(B) - R(1K) - DATA(PA7)
 * S9012(C) - GND
 */
#define BEEP_PIN   GPIO_PIN_7
#define BEEP_PORT  GPIOA

#define BEEP_OUT_0 HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET)
#define BEEP_OUT_1 HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_SET)

#define NOTE_B0    31
#define NOTE_C1    33
#define NOTE_CS1   35
#define NOTE_D1    37
#define NOTE_DS1   39
#define NOTE_E1    41
#define NOTE_F1    44
#define NOTE_FS1   46
#define NOTE_G1    49
#define NOTE_GS1   52
#define NOTE_A1    55
#define NOTE_AS1   58
#define NOTE_B1    62
#define NOTE_C2    65
#define NOTE_CS2   69
#define NOTE_D2    73
#define NOTE_DS2   78
#define NOTE_E2    82
#define NOTE_F2    87
#define NOTE_FS2   93
#define NOTE_G2    98
#define NOTE_GS2   104
#define NOTE_A2    110
#define NOTE_AS2   117
#define NOTE_B2    123
#define NOTE_C3    131
#define NOTE_CS3   139
#define NOTE_D3    147
#define NOTE_DS3   156
#define NOTE_E3    165
#define NOTE_F3    175
#define NOTE_FS3   185
#define NOTE_G3    196
#define NOTE_GS3   208
#define NOTE_A3    220
#define NOTE_AS3   233
#define NOTE_B3    247
#define NOTE_C4    262
#define NOTE_CS4   277
#define NOTE_D4    294
#define NOTE_DS4   311
#define NOTE_E4    330
#define NOTE_F4    349
#define NOTE_FS4   370
#define NOTE_G4    392
#define NOTE_GS4   415
#define NOTE_A4    440
#define NOTE_AS4   466
#define NOTE_B4    494
#define NOTE_C5    523
#define NOTE_CS5   554
#define NOTE_D5    587
#define NOTE_DS5   622
#define NOTE_E5    659
#define NOTE_F5    698
#define NOTE_FS5   740
#define NOTE_G5    784
#define NOTE_GS5   831
#define NOTE_A5    880
#define NOTE_AS5   932
#define NOTE_B5    988
#define NOTE_C6    1047
#define NOTE_CS6   1109
#define NOTE_D6    1175
#define NOTE_DS6   1245
#define NOTE_E6    1319
#define NOTE_F6    1397
#define NOTE_FS6   1480
#define NOTE_G6    1568
#define NOTE_GS6   1661
#define NOTE_A6    1760
#define NOTE_AS6   1865
#define NOTE_B6    1976
#define NOTE_C7    2093
#define NOTE_CS7   2217
#define NOTE_D7    2349
#define NOTE_DS7   2489
#define NOTE_E7    2637
#define NOTE_F7    2794
#define NOTE_FS7   2960
#define NOTE_G7    3136
#define NOTE_GS7   3322
#define NOTE_A7    3520
#define NOTE_AS7   3729
#define NOTE_B7    3951
#define NOTE_C8    4186
#define NOTE_CS8   4435
#define NOTE_D8    4699
#define NOTE_DS8   4978

void tone(uint32_t frequency, uint32_t duration)
{
    uint32_t i, n;
    if (frequency != 0) {
        n = 500000 / frequency;

        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
        for (i = 0; i < (frequency * duration / 1000); i++) {
            BEEP_OUT_0;
            HAL_Delay(n);  // delay_us
            BEEP_OUT_1;
            HAL_Delay(n);
        }
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    } else {
        HAL_Delay(duration);
    }
}

void play_badapple()
{
    uint32_t bpm      = 137;
    // 12000 = 60 * 1000 * 4 * 0.8 / 16 quarter note = one beat
    uint32_t ndms     = 12000;
    // notes in the melody:
    // 1 = #F to 1 = G 4/4
    uint32_t melody[] = {
        // Intro
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2, NOTE_DS2,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
        // Verse 1 - 16
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
        // Verse 17 - 32
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_F5,
        NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        // Verse 33 - 48
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_F5,
        NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, 0,
        // Interlude
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_GS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3,
        NOTE_DS3, 0, NOTE_DS3, NOTE_CS3, NOTE_DS3, NOTE_GS3, NOTE_FS3, NOTE_GS3, NOTE_FS3, NOTE_DS3, NOTE_FS3,
        // Verse(2) 1 - 16
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_F4, NOTE_DS4, NOTE_D4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_CS5,
        NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_GS4, NOTE_FS4, NOTE_F4,
        NOTE_DS4, NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_FS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4,
        // Verse(2) 17 - 32
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_DS4,
        NOTE_F4, NOTE_FS4, NOTE_GS4, NOTE_AS4, NOTE_DS4, NOTE_AS4, NOTE_CS5,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        NOTE_CS5, NOTE_DS5, NOTE_AS4, NOTE_GS4, NOTE_AS4, NOTE_DS5, NOTE_F5,
        NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_CS5, NOTE_AS4, NOTE_GS4, NOTE_AS4,
        //                                               to 1=G
        NOTE_GS4, NOTE_FS4, NOTE_F4, NOTE_CS4, NOTE_DS4, NOTE_B4, NOTE_D5,
        // Verse(2) 33 - 48 1=G
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
        NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_D5,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
        NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_D5,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4,
        NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_B4, NOTE_D5,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_D5, NOTE_E5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_E5, NOTE_FS5,
        NOTE_G5, NOTE_FS5, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_B4,
        NOTE_A4, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_E4, 0,
        // Outro 1=G
        NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
        NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
        NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
        NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, 0,
        0};
    // note durations: unit: 1/16 note, time = number * (1/16), ex: 1/4 note, number: 4, 1/8 note, number: 2:
    uint32_t noteDurations[] = {
        // Intro
        4, 4, 4, 1, 1, 1, 1,
        4, 4, 4, 2, 2,
        4, 4, 4, 1, 1, 1, 1,
        4, 4, 4, 2, 2,
        4, 4, 4, 1, 1, 1, 1,
        4, 4, 4, 2, 2,
        4, 4, 4, 1, 1, 1, 1,
        4, 4, 4, 2, 2,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        // Verse 1 - 16
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 4, 4,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 4, 4,
        // Verse 17 - 32
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        // Verse 33 - 48
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 4,
        // Interlude
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        4, 1, 1, 1, 1, 4, 1, 1, 1, 1,
        4, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1,
        // Verse(2) 1 - 16
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 4, 4,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        4, 4, 4, 4,
        // Verse(2) 17 - 32
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        // Verse(2) 33 - 48
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 2, 2,
        2, 2, 2, 2, 4, 4,
        // Outro
        3, 3, 2, 3, 3, 2,
        3, 3, 2, 3, 3, 2,
        3, 3, 2, 3, 3, 2,
        3, 3, 2, 3, 3, 2,
        16};
    for (uint32_t i = 0; i < 895; i++) {
        uint32_t noteDuration = ndms * noteDurations[i] / bpm;
        tone(melody[i], noteDuration);
        uint32_t pauseBetweenNotes = noteDuration / 4;
        HAL_Delay(pauseBetweenNotes);
    }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        play_badapple();
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType     = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState           = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue     = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState           = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState       = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource      = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL         = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
