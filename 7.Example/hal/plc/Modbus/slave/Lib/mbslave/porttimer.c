/* ----------------------- System includes --------------------------------- */
#include "port.h"
#include "tim.h"

/* ----------------------- Modbus includes --------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ----------------------------------------- */

/* ----------------------- Static variables -------------------------------- */

/* ----------------------- Start implementation ---------------------------- */

#include "usart.h"

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    htim7.Init.Period = usTim1Timerout50us - 1;
    printf("%d\r\n", usTim1Timerout50us);
    return HAL_TIM_Base_Init(&htim7) == HAL_OK ? TRUE : FALSE;
}

inline void vMBPortTimersEnable()
{
    htim7.Instance->CNT = 0;
    HAL_TIM_Base_Start_IT(&htim7);
}

inline void vMBPortTimersDisable()
{
    HAL_TIM_Base_Stop(&htim7);
}
