# 设备参数存储方案

#### 介绍
设备参数存储方案，目前有基于EEPROM的、SPI FLASN的、NAND FLASH、SD卡的等等。

#### 使用说明

- 01.基于AT24C02

模拟以下参数的读写：

```c
typedef struct 
{
	//LED状态
	uint8_t led_status ;				
	//电机状态
	uint8_t motor_status ;
	//蜂鸣器状态
	uint8_t buzzer_status ;
	//网络状态
	uint8_t network_status ;
	//语言版本
	uint8_t language_version ;
	//设备密码
	uint8_t device_password[4] ;
	//报警阈值1
	uint16_t alarm_threshold1 ;
	//报警阈值2
	uint16_t alarm_threshold2 ;
	//报警阈值3
	uint16_t alarm_threshold3 ;
}System_Param ;
```

核心思想是通过定义两个全局的结构体变量，一个用来存储，一个用来读取。

- 02.基于W25Q64

思想和第一个案例一致。