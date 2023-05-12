#include "delay.h"

inline void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

inline void delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}
