/*abs_addr*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stm32f10x_flash.h"
#include <stdio.h>
#include <absacc.h>
#include <io.h>
int print_time __attribute__ ((section ("ER_RSV")));
//指定文件内所以变量不被初始化：
#pragma arm section code ="ER_RSV", rwdata = "ER_RSV"
int abc;
int systick;
int sys_var1;
#pragma arm section code, rwdata

LR_IROM1 0x08000000 0x00020000 
{ ; load region size_region
 ER_IROM1 0x08000000 0x00020000 
{ ; load address = execution address
*.o (RESET, +First)
*(InRoot$$Sections)
.ANY (+RO)
}

RW_IRAM1 0x20000000 0x00004000 { ; RW data
.ANY (+RW +ZI)
}

OVERLAY_RAM1 0x20004000 OVERLAY {
*.o (ER_RSV) ;//所有模块的指定段为"ER_RSV"的变量放这里
}

}
