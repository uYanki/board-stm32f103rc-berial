#include "iwdg.h"

// IWDG 工作在 40KHz LSI 的时钟下, 且没有复位预警中断

/**
 * @param [in] prescaler 分频数 0~7 (3bit), 分频因子 = 4*2^prescaler (最大值只能是256)
 * @param [in] reload 重装载值 11-bit
 *
 * @note 溢出时间 Tout=((4*2^prescaler)*reload)/40 (ms).
 */
void iwdg_init(uint8_t prescaler, uint16_t reload)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(prescaler);  // 置预分频值
    IWDG_SetReload(reload);        // 置重装载值
    IWDG_ReloadCounter();
    IWDG_Enable();
}

void iwdg_feed(void)
{
    IWDG_ReloadCounter();
}
