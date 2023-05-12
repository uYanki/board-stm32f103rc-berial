/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：华邦 2M串行flash测试，并将测试信息通过串口1在电脑的超级终端中打印出来。         

**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "spi_flash.h"

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
//#define  sFLASH_ID              0xEF3015     //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID              0xEF4014	 //W25Q80
//#define  sFLASH_ID              0xEF4016	 //W25Q32

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = " 感谢您选用艾尔赛产品               http://lctech-inc.taobao.com";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{ 	
	/* 配置串口1为：115200 8-N-1 */
	USART1_Config();
	printf("\r\n 这是一个2M串行flash实验 \r\n");
	
	/* 2M串行flash W25X16初始化 */
	SPI_FLASH_Init();
	
	/* Get SPI Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	
	Delay( 200 );
	
	/* Get SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\n FlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* Check the SPI Flash ID */
	if (FlashID == 0xEF4015)  /* #define  sFLASH_ID  0xEF3015 */
	{	
		printf("\r\n 检测到华邦串行flash W25Q16 !\r\n");
		
		/* Erase SPI FLASH Sector to write on */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n 读出的数据为：%s \r\n", Tx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			printf("\r\n 2M串行flash(W25Q16)测试成功!\n\r");
		}
		else
		{        
			printf("\r\n 2M串行flash(W25Q16)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		printf("\r\n 获取不到 W25Q16 ID!\n\r");
	}
	
	
	
		if (FlashID == 0xEF4014)  /* #define  sFLASH_ID  0xEF3015 */
	{	
		printf("\r\n 检测到华邦串行flash W25Q80 !\r\n");
		
		/* Erase SPI FLASH Sector to write on */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n 读出的数据为：%s \r\n", Tx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			printf("\r\n 2M串行flash(W25Q80)测试成功!\n\r");
		}
		else
		{        
			printf("\r\n 2M串行flash(W25Q80)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		printf("\r\n 获取不到 W25Q80 ID!\n\r");
	}
	
	
			if (FlashID == 0xEF4016)  /* #define  sFLASH_ID  0xEF3015 */
	{	
		printf("\r\n 检测到华邦串行flash W25Q32 !\r\n");
		
		/* Erase SPI FLASH Sector to write on */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		printf("\r\n 写入的数据为：%s \r\t", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("\r\n 读出的数据为：%s \r\n", Tx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			printf("\r\n 2M串行flash(W25Q32)测试成功!\n\r");
		}
		else
		{        
			printf("\r\n 2M串行flash(W25Q32)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		printf("\r\n 获取不到 W25Q32 ID!\n\r");
	}
	
	
	SPI_Flash_PowerDown();  
	while(1);  
}

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
