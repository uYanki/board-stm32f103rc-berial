[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver INA219

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ina219/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

INA219 是一款具备 I2C 或 SMBUS 兼容接口的分流器和功率监测计。该器件监测分流器电压降和总线电源电压，转换次数和滤波选项可通过编程设定。可编程校准值与内部乘法器相结合，支持直接读取电流值（单位：安培）。通过附加乘法寄存器可计算功率（单位：瓦）。I2C 或 SMBUS 兼容接口 具有 16 个可编程地址。INA219 可在 0V 至 26V 范围内感测总线中的分压。该器件由 3V 至 5.5V 单电源供电，电源的最大流耗为 1mA。该设备可用于服务器、通信设备、笔记本、电源管理、充电器、电焊、电源和测试设备等。

LibDriver INA219是LibDriver推出的INA219的全功能驱动，该驱动提供连续模式读取、单次模式读取，电流采集、电压采集和功率采集等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver INA219的源文件。

/interface目录包含了LibDriver INA219与平台无关的IIC总线模板。

/test目录包含了LibDriver INA219驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver INA219编程范例。

/doc目录包含了LibDriver INA219离线文档。

/datasheet目录包含了INA219数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，/interface目录和/example目录加入工程。

### 使用

#### example basic

```C
#include "driver_ina219_basic.h"

uint8_t res;

res = ina219_basic_init(INA219_ADDRESS_0, 0.1);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < times; i++)
{
    float mV;
    float mA;
    float mW;

    res = ina219_basic_read(&mV, &mA, &mW);
    if (res != 0)
    {
        (void)ina219_basic_deinit();

        return 1;
    }

    ina219_interface_debug_print("ina219: %d/%d.\n", i + 1, times);
    ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", mV);
    ina219_interface_debug_print("ina219: current is %0.3fmA.\n", mA);
    ina219_interface_debug_print("ina219: power is %0.3fmW.\n", mW);
    ina219_interface_delay_ms(1000);
    
    ...
}

...

(void)ina219_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_ina219_shot.h"

uint8_t res;

res = ina219_shot_init(INA219_ADDRESS_0, 0.1);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < times; i++)
{
    float mV;
    float mA;
    float mW;

    res = ina219_shot_read(&mV, &mA, &mW);
    if (res != 0)
    {
        (void)ina219_shot_deinit();

        return 1;
    }

    ina219_interface_debug_print("ina219: %d/%d.\n", i + 1, times);
    ina219_interface_debug_print("ina219: bus voltage is %0.3fmV.\n", mV);
    ina219_interface_debug_print("ina219: current is %0.3fmA.\n", mA);
    ina219_interface_debug_print("ina219: power is %0.3fmW.\n", mW);
    ina219_interface_delay_ms(1000);
    
    ...
}

...

(void)ina219_shot_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/ina219/index.html](https://www.libdriver.com/docs/ina219/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。