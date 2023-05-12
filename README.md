## board-stm32f103rc-berial

基于 stm32f103rct6 的开发板

硬件：https://oshwhub.com/aris.95/stm32f103rct6-core（最新）

注：HAL 工程不带固件库，需提前在 STM32CubeMX 中安装好 F1 的固件库。

---

例程包含：

* 假货宝上的常用模块例程
* ARM 数学库中的 DSP 例程
* CMSIS-NN 神经网络例程

|                   |  | desc | status |
| ----------------- | ------------- | ----------------------------------- | ------ |
| **GPIO**          |               |                                     | - |
| | ds1302                           | 实时时钟                            | √      |
|| dht11                            | 湿度传感器                          | √      |
| | ds18b20                          | 温度传感器                          | √      |
| | hx71x                            | 24 位 ADC | √      |
|  |matrixkey                       | 矩阵键盘                            | √      |
| |beep | 无源蜂鸣器播放音乐 |  |
| **UART** |               |                                     | - |
|  | esp01 | wifi 模块 | |
|  | sim800 | | |
| **TIM**           |               | 定时器                              | - |
| | delayus                          | 微秒级延时                          | √      |
| | ws2812                           | 彩灯（PWM驱动） | √      |
|  |hcsr04                           | 超声波传感器（输入捕获）            | √      |
|  |encoder                          | 旋转编码器（定时器模式&编码器模式） | √      |
| | hx1838                          | 红外接收（输入捕获）                      | √      |
| | button | 按键短按长按双击识别 | √ |
| **WDG**           |               | 看门狗                          | √ |
| **ADC**           |               | 模拟量输入                          | - |
|  | irq | 单通道中断采集 | √ |
|  | dma | 单通道/多通道DMA采集 | √ |
|  |temt6000                         | 光照传感器                          | √      |
|  |s12d                             | 紫外光传感器                        | √      |
|  |pulse                           | 脉搏传感器                          | √      |
|  |sound                            | 声音传感器                          | √      |
| **DAC**           |               | 模拟量输出                          | - |
|                   | triangle wave | 三角波生成                          | √      |
|  |sine wave                        | 正弦波生成                          | √      |
| | heart-shaped wave                | 心形波生成                          | √      |
| | xy-mode | 示波器播放动画 |  |
| **I2C**           |               |                                     | - |
|                   | i2cbus | 模拟 I2C | √ |
| | lis3dh | 3轴加速度传感器 | √ |
|                | mpu6050（euler）   | 加速度+陀螺仪，欧拉角输出            | √      |
|                | mpu6050（kalman）  | 6轴，欧拉角+卡尔曼滤波输出 | √      |
|                | mpu6050（dmp）     | 6轴，DMP输出               | √      |
|                | mpuxxxx            | 6轴（加速度+陀螺仪）                 | √      |
|                |                    | 9轴（加速度+陀螺仪+地磁计）          | √      |
|  | mpu9250（dmp） | 9轴，DMP输出 |  |
|                | bmp280             | 大气压传感器                         | √      |
|                | bme280             | 温湿度大气压传感器                   | √ |
|                | aht10              | 温湿度传感器                         |        |
|                | aht20              | 温湿度传感器                         |        |
|                | sht30              | 温湿度传感器 | √ |
|                | as5600             | 磁编码器 | √ |
|                | hmc5883l           | 地磁传感器 | √ |
|                | xmc5883l           | 地磁传感器（HMC\QMC\VMC） | √ |
|                | pcf8574            | IO 扩展 | √ |
|                | pcf8574（lcd1602） | 液晶屏 | √ |
|                | pcf8575            | IO 扩展 |  |
|                | axp173             | 电源管理 IC | √ |
|                | axp192             | 电源管理 IC |  |
|                | axp209             | 电源管理 IC |  |
|                | at24cxx            | eeprom | √ |
|                | ssd1306 | OLED | √ |
| | ssd1306（animation） | 太空人 / IKUN  动画 | √ |
| | ssd1306（video） | 串口数据流播放视频 | √ |
| | ssd1306（u8g2） | 移植 u8g2 图形库 | √ |
|                | max30100 | 心率血氧传感器 | √ |
|                | max30102 | 心率血氧接近传感器 | √ |
| | max30105 |  | |
| | pn532 |  | |
| | tca9548 | I2C 多路复用器 | √ |
| | ina3221 | | |
| | ina219 | | |
| | adxl345 | | |
| |                      |                                     |        |
| | ap3216 | | |
| | si5351 | 波形发生器 | |
| | tcs3472 | 颜色传感器 | √ |
| | lm75 | 温度传感器 | √ |
| | pca9685 | 多路PWM舵机控制器 | √ |
| | mlx90614 | 红外测温传感器 | √ |
| | gy614 | 红外测温传感器 | √ |
| **SPI**           |               |                                     | - |
|  | spibus | 模拟 SPI | √ |
|                   | st7735_0.96 | 板载 0.96 寸屏幕                    | √      |
| | st7735_luat | 合宙 1.8 寸屏幕 | √ |
| | st7735_gfx3d | 3D 图形加速 | √ |
|                   | mfrc522       | RFID                                | √      |
|                   | w25qxx        | FLASH                               | √      |
|                   | ad9833        | 波形发生器 | √      |
| | ssd1306 | OLED |  |
| | pn532 |  |  |
| | w5500                | 以太网                              |        |
|                | lan8720              | 以太网                              |        |
|                | nrf24l01             | 2.4传输                             |        |
|                | mcp2551              | SPI转CAN                            |        |
|                | st7789               | 屏幕                                | √ |
|                | ws2812               | 彩灯                                |  |
| | ad7705 | 16位ADC | √ |
| | epaper | 合宙 1.54 寸墨水屏 | √ |
| **I2S**           |               |                                     | - |
|  | i2sbus | 模拟 I2S |  |
|                   | inmp441       | 麦克风                              | √      |
| | mp34dt05 | 麦克风 |  |
| | max98357 | 喇叭 |  |
| **CAN** |  |  | - |
|  | tj1050 |  |  |
|  | vp232 |  |  |
| **FAFTS**         |               | 文件系统                            | - |
|                   | sd card（spi） | TF 卡（spi 接口）                   | √      |
|                   | sd card（sdio） | TF 卡（sdio 接口）                  | √      |
|                   | w25qxx（spi） | 外部 flash（spi 接口）              | √      |
| **USB**           |               |                                     | - |
| MSC               |               | 大容量设备（U盘）                   | - |
|                   | sram          | 内部 SRAM 模拟 U 盘                 | √      |
|                   | w25qxx        | 外部 Flash 模拟 U 盘                | √      |
|                   | rom           | 片上 Flash 模拟 U 盘                |        |
| CDC               |               |                             | -      |
|  | | 虚拟串口 | √ |
| HID               |               | 人体学接口设备 | - |
|                   | keypress      | 模拟键盘按键 | √      |
| **RTOS**          |               | 实时操作系统                          | - |
| UCOS-II | |  | - |
| UCOS-III | |  | - |
| FreeRTOS          |               |                                     | - |
|  | task | 任务 | √ |
| RT-Thread Nano    |               | RTT OS                              | - |
|  | start | 移植 | √ |
| TencentOS tiny    |               | 腾讯 OS                             | - |
|  | start | 移植 | √ |
|                   | task | 任务 | √ |
| | mutex | 互斥量 | √ |
| | sem | 信号量 |  |
| HUAWEI LiteOS |  | 华为 OS | - |
| **Language**      |               |                                     | - |
| Lua               |               |                                     | - |
|  | start | 移植 | ● |
| C++ |  |  | - |
|  |  | 最小工程 | √ |
| **GUI** | |  | - |
|  | EmberGL | 2D / 3D 界面库 | √ |
|  | LittleVGL / LVGL8.3 | 界面库 | ● |
|  | u8g2 | OLED 界面库 | √ |
|  | GuiLite | 界面库 | √ |
|  | ucGUI |  |  |
|  | emWin |  |  |
| **Lib** |  |  | - |
|  | memory | malloc ( sram 模拟 ) | √ |

