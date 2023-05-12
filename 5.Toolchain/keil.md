**Keil、uVision、MDK、Keil C51 之间的区别比较**

**KEIL**是**公司的名称**，有时候也指KEIL公司的所有软件开发工具，目前2005年Keil被ARM公司收购，成为ARM的公司之一。

**uVision**是KEIL公司开发的一个**集成开发环境**（IDE），和Eclipse类似。它包括工程管理，源代码编辑，编译设uVision置，下载调试和模拟仿真等功能，uVision有uVision2、uVision3、uVision4、uVision5四个版本，目前最新的版本是uVision5。它提供一个环境，让开发者易于操作，并不提供能具体的编译和下载功能，需要软件开发者添加。uVisionu通用于KEIL的开发工具中，例如MDK，PK51，PK166，DK251等。目前使用Keil uVision4的产品有Keil MDK-ARM，Keil C51，Keil C166和Keil C251

**MDK**（Microcontroller Development Kit），也称**MDK-ARM**、**KEIL MDK**、**RealView MDK**、**KEIL For ARM**，都是同一个东西。ARM公司现在统一使用MDK-ARM的称呼，MDK的设备数据库中有很多厂商的芯片，是专为微控制器开发的工具，为满足基于MCU进行嵌入式软件开发的工程师需求而设计，支持ARM7，ARM9，Cortex-M4/M3/M1，Cortex-R0/R3/R4等ARM微控制器内核。

**KEIL C51**，亦即**PK51**，KEIL公司开发的基于uVision IDE，支持绝大部分8051内核的微控制器开发工具。

因此，如图C51是51系列单片机开发工具，MDK是arm开发工具。如果想两个都能用就必须两个都得装。