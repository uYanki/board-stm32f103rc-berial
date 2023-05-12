/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
 * File Name          : main.c
 * Author             : MCD Application Team
 * Version            : V2.0.3
 * Date               : 09/22/2008
 * Description        : Main program body
 ********************************************************************************
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x_lib.h"
#include "userinc/OLED.h"
#include "userinc/gpio.h"

extern u8 nonside[];

// ´®¿Ú³õÊ¼º¯Êý
void USARTTEST_Configuration(void);
/* Private functions ---------------------------------x------------------------*/
void Dly_ms(int ms)
{
    int ii, jj;
    if (ms < 1) ms = 1;
    for (ii = 0; ii < ms; ii++)
        for (jj = 0; jj < 500; jj++)
            ;  // 16MHz--1ms
    // for(jj=0;jj<4006;jj++);  //48MHz--1ms
    // for(jj=0;jj<5341;jj++);    //64MHz--1ms
}

/*******************************************************************************
 * Function Name  : main
 * Description    : Main program.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{
    LCD_Init();

    while (1) {
        LCD_Print(0, 0, "ËÑË÷ÎÀÐÇ...", TYPE16X16, TYPE8X16);
        LCD_Print(0, 16, "abcd09876", TYPE16X16, TYPE8X16);
        LCD_Print(16, 32, "efg12345", TYPE16X16, TYPE6X8);
        Draw_BMP(32, 48, nonside);
        Draw_BMP(48, 48, nonside);
        Draw_BMP(64, 48, nonside);
    }
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