```
DSP&NN ARM数学库
├─Sin 基本测试
├─Matrix 矩阵运算
├─Conv 卷积
├─FFT 傅里叶变换
├─SVM 支持向量机
├─Gaussian Naive Bayes 高斯贝叶斯分类
├─Biquad Cascade Filter 双二阶级联滤波器
├─Interpolation 插值
├─PID PID控制
├─Kalman Filter 卡尔曼滤波
├─Adaptive Filter 自适应滤波
├─DCT 离散傅里叶变换
├─Clarke 克拉克变化
├─QLearn 强化学习
├─Genetic Algorithm 遗传算法
├─Inductive Learning 归纳学习
├─Ant Colony Optimization 蚁群算法
├─Linear Regression 线性回归
├─Convolutional Neural Network 卷积神经网络CNN
└─Recurrent Neural Network 循环神经网络RNN
```

* 电机控制：FOC、BLDC、SVPWM
* GUI：LVGL，emWin，GuiLite，Pico3D
* 数据结构：链表、队列、堆、栈、哈希表、二分法

----

TODO：

https://gitee.com/zhengnianli/EmbedSummary

https://github.com/zhengnianli/EmbedSummary

## 1、OS

[鸿蒙OS](https://openharmony.gitee.com/openharmony)： 一款面向全场景的开源分布式操作系统。

[RT-Thread](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FRT-Thread)：一款小而美的物联网操作系统。

[TencentOS tiny](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FTencent%2FTencentOS-tiny)：一款面向物联网领域开发的实时操作系统。

[Azure RTOS](https://gitee.com/link?target=https%3A%2F%2Fazure.microsoft.com%2Fzh-cn%2Fservices%2Frtos%2F%23overview)：Azure RTOS 是一个嵌入式开发套件。

[eventos-nano](https://gitee.com/event-os/eventos-nano)：一个超轻量级嵌入式开发框架，事件驱动，占用资源小，可运用于各型单片机。

[zephyr](https://gitee.com/link?target=https%3A%2F%2Fwww.zephyrproject.org%2F)：Zephyr 是 Linux 基金会推出的一个适用于物联网的小型可伸缩的实时操作系统。

[mbed-os](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FARMmbed%2Fmbed-os) ：ARM自己打造、主打IoT的一整套软件解决方案 。

[BabyOS](https://gitee.com/notrynohigh/BabyOS)： 专为MCU项目开发提速的代码框架 。

[LuatOS](https://gitee.com/openLuat/LuatOS) ： LuatOS是运行在嵌入式硬件的实时操作系统，只需要少量内存的flash空间就能运行，用户编写lua代码就可完成各种功能。

[Contiki-OS](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fcontiki-os%2Fcontiki)： 一个小型的，开源的，极易移植的多任务操作系统。

[DJYOS](https://gitee.com/link?target=http%3A%2F%2Fwww.djyos.com%2F)： 都江堰操作系统。

[klite](https://gitee.com/kerndev/klite) ： 简洁易用的嵌入式操作系统内核。

[lmosem](https://gitee.com/lmnos/lmosem) ： 一个完全从第一行引导代码开始编写，基于ARM平台，支持多进程、多CPU、内存管理、文件与设备管理的全32位操作系统内核。

[freenos](https://gitee.com/link?target=http%3A%2F%2Fwww.freenos.org%2F)：FreeNOS微内核操作系统。

[ros](https://gitee.com/zhengnianli/EmbedSummary/blob/master/www.ros.org)：机器人操作系统。

[openwrt](https://gitee.com/link?target=https%3A%2F%2Foldwiki.archive.openwrt.org%2Fzh-cn%2Fdoc%2Fstart)：openwrt文档。

[cola_os](https://gitee.com/schuck/cola_os)：300行代码实现多任务管理的OS。

[MS-RTOS](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FMS-RTOS)： Micro Safe RTOS 。

## 2、实用库 / 框架

[ametal](https://gitee.com/zlgopen/ametal)或[ametal](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fzlgopen%2Fametal)：AMetal是芯片级的裸机软件包，定义了跨平台的通用接口。

[Melon](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FWater-Melon%2FMelon)：一个用于简化开发的 C 框架库。

[zlog](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FHardySimpson%2Fzlog)：一个高可靠性、高性能、纯C日志函数库。

[EasyLogger](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farmink%2FEasyLogger)：一款超轻量级、高性能的 C/C++ 日志库。

[Smartlink](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fjolin90%2Fsmartconfig)：Linux上实现的smartconfig。

[airkissOpen](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fheyuanjie87%2FairkissOpen) ：腾讯airkiss协议解析库 。

[CodeBrick](https://gitee.com/moluo-tech/CodeBrick)：一种无OS的MCU实用软件管理系统。

[RIL](https://gitee.com/moluo-tech/ril)：一款专门为嵌入式平台开发的无线通信模块(GSM/GPRS/CatM1/NB)管理框架。

[cJSON](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FDaveGamble%2FcJSON)：一个基于C语言的轻量级的JSON解析库。

[jsmn](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fzserge%2Fjsmn)：一个小巧的基于C语言的JSON解析库。

[inih](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fbenhoyt%2Finih)： C 语言编写的 INI 文件解析器。

[FlexibleButton](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fmurphyzhao%2FFlexibleButton)：一个基于标准 C 语言的小巧灵活的按键处理库。

[CmBacktrace](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farmink%2FCmBacktrace) ： 一款针对 ARM Cortex-M 系列 MCU 的错误代码自动追踪库。

[EasyLogger](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farmink%2FEasyLogger)：一款超轻量级、高性能的 C/C++ 日志库。

[limlog](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fdianjixz%2Flimlog) ：一款超轻量级、高性能的 C/C++ 日志库。

[NanoLog](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FIyengar111%2FNanoLog) ：一款超轻量级、高性能的 C/C++ 日志库。

[EasyFlash](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farmink%2FEasyFlash)：一款开源的轻量级嵌入式Flash存储器库 。

[SFUD](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farmink%2FSFUD)：一款开源的串行 SPI Flash 通用驱动库。

[lw_oopc](https://gitee.com/link?target=https%3A%2F%2Fsourceforge.net%2Fprojects%2Flwoopc%2F)：轻量级的C语言面向对象编程框架。

[PLOOC](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FGorgonMeducer%2FPLOOC)：受保护的低开销面向对象编程。

[mqttclient](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FjiejieTop%2Fmqttclient)：一个省资源、高稳定的MQTT客户端。

[NorthFrame](https://gitee.com/PISCES_X/NorthFrame) ：一个单片机极简图形化状态机框架 。

[pigweed](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fizzimat%2Fpigweed)：谷歌开源的嵌入式目标库（模块）集合。

[lwrb](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FMaJerle%2Flwrb)：一个轻量级通用环缓冲区管理器库。

[cQueue](https://gitee.com/yorkjia/cQueue) ：使用ANSI C 编写的消息队列功能函数。

[Unity](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FThrowTheSwitch%2FUnity%2Freleases)：一个轻量级的、适合嵌入式的测试框架。

[Embedded Unit](https://gitee.com/link?target=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fembunit)： 是个纯标准c构建的单元测试框架。

[CuTest ](https://gitee.com/link?target=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fcutest%2F)：一款微小的C语言单元测试框， 全部代码加起来不到一千行。

[cmockery](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fgoogle%2Fcmockery)： 谷歌C单元测试框架。

[googletest](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fgoogle%2Fgoogletest)：谷歌C++测试框架。

[znfat ](https://gitee.com/dbembed/znfat?_from=gitee_search)：振南fat，国产嵌入式文件系统方案 。

[libu](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fkoanlogic%2Flibu)：一个C语言写的多平台工具库。

[tbox](https://gitee.com/tboox/tbox)：一个用c语言实现的跨平台开发库。

[toolkit](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fcproape%2Ftoolkit)：ToolKit是一套应用于嵌入式系统的通用工具包。

[LWIP](https://gitee.com/link?target=http%3A%2F%2Fsavannah.nongnu.org%2Fprojects%2Flwip%2F)： 一个小型开源的TCP/IP协议栈 。

[SQLite ](https://gitee.com/link?target=https%3A%2F%2Fwww.sqlite.org%2Fdownload.html)：一个开源的嵌入式关系数据库。

[OpenBLT](https://gitee.com/link?target=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fopenblt%2Ffiles%2F)：一种开源引导加载程序。

[Linux Lab](https://gitee.com/tinylab/linux-lab) ： Linux 内核实验室，基于 Docker/Qemu 的极速 Linux 内核学习、开发和测试环境。

[airkissOpen](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fheyuanjie87%2FairkissOpen) ：腾讯airkiss协议解析库 。

[mbedtls](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FARMmbed%2Fmbedtls) ：一个开源、便携,易于使用,可读的和灵活的SSL库。

[mosquitto](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Feclipse%2Fmosquitto)或[mosquitto(下载速度快)](https://gitee.com/zhengnianli/mosquitto)：一个开源的MQTT代理。

[inih](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fbenhoyt%2Finih)：C 语言编写的 INI 文件解析器。

[QP](https://gitee.com/link?target=http%3A%2F%2Fwww.state-machine.com%2F)：QP实时嵌入式框架。

[MS-RTOS](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FMS-RTOS)：Micro Safe RTOS 。

[protobuf-c](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fprotobuf-c%2Fprotobuf-c%2Fwiki%2FExamples)：protobuf-c。

[eepromfs](https://gitee.com/wtau_zaozao/eepromfs?_from=gitee_search)： 基于EEPROM的简易类文件的数据读写库 。

[gear-lib](https://gitee.com/gozfreee/gear-lib) ： 适用于IOT/嵌入式/网络服务开发的C库 。

[EFSM](https://gitee.com/simpost/EFSM) ： 是一个基于事件驱动的有限状态机 。

[tbox](https://gitee.com/tboox/tbox)：一个用c语言实现的跨平台开发库。

[sys/queue.h](https://gitee.com/link?target=https%3A%2F%2Fcode.woboq.org%2Fuserspace%2Fglibc%2Fmisc%2Fsys%2Fqueue.h.html)：用宏实现的常用数据结构。

[nanomsg](https://gitee.com/link?target=https%3A%2F%2Fnanomsg.org%2F)：一个实现了几种“可扩展协议”的高性能通信库。

[thttpd](https://gitee.com/link?target=http%3A%2F%2Facme.com%2Fsoftware%2Fthttpd%2F)：一个简单，小型，可移植，快速且安全的HTTP服务器。

[boa](https://gitee.com/link?target=http%3A%2F%2Fwww.boa.org%2F)：一个小巧高效的web服务器。

[FreeTpye](https://gitee.com/link?target=https%3A%2F%2Ffreetype.org%2F) ：字体渲染库。

[qpc](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FQuantumLeaps%2Fqpc) ：一个开源的状态机实现。

[Quantum Leaps](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FQuantumLeaps) ：状态机组织。

[ZBar](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FZBar%2FZBar) ：二维码扫描工具和开发包.

[FFmpeg](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FFFmpeg%2FFFmpeg) ：多媒体处理工具库.

[libnabo](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fethz-asl%2Flibnabo) ：是一个快速为低维度空间提供K最近邻居算法库。

[DSAL](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fdheeraj-2000%2Fdsalgo)：是一个经典的数据结构和算法库。

[FLINT](https://gitee.com/link?target=http%3A%2F%2Fwww.flintlib.org%2F) ：是一个数论库，用C语言编写，它包括一个非常快的多项式算法库。

[LibMMSeg](https://gitee.com/link?target=http%3A%2F%2Fwww.oschina.net%2Fp%2Flibmmseg)：中文分词软件包 。

[CDS](https://gitee.com/link?target=http%3A%2F%2Flibcds.sourceforge.net%2F)：CDS是一个 C++ 模板库，包含 lock-free and fine-grained 算法。

[hashlib++](https://gitee.com/link?target=http%3A%2F%2Fhashlib2plus.sourceforge.net%2F)：是一个简单易用的用来生成 checksum 算法。

[libcstl](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Factivesys%2Flibcstl)：C语言编写的一个通用的数据结构和常用的算法库。

[Botan](https://gitee.com/link?target=http%3A%2F%2Fbotan.randombit.net%2F)：Botan 是一个 C++ 的加密算法库。

[CGAL](https://gitee.com/link?target=http%3A%2F%2Fwww.cgal.org%2F)：计算几何算法库。

[MyMediaLite](https://gitee.com/link?target=http%3A%2F%2Fwww.ismll.uni-hildesheim.de%2Fmymedialite%2F)：是一个轻量级的多用途的推荐系统的算法库。

[cblas](https://gitee.com/link?target=http%3A%2F%2Fwww.netlib.org%2Ff2c%2F)：提供c接口的blas库，源码通过f2c转换为C语言。f2c官网。

[clapack](https://gitee.com/zhengnianli/EmbedSummary)：提供c接口的lapack库，源码通过f2c转换为C语言。

[scalapack](https://gitee.com/zhengnianli/EmbedSummary)：提供并行运算的lapack库，源码用fortran编写。

[armadillo](https://gitee.com/zhengnianli/EmbedSummary)：线性代数运算库，源码用c++编写。

[openblas](https://gitee.com/zhengnianli/EmbedSummary)：针对intel cpu优化的blas库。

[mkl](https://gitee.com/zhengnianli/EmbedSummary)：扩展的且针对intel cpu优化的库，包括快速傅立叶变换（FFT）、矢量数学库。

[eigen](https://gitee.com/zhengnianli/EmbedSummary)：C++写的线性代数运算库。

[libjpeg](https://gitee.com/link?target=https%3A%2F%2Flatelee.blog.csdn.net%2Farticle%2Fdetails%2F6918543%3Futm_medium%3Ddistribute.pc_relevant_download.none-task-blog-baidujs-1.nonecase%26depth_1-utm_source%3Ddistribute.pc_relevant_download.none-task-blog-baidujs-1.nonecase)：用libjpeg库在LCD上显示图片。

[x264](https://gitee.com/link?target=https%3A%2F%2Fcode.videolan.org%2Fvideolan%2Fx264)：x264开源的解码编码库。

[ws2812](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fhepingood%2Fws2812b)：ws2812驱动库。

## 3、GUI相关

[yoxios](https://gitee.com/link?target=https%3A%2F%2Fwww.yoxios.com%2F)： 基于Linux开发的轻量级物联网系统和硬件平台。

[ToughGFX](https://gitee.com/link?target=https%3A%2F%2Fwww.touchgfx.com%2Fzh%2F)： 一个C++编写的GUI软件框架 。

[emwin](https://gitee.com/link?target=https%3A%2F%2Fwww.segger.com%2Fproducts%2Fuser-interface%2Femwin%2F)：一个老牌GUI库。

[littlevGL](https://gitee.com/mirrors/lvgl) ： 一个免费的开源图形库。

[野牛 LittlevGL demo](https://gitee.com/mzy2364/LittlevGL_Demo)：基于野牛开发板的 LittlevGL demo 程序 。

[MonoGUI ](https://gitee.com/liuxinouc/MonoGUI)：一个黑白图形用户接口系统 。

[MiniGUI](https://gitee.com/link?target=http%3A%2F%2Fwww.minigui.com%2Fdownload)： 一个快速、稳定、跨操作系统的GUI。

[Gtk](https://gitee.com/link?target=https%3A%2F%2Fwww.gtk.org%2F)： 一个用于创造图形用户接口的图形库。

[AWTK](https://gitee.com/zlgopen/awtk?_from=gitee_search)：是 ZLG 倾心打造的一套基于 C 语言开发的 GUI 框架 。

[玲珑GUI](https://gitee.com/link?target=https%3A%2F%2Fwww.yuque.com%2Fbooks%2Fshare%2F3317aaa7-f47f-4bfd-a4c2-7e64e7f1c4be%3F%23)：玲珑GUI教程页。

[std](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fnothings%2Fstb) ：一个嵌入式小型图形库。

[CUGUI](https://gitee.com/cuihongxi/CUIGUI)：为单片机写的GUI ，支持字体，按键，图片，滑动条。

## 4、项目 / 软硬结合

[ElectronBot](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fpeng-zhihui%2FElectronBot)：一个桌面级小机器人。

[Planck-Pi](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fpeng-zhihui%2FPlanck-Pi)：超迷你Linux开发板。

[qt_2019_ncov](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fwhik%2Fqt_2019_ncov)：基于Qt/C++实现的新冠肺炎疫情监控平台。

[H7-TOOL_STM32H7_App](https://gitee.com/armfly/H7-TOOL_STM32H7_App) ：安富莱H7-TOOL 多功能开发工具。

[the-little-bili-tv](https://gitee.com/wangpeng25/the-little-bili-tv)：基于esp32+lvgl8.0的哔哩哔哩小电视。

[DAPLink/CMSIS DAP](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FARMmbed%2FDAPLink)：一个调试器， 集成下载、调试和USB虚拟串口。

[Crazepony](https://gitee.com/link?target=http%3A%2F%2Fwww.crazepony.com%2F)：Crazepony开源四轴飞行器。

[MiniQ](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fbillhsu%2FMiniQ) ：一个迷你四轴飞行器。

[LiPow-Firmware](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FAlexKlimaj%2FLiPow-Firmware)：基于STM32G0采用USB type-C供电的开源锂电池充电器。

[Avem](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Favem-labs%2FAvem)： 一个轻量级无人机飞控。

[esp8266-lattice-clock](https://gitee.com/lengff/esp8266-lattice-clock-open)：Esp8266多功能点阵时钟。

[串口ISP程序](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fnicekwell%2Fstm32ISP)：一个stm32串口ISP程序。

[DSView](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FDreamSourceLab%2FDSView)： 一个跨平台的逻辑分析仪。

[MCU-Development](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fcjeeks%2FMCU-Development)： 基于51、430、STM32F10X、STM32F407X、T4MC123G平台的各常见硬件模块demo。

[Arduino](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farduino%2FArduino) ： 开源电子原型平台。

[EWAHBoolArray](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Flemire%2FEWAHBoolArray)： bitmap算法。

[mcush](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fpengshulin%2Fmcush)：MCU shell。

[DoST](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fzhangyuhu%2FDoST)： Linux 下开发 STM32 。

[Avem](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Favem-labs%2FAvem) : 这是一个轻量级无人机飞控项目。

## 5、资源 / 工具 / 网站 / 论坛

[100ask download](https://gitee.com/link?target=http%3A%2F%2Fdownload.100ask.org%2F)：百文网资料下载中心。

[armfly](https://gitee.com/link?target=http%3A%2F%2Fwww.armbbs.cn%2Fforum.php)：安富莱/硬汉嵌入式论坛。

[fire download](https://gitee.com/link?target=https%3A%2F%2Febf-products.readthedocs.io%2Fzh_CN%2Flatest%2F)：野火资料下载中心。

[openedv download](https://gitee.com/link?target=http%3A%2F%2Fwww.openedv.com%2Fdocs%2Findex.html)：正点原子资料下载中心。

[野火](https://gitee.com/link?target=https%3A%2F%2Fwww.firebbs.cn%2Fforum.php)：野火电子论坛。

[正点](https://gitee.com/link?target=http%3A%2F%2Fwww.openedv.com%2Fforum.php)：正点原子论坛。

[小熊猫C++ IDE](https://royqh1979.gitee.io/redpandacpp/)：跨平台、轻量易用的开源C/C++集成开发环境。

[ST](https://gitee.com/link?target=https%3A%2F%2Fwww.stmcu.org.cn%2Fmodule%2Fforum%2Fforum.php)：ST中文社区。

[讯为开发社区](https://gitee.com/link?target=http%3A%2F%2Fbbs.topeetboard.com%2Fforum.php)：北京讯为ARM开发社区。

[讯为官网](https://gitee.com/link?target=http%3A%2F%2Fwww.topeetboard.com%2F)：讯为官网。

[NXP社区](https://gitee.com/link?target=https%3A%2F%2Fwww.nxpic.org.cn%2F)：恩智浦论坛。

[Linux](https://gitee.com/link?target=https%3A%2F%2Fwww.kernel.org%2F)：Linux内核官网。

[Linux源码](https://gitee.com/link?target=https%3A%2F%2Felixir.bootlin.com%2Flinux%2Flatest%2Fsource)：可在线阅读Linux内核源码。

[Buildroot](https://gitee.com/link?target=https%3A%2F%2Fbuildroot.org%2Fdownloads%2F)：buildroot源码。

[Debian](https://gitee.com/link?target=https%3A%2F%2Fwww.debian.org%2F)：Debian官网。

[Ubuntu](https://gitee.com/link?target=https%3A%2F%2Fubuntu.com%2F)：Ubuntu官网。

[BOA官网](https://gitee.com/link?target=http%3A%2F%2Fwww.boa.org%2F)：一个单任务 web 服务器 ，可以移植到嵌入式平台。

[荔枝派](https://gitee.com/link?target=https%3A%2F%2Fwww.kancloud.cn%2Flichee%2Flpi0%2F317714)：荔枝派指南。

[清华镜像站](https://gitee.com/link?target=https%3A%2F%2Fmirrors.tuna.tsinghua.edu.cn%2F)：清华大学开源软件镜像站。

[中科大镜像站](https://gitee.com/link?target=https%3A%2F%2Fmirrors.ustc.edu.cn%2F)：中国科技大学镜像站。

[u-boot](https://gitee.com/link?target=https%3A%2F%2Fftp.denx.de%2Fpub%2Fu-boot%2F)：u-boot源码下载页面。

[bear-pi](https://gitee.com/morixinguan/bear-pi)：小熊派demo。

[宅学部落](https://gitee.com/link?target=http%3A%2F%2Fwww.zhaixue.cc%2F)：一个Linux、编程语言、内核、驱动开发学习网站。

[单片机教程网](https://gitee.com/link?target=http%3A%2F%2Fwww.51hei.com%2F)：单片机基础、设计实例、论坛。

[GNU](https://gitee.com/link?target=http%3A%2F%2Fwww.gnu.org%2F)：GNU官网。

[ChinaUnix](https://gitee.com/link?target=http%3A%2F%2Fwww.chinaunix.net%2F)：Linux/Unix相关。

[PyQt5](https://gitee.com/link?target=http%3A%2F%2Fcode.py40.com%2Fface)：PyQt5教程。

[qter](https://gitee.com/link?target=https%3A%2F%2Fwww.qter.org%2F)：QT开源社区。

[git手册](https://gitee.com/link?target=https%3A%2F%2Fgit-scm.com%2Fbook%2Fzh%2Fv2)：git学习手册。

[开源项目风格](https://gitee.com/link?target=https%3A%2F%2Fzh-google-styleguide.readthedocs.io%2Fen%2Flatest%2F%23)：Google 开源项目风格指南 (中文版)。

[电子世家](https://gitee.com/link?target=http%3A%2F%2Fwww.dianzishijia.com%2F)：一个电子类导航网站。

[21ic](https://gitee.com/link?target=https%3A%2F%2Fwww.21ic.com%2F)：21IC中国电子网。

[芯路恒电子](https://gitee.com/link?target=http%3A%2F%2Fwww.corecourse.cn%2Fforum.php)：小梅哥博客、FPGA论坛。

[嵌入式开发者社区](https://gitee.com/link?target=http%3A%2F%2Fwww.51ele.net%2F)：创龙论坛。

[micropython](https://gitee.com/link?target=http%3A%2F%2Fmicropython.openioe.net%2F)：micropython中文网。

[泰晓科技](https://gitee.com/link?target=http%3A%2F%2Ftinylab.org%2Fusing-linux-lab-to-do-embedded-linux-development%2F)：Linux相关。

[电堂科技](https://gitee.com/link?target=https%3A%2F%2Fc.51diantang.com%2F)：STM32相关。

[源代码使用示例聚合器](https://gitee.com/link?target=https%3A%2F%2Fcpp.hotexamples.com%2Fzh%2F)： 从超过100万个开源项目搜索CPP代码示例。

[KST-51 ](https://gitee.com/link?target=http%3A%2F%2Fwww.qdkingst.com%2Fcn)：《手把手教你学51单片机》 。

[原子哥](https://gitee.com/link?target=https%3A%2F%2Fwww.yuanzige.com%2F)：专注电子技术教学。。

[digoboy](https://gitee.com/link?target=http%3A%2F%2Fwww.digoboy.com%2F)：地瓜派视频网。

[c.biancheng](https://gitee.com/link?target=http%3A%2F%2Fc.biancheng.net%2F)：C语言中文网。

[open-c-book](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Ftinyclub%2Fopen-c-book) ：《C语言编程透视》。

[PyQt5](https://gitee.com/link?target=http%3A%2F%2Fcode.py40.com%2F)：PyQt5在线教程。

[open-shell-book](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Ftinyclub%2Fopen-shell-book)：《Shell 编程范例》 。

[software_unit_test](https://gitee.com/link?target=https%3A%2F%2Fwww.zlg.cn%2Ffoxmail%2Fweixinpdf%2Fsoftware_unit_test.pdf)： 《软件单元测试入门与实践》。

[kerneltravel](https://gitee.com/link?target=http%3A%2F%2Fwww.kerneltravel.net%2F)：Linux内核之旅。

[蜗窝科技](https://gitee.com/link?target=http%3A%2F%2Fwww.wowotech.net%2F)：慢下来，享受技术。

[bookstack](https://gitee.com/link?target=https%3A%2F%2Fwww.bookstack.cn%2F)：书栈网。

[wireshark](https://gitee.com/link?target=https%3A%2F%2Fwww.wireshark.org%2F%23download)：抓包工具下载链接。

[MobaXterm](https://gitee.com/link?target=https%3A%2F%2Fmobaxterm.mobatek.net%2Fdownload.html)：一个好用的终端软件。

[Stduino](https://gitee.com/link?target=http%3A%2F%2Fwww.stduino.com%2Fpindex.php)： 一款面向32位处理器快速入门学习的集成开发平台 。

[easyicon](https://gitee.com/link?target=https%3A%2F%2Fwww.easyicon.net%2F)：一个免费图标下载网站。

[codingdict](https://gitee.com/link?target=https%3A%2F%2Fcodingdict.com%2F)：一个类似于菜鸟教程的编程类教程网站。

[codingdict开源软件](https://gitee.com/link?target=https%3A%2F%2Fcodingdict.com%2Fos%2F)：codingdict网站收集的各类开源软件集合。

[C经典示例](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FMzzopublic%2FC%2Ftree%2Fmaster%2F%E7%BB%8F%E5%85%B8%E7%A4%BA%E4%BE%8B)：一些C语言的实例。

[xmake](https://gitee.com/tboox/xmake)：轻量级跨平台C/C++构建工具。

[spacevim](https://gitee.com/link?target=https%3A%2F%2Fspacevim.org%2Fcn%2F)：一个模块化的 Vim IDE。

[vofa+](https://gitee.com/link?target=https%3A%2F%2Fwww.vofa.plus%2F)：一个插件驱动的高自由度上位机。

[联合开发网](https://gitee.com/link?target=http%3A%2F%2Fwww.pudn.com%2F)：356万个编程源码资料

[觅思文档](https://gitee.com/link?target=https%3A%2F%2Fmrdoc.pro%2F)：私有云笔记、云文档和知识库系统。

[aardio](https://gitee.com/link?target=https%3A%2F%2Fwww.aardio.com%2F)：易用性极强的动态语言。

[OneNET SDK](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fcm-heclouds)：OneNET SDK仓库。

[ros手册](https://gitee.com/link?target=http%3A%2F%2Fdocs.ros.org%2Fen%2Frolling%2F)：ros的官方手册。

[gnu的开源软件](https://gitee.com/link?target=https%3A%2F%2Fzh.opensuse.org%2F%E5%B8%B8%E7%94%A8%E8%BD%AF%E4%BB%B6)：gnu的开源软件

[soft-and-hard](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Falwxkxk%2Fsoft-and-hard)：这是一个以物联网项目为主方向分享web开发教程。

[FireflyTeam ](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FFireflyTeam)：一个开放源代码的仓库。

[python脚本](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fsmilejay%2Fpython)：一些好用的python脚本。

[C++ libraries](https://gitee.com/link?target=https%3A%2F%2Fen.cppreference.com%2Fw%2Fcpp%2Flinks%2Flibs)：A list of open source C++ libraries.

[Lindenis](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Flindenis-org)：An open source software for Lindenis SBC。

[fast-line-following](https://gitee.com/link?target=https%3A%2F%2Fwww.a1k0n.net%2F2018%2F11%2F13%2Ffast-line-following.html)：一个快速寻线机器人的算法。

[深度神经网络可视化工具](https://gitee.com/link?target=https%3A%2F%2Fblog.csdn.net%2Fdcxhun3%2Farticle%2Fdetails%2F77746550)：深度神经网络可视化工具。

[Awesome-Embedded](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fnhivp%2FAwesome-Embedded) ：汇聚了各种嵌入式相关的资源。

[FreeModbus](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Farmink%2FFreeModbus_Slave-Master-RTT-STM32) : FreeModbus是一款开源的Modbus协议栈，但是只有从机开源。

更多资源敬请期待......

## 6、一些芯片原厂代码仓库

[STMicroelectronics](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FSTMicroelectronics)：意法半导体（ST）。

[TI](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fti-simplelink)：德州仪器（TI）。

[NXP](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FNXP)：恩智浦（NXP）。

[Freescale](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FFreescale)：飞思卡尔半导体（Freescale）。

[hisilicon](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fhisilicon)：海思。

[rockchip](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Frockchip-linux)：瑞芯微。

[Samsung](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FSamsung)：三星。

[Infineon](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FInfineon)：英飞凌。

[analogdevicesinc](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fanalogdevicesinc)：亚德诺半导体（ADI）。

[MicrochipTech](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FMicrochipTech)：微芯半导体（Microchip ）。

[NordicSemiconductor](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FNordicSemiconductor)：北欧集成电路（Nordic）。

[cypress-io](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fcypress-io)：赛普拉斯半导体（Cypress）。

[GD32](https://gitee.com/link?target=http%3A%2F%2Fwww.gd32mcu.com%2Fcn%2Fdownload%2F7)：易兆创新(GD)。

[GD32网盘](https://gitee.com/link?target=http%3A%2F%2Fpan.baidu.com%2Fs%2F1nuifedz)：：易兆创新(GD)网盘资料。

更多资源敬请期待......

## 7、物联网、智能家居

[Domoticz ](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2Fdomoticz%2Fdomoticz)：一个开源的智能家居系统 。

[Kaa IoT Platform](https://gitee.com/link?target=https%3A%2F%2Fwww.kaaproject.org%2F)：功能丰富的开放和高效的物联网云平台。

[RT-Thread IoT SDK ](https://gitee.com/link?target=https%3A%2F%2Fgithub.com%2FRT-Thread%2FIoT_Board)：基于RT-Thread IOT开发板的各类例程。

[从零打造物联网](https://gitee.com/link?target=https%3A%2F%2Fwww.scaugreen.cn%2Fposts%2F44755%2F)：一份适合嵌入式开发人员学习的web开发教程。

更多资源敬请期待......

## 8、机器人相关 / 定位 / 视觉

[RoboCar](https://gitee.com/cv_team/uestc-careye)：机器人自动寻线、避障、建图、导航、遥控版本/车载电脑版本路面分析，及交通路况识别的车辆辅助驾驶系统

[ROS教程](https://gitee.com/link?target=http%3A%2F%2Fwiki.ros.org%2Fcn%2FROS%2FTutorials)：机器人操作系统

[slam](https://gitee.com/link?target=https%3A%2F%2Fwww.slamtec.com%2F)：思岚科技。

## 9、推荐博客 / 博文

[嵌入式大杂烩周记 | 第 11 期 aardio](https://gitee.com/link?target=https%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247508249%26idx%3D1%26sn%3Dd9ec13b5ffd8696c33d16f852129b4c7%26chksm%3Dfe0ecfdec97946c830fae7314a4ff1f83ffd949d8aca89036dbee3c285885e280870b07840e9%26token%3D1729383283%26lang%3Dzh_CN%23rd)

[嵌入式大杂烩周记 | 第 10 期 inih](https://gitee.com/link?target=https%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247507990%26idx%3D1%26sn%3Dc521f8761a659392c88943488987129e%26chksm%3Dfe0eced1c97947c7ffc3fab6de185a5059fa790f892895656d3236affd41baa061e606d09e70%26token%3D1729383283%26lang%3Dzh_CN%23rd)

[嵌入式大杂烩周记 | 第 9 期 nanopb](https://gitee.com/link?target=https%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247507711%26idx%3D1%26sn%3Dff20087065e5ff65ad42437adb41ee3e%26chksm%3Dfe0eb038c979392eb6fc6c50bf2b2b3fffce3e9c689a1f702dcb14c7045b25786b60960373d3%26token%3D1729383283%26lang%3Dzh_CN%23rd)

[嵌入式大杂烩周记 | 第 8 期 AMetal](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247507328%26idx%3D1%26sn%3Dbe02dff353abab76e6d277a1d8fe9ec0%26chksm%3Dfe0eb347c9793a51e84365642d65af23720e27b855dc257ebbecb94d1a331b54c4e0eedd3285%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 7 期 zlog](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247507038%26idx%3D1%26sn%3D9fa92d6e0ceeb207e732fdd37943d581%26chksm%3Dfe0eb299c9793b8f2c698ae579eb9d8d80f7be4b852ea805c1dfe519fabbe6de363f25aa150f%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 6 期 FlexibleButton](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247506418%26idx%3D1%26sn%3D7d3002a4d2821dfee59eba97caaa6aae%26chksm%3Dfe0eb735c9793e2313e6418f48ff96ecd887e934f7c09178822475adf505241193a6cffe19ab%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 5 期 smartlink](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247506144%26idx%3D1%26sn%3D8d6ca3a50341d6147eaba41ab0da8eb6%26chksm%3Dfe0eb627c9793f31ee01411d6b1a6cd7331610d24aefd6c697e540ea26262b91fd3c00e619c8%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 4 期 cola_os](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247505918%26idx%3D1%26sn%3D8782c2fed223af07f77ff11c1e02d4a9%26chksm%3Dfe0eb939c979302fc69171a6786bfed007673d689a7ff9836dc1702f3585b4d796b0bb5bb19d%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 3 期 sys/queue.h](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247505803%26idx%3D1%26sn%3D78714f6c35c3df080d4a646cd3f9e9ef%26chksm%3Dfe0eb94cc979305aec86d41c9d532b83d49825f543d340d846e856737ee8927b9fab3c299033%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 2 期 llgui](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247505561%26idx%3D1%26sn%3D44e1ba8b99a2c312212e079c7f533208%26chksm%3Dfe0eb85ec979314852a2e0e6ec335287c6fe597e50428005b3d6b6f57cd199c97139863eb9ee%26scene%3D21%23wechat_redirect)

[嵌入式大杂烩周记 | 第 1 期 gear-lib](https://gitee.com/link?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzU5MzcyMjI4MA%3D%3D%26mid%3D2247505251%26idx%3D1%26sn%3Df3ca02a6538af599e7705af5133ba40d%26chksm%3Dfe0ebba4c97932b2530651671bcde2d7b1e76b25c37e58fab0f4010b0949a2764303cfcfd289%26scene%3D21%23wechat_redirect)

[博文——Tslib移植与分析](https://gitee.com/link?target=https%3A%2F%2Fblog.csdn.net%2Fwater_cow%2Farticle%2Fdetails%2F7215308%3Futm_medium%3Ddistribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-2.control%26depth_1-utm_source%3Ddistribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-2.control)

[博主——ychy](https://gitee.com/link?target=https%3A%2F%2Fychy.me%2F)