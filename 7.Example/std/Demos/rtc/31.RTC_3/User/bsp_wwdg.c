#include "bsp_wwdg.h"

unsigned char WWDG_CNT = 0x7F;

void WWDG_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void WWDG_Configuration(unsigned char tr, unsigned char wr, unsigned int pr)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	WWDG_CNT = tr & WWDG_CNT;
	WWDG_SetPrescaler(pr);
	WWDG_SetWindowValue(wr);
	WWDG_Enable(WWDG_CNT);

	WWDG_NVIC_Init();
	WWDG_ClearFlag();
	WWDG_EnableIT();
}
