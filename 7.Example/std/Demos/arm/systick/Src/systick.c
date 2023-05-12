#include "bsp_systick.h"


#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	//  判断 tick 的值是否大于 2^24，如果大于，则不符合规则
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);

  // 初始化reload寄存器的值	
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;
	
  // 配置中断优先级，配置为15，默认为最低的优先级
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 

  // 初始化counter的值为0	
  SysTick->VAL   = 0; 
  
  // 配置 systick 的时钟为 72M
  // 使能中断
	// 使能systick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    
  return (0);                                                 
}
#endif


void SysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	SysTick_Config(72);
	
	for(i=0; i<us; i++)
	{
		while( !((SysTick->CTRL) & (1<<16)) );
	}
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000);
	
	for(i=0; i<ms; i++)
	{
		while( !((SysTick->CTRL) & (1<<16)) );
	}
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

















