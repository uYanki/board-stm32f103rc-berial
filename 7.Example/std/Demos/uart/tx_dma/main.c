#include "Led_Key.h"
#include "bsp_exti.h"
#include "bsp_SysTick.h"
#include "bsp_iwdg.h"
#include "bsp_wwdg.h"
#include "bsp_uart.h"
#include "bsp_dma.h"

int main(void)
{
	int i;
	
	SysTick_Configuration();
	
	Led_Configuration();
	
	Uart1_Configuration();
	
	DMA_Configuration();
	
	for(i = 0; i < 2000; i++)
		arr[i] = 'A';
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	while(1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 |  GPIO_Pin_2 |  GPIO_Pin_3 |  GPIO_Pin_4, Bit_SET);
		Delay_us(200000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 |  GPIO_Pin_2 |  GPIO_Pin_3 |  GPIO_Pin_4, Bit_RESET);
		Delay_us(200000);
	}
}

