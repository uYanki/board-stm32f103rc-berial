### RTOS

#### 全局配置

![9](README.assets/9.png)

![10](README.assets/10.png)

![11](README.assets/11.png)

![12](README.assets/12.png)

此中断优先级是针对硬件芯片的 `NVIC` 中断优先级而言的，与 RTOS 中的任务优先级是不同的。

#### 函数使能

![13](README.assets/13.png)

#### 【添加】任务&队列

![1](README.assets/1.png)

任务调度的简单理解：在周期性的触发中断里，保存上一个任务的上下文，然后切换到下一个任务中。

#### 【添加】定时器&信号量

![15](README.assets/15.png)

#### 【添加】互斥量

![16](README.assets/16.png)

#### 【添加】事件标志组

![17](README.assets/17.png)

#### 【添加】用户常量

通常不用。

![14](README.assets/14.png)

#### 堆使用情况

![18](README.assets/18.png)

### 时基

警告：建议使用除了`Systick` 以外的时基。（即 HAL库 和 FreeeRTOS 不要使用相同的时基）

![4](README.assets/4.png)

在生成的函数 `SysTick_Handler` 中，可看出 HAL 和 FreeRTOS 使用了相同的时基：

![6](README.assets/6.png)

![7](README.assets/7.png)

注释大意是：SysTick 以最低的中断优先级运行，因此当该中断执行时，会屏蔽其他所有中断。

```
vPortRaiseBASEPRI()：进入临界区，即将滴答定时器中断设置为最高，防止被其他中断打断。
vPortClearBASEPRIFromISR()：退出临界区，即恢复滴答定时器的中断优先级。
```

**处理方法**：将 HAL库 的时基从 `Systick` 改为 `TIMx` 即可

![3](README.assets/3.png)

此处选择 TIM2 作为 HAL库 的是时钟源：

![5](README.assets/5.png)

此时的 `StsTick_Handler` 被定义为 `xPortSysTickHandler`

```c
#define xPortSysTickHandler SysTick_Handler
```

![8](README.assets/8.png)

### 编译器

AC6 较于 AC5 有较大的改动，使用 AC6 直接编译会报错，因此选择 AC5 版本。

![2](README.assets/2.png)