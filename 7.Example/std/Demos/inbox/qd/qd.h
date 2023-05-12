#ifndef __qd_H
#define __qd_H
#include "sys.h"

#define LED0 PAout(0)
#define LED1 PAout(1)
#define LED2 PAout(2)

#define KEYN 1   	//定义使用几个按键
#define KEYCS 60   //定义长按判定时间
#define KEYSS 60   //定义双击判定时间


typedef struct {
  u8 s[KEYN+1];	//存储按键位号
	u8 z;	//存储按键状态
} KEY;
extern KEY key;

extern const unsigned char kl[];
extern const unsigned char pm[];
	
void LED_init(void);
void KEY_init(void);

void tset(void);
	
//void ZD_init(void);//外部中断
//void TIME_init(u16 per,u16 psc);
//void TIME_PWM_init(u16 per,u16 psc);

void key_scan();

void RCC_ClkConfiguration(void);//设置系统时钟


#endif
