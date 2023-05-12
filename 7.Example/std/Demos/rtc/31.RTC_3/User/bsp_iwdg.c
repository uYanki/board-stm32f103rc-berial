#include "bsp_iwdg.h"

//使能IWDG，在40KHz LSI下，约等于每隔1s产生IWDG复位
void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	IWDG_SetReload(625);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void IWDG_FeedDog(void)
{
	IWDG_ReloadCounter();
}

