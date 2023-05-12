#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "myiic.h"
#include "snake.h"
#include "adc.h"
int main(void)
{
    delay_init();                                    // 延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    LED_Init();                                      // LED端口初始化
    OLED_Init();                                     // 初始化OLED
    Adc_Init();                                      // ADC初始化
    Snake_Init();                                    // 蛇初始化
    Map();                                           // 地图
    Food();                                          // 食物
    while (1) {
        GUI();
    }
}
