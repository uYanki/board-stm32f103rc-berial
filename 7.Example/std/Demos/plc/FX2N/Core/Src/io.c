#include "abs_addr.h"

u16        in_x, out_y, M00;
extern u16 all_data[16600] __at(0x20005002);

void io_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  // 失能（JTAG+SW-DP）释放I/O

    // define Y00-Y08
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // define Y10-Y17
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // define X00-X13
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;  // EEPROM
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void x_filter(void)  // 每 1ms 需调用一次，用于X滤波，暂定为20MS
{
    static int8_t x_buffer[16];  // 暂定刷新16个X

    uint8_t i, bit;

    for (i = 0; i < 16; ++i) {
        switch (i) {
            //
            case 0: bit = X00; break;
            case 1: bit = X01; break;
            case 2: bit = X02; break;
            case 3: bit = X03; break;
            case 4: bit = X04; break;
            case 5: bit = X05; break;
            case 6: bit = X06; break;
            case 7: bit = X07; break;
            //
            case 8: bit = X10; break;
            case 9: bit = X11; break;
            case 10: bit = X12; break;
            case 11: bit = X13; break;
            case 12: bit = X14; break;
            case 13: bit = X15; break;
            case 14: bit = X16; break;
            case 15: bit = X17; break;
        }

        bit = !!!bit;

        if (bit) {
            if (x_buffer[i] < 10) {
                x_buffer[i]++;
            } else {
                in_x |= (1 << i);
            }
        } else {
            if (x_buffer[i] > -10) {
                x_buffer[i]--;
            } else {
                in_x &= ~(1 << i);
            }
        }
    }
}

void x_refresh(void)  // 刷新输入点共4点
{
    all_data[0x240 / 2] &= 0x0000;  // 清除有外部输入的X
    all_data[0x240 / 2] |= in_x;    // 更新外部输入X数据
    // CH-X00:all_data[0x240/2]----all_data[0x240/2+15]	   16组
    // CH-Y00:all_data[0x180/2]----all_data[0x180/2+15]	   16组
    all_data[0x2000 + 500] = M00++;  // D500
    all_data[0x0500 + 100] = M00;    // C100
    all_data[0x0800 + 100] = M00;    // T100
}

// 输出
void y_refresh(void)
{
    out_y = all_data[0x180 / 2];

    //
    Y00 = !(out_y & 0x0001);
    Y01 = !(out_y & 0x0002);
    Y02 = !(out_y & 0x0004);
    Y03 = !(out_y & 0x0008);
    Y04 = !(out_y & 0x0010);
    Y05 = !(out_y & 0x0020);
    Y06 = !(out_y & 0x0040);
    Y07 = !(out_y & 0x0080);
    //
    Y10 = !(out_y & 0x0100);
    Y11 = !(out_y & 0x0200);
    Y12 = !(out_y & 0x0400);
    Y13 = !(out_y & 0x0800);
    Y14 = !(out_y & 0x1000);
    Y15 = !(out_y & 0x2000);
    Y16 = !(out_y & 0x4000);
    Y17 = !(out_y & 0x8000);
}

void init_xy(void)
{
    io_config();
    y_refresh();
}
