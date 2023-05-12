sys.h

```c
#ifndef _SYS_H_
#define _SYS_H_

#include "main.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //
#endif

```

```
// IO方向设置
#define DHT11_IO_IN()  {IODHT22_GPIO_Port->CRH &= 0xFFFFF0FF;	IODHT22_GPIO_Port->CRH |= 8 << 8;}	// PC10 IN  MODE
#define DHT11_IO_OUT() {IODHT22_GPIO_Port->CRH &= 0xFFFFF0FF;	IODHT22_GPIO_Port->CRH |= 3 << 8;}	// PC10 OUT MODE

// IO操作函数
#define	DHT11_DQ_OUT PCout(10) // 数据端口	PC10
#define	DHT11_DQ_IN  PCin(10)  // 数据端口	PC10
```

