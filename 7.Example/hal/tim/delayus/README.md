使用16位基本定时器作微秒级延时：

![1](README.assets/1.png)

72MHz 经分频后为 1MHz（即每1微秒计1次数），并设置为向上计数。

```c
#define TIM_DELAY htim7
void delay_us(uint16_t n) {
    __HAL_TIM_SET_COUNTER(&TIM_DELAY, 0);
    HAL_TIM_Base_Start(&TIM_DELAY);
    while (__HAL_TIM_GET_COUNTER(&TIM_DELAY) < n) {}
    HAL_TIM_Base_Stop(&TIM_DELAY);
}
```

