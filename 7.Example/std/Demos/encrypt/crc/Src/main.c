/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   CRC计算。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 霸道开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./crc/bsp_crc.h"

extern __IO uint32_t CRCValue;

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{
	uint8_t i = 0;
		
	/* USART1 config 115200 8-N-1 */
	USART_Config();
	
	/* 使能CRC时钟 */
	CRC_Config();
	
	printf("\r\n 这是一个 CRC(循环冗余校验)实验 \r\n");
	
	/* Compute the CRC of "DataBuffer" */
	for(i=0; i<BUFFER_SIZE; i++ ) 
	{
		//对该寄存器进行写操作时，作为输入寄存器，可以输入要进行CRC计算的新数据。
		//对该寄存器进行读操作时，返回上一次CRC计算的结果。
		//每一次写入数据寄存器，其计算结果是前一次CRC计算结果和新计算结果的组合(对整个32位字
		//进行CRC计算，而不是逐字节地计算)。
		CRCValue = CRC_CalcBlockCRC((uint32_t *)DataBuffer, BUFFER_SIZE);
		printf("\r\n32-bit CRC 校验码为:0X%X\r\n", CRCValue);
	}
	printf("\r\nCRC(循环冗余校验)测试成功\r\n");

	for(;;)
	{
		
	}
}
