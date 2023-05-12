启动过程：

![alloc_stack_heap](README.assets/alloc_stack_heap.png)





![hex](README.assets/hex.png)

| __inital_sp                                   | Reset_Handler                                 | NMI_Handler                                       |
| --------------------------------------------- | --------------------------------------------- | ------------------------------------------------- |
| ![__inital_sp](README.assets/__inital_sp.png) | ![NMI_Handler](README.assets/NMI_Handler.png) | ![Reset_Handler](README.assets/Reset_Handler.png) |





R14（LR，Link Register）：



SP：



heap：动态内存的申请和释（ malloc、realloc、free...）



![register_bank](README.assets/register_bank.png)





##### 双堆栈

主栈指针（MSP，Main Stack Pointer），进程栈指针（ PSP，Process Stack Pointer）。

`R13`（SP，Stack Pointer）：中断里只能指向 MSP，中断外可指向 MSP 或 PSP。

`R15`（PC，Program Counter）：程序计数器，直向下一条指令的位置。

注：在 RTOS 中，中断以外全使用 PSP，中断以内只使用 MSP。

CM3 和 CM4 是向下生长的满栈，即从高地址向低地址生长。

① 

进入 `main()` 之前，R13 寄存器的内容为栈顶指针 `__inital_sp` 。

进入 `main()` 之后，给局部变量分配了64个字节的空间，R13 寄存器的值减少了 0x40。

| 进入 `main()` 之前                            | 进入 `main()` 之后                          |
| --------------------------------------------- | ------------------------------------------- |
| ![before_main](README.assets/before_main.png) | ![after_main](README.assets/after_main.png) |

② 

单步往下，给变量赋值。

![set_value](README.assets/set_value.png)

③

设置 PSP 指向的位置，并切换为使用 PSP 模式。

| 设置 PSP 指针之前                                   | 设置 PSP 指针之后                                 | 切换到 PSP                            |
| --------------------------------------------------- | ------------------------------------------------- | ------------------------------------- |
| ![before_set_psp](README.assets/before_set_psp.png) | ![after_set_psp](README.assets/after_set_psp.png) | ![use_psp](README.assets/use_psp.png) |

④

触发中断前为 PSP，触发中断时自动切换到 MSP，处理完中断后自动切换回 PSP。

| 调用 `delay` 前                                 | 调用 `delay` 中                         | 调用 `delay` 后                               |
| ----------------------------------------------- | --------------------------------------- | --------------------------------------------- |
| ![before_delay](README.assets/before_delay.png) | ![do_delay](README.assets/do_delay.png) | ![after_delay](README.assets/after_delay.png) |

