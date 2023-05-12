/**
 ******************************************************************************
 * @file    blood.c
 * @author  张东
 * @version V1.0.0
 * @date    2019-12-28
 * @brief   血氧图示仪主程序部分
 ******************************************************************************
 */
/*--Include-start-------------------------------------------------------------*/
#include "blood.h"
#include "MAX30100.h"
#include "ST7735.h"
#include "algorithm.h"
#include "beep.h"
#include "gui.h"
#include "math.h"
#include "stdio.h"
/*Global data space ----------------------------------------------------------*/
uint16_t g_fft_index = 0;  // fft输入输出下标

struct compx s1[FFT_N + 16];  // FFT输入和输出：从S[1]开始存放，根据大小自己定义
struct compx s2[FFT_N + 16];  // FFT输入和输出：从S[1]开始存放，根据大小自己定义

struct
{
    float Hp;    // 血红蛋白
    float HpO2;  // 氧合血红蛋白

} g_BloodWave;  // 血液波形数据

BloodData g_blooddata = {0};  // 血液数据存储

#define CORRECTED_VALUE 47  // 标定血液氧气含量

/*funcation start ------------------------------------------------------------*/

/*Sensor func -----------------------------------------------------------------*/
// 血液检测信息更新
void blood_data_update(void) {
    uint16_t temp_num = 0;
    uint16_t fifo_word_buff[1][2];

    temp_num = max30100_Bus_Read(INTERRUPT_REG);

    // 标志位被使能时 读取FIFO
    if (INTERRUPT_REG_A_FULL & temp_num) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
        // 读取FIFO
        max30100_FIFO_Read(0x05, fifo_word_buff, 1);  // read the hr and spo2 data form fifo in reg=0x05

        // 将数据写入fft输入并清除输出
        for (int i = 0; i < 1; i++) {
            if (g_fft_index < FFT_N) {
                s1[g_fft_index].real = fifo_word_buff[i][0];
                s1[g_fft_index].imag = 0;
                s2[g_fft_index].real = fifo_word_buff[i][1];
                s2[g_fft_index].imag = 0;
                g_fft_index++;
            }
        }
        // 信息更新标志位
        g_blooddata.update++;
    } else {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
    }
}
// 血液信息转换
void blood_data_translate(void) {
    // 缓冲区写入结束
    if (g_fft_index >= FFT_N) {
        // 开始变换显示
        Gui_DrawFont_GBK16(102, 2, BLACK, GREEN, "FFT");

        g_fft_index = 0;

        // 数据更新标志位
        g_blooddata.display = 1;

        // 快速傅里叶变换
        FFT(s1);
        FFT(s2);

        // 解平方
        for (int i = 0; i < FFT_N; i++) {
            s1[i].real = sqrtf(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag);
            s2[i].real = sqrtf(s2[i].real * s2[i].real + s2[i].imag * s2[i].imag);
        }

        // 读取峰值点 结果的物理意义为
        uint16_t s1_max_index = find_max_num_index(s1, 60);
        uint16_t s2_max_index = find_max_num_index(s2, 60);

        // 检查HbO2和Hb的变化频率是否一致
        if (s1_max_index == s2_max_index) {
            // 心率计算
            uint16_t Heart_Rate = 60 * SAMPLES_PER_SECOND * s2_max_index / FFT_N;

            g_blooddata.heart = Heart_Rate - 10;

            // 血氧含量计算
            float sp02_num = (s2[s1_max_index].real * s1[0].real) / (s1[s1_max_index].real * s2[0].real);

            sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;

            g_blooddata.SpO2 = sp02_num;

            // 状态正常
            g_blooddata.state = BLD_NORMAL;
        } else  // 数据发生异常
        {
            g_blooddata.heart = 0;
            g_blooddata.SpO2  = 0;
            g_blooddata.state = BLD_ERROR;
        }
        // 结束变换显示
        Gui_DrawFont_GBK16(102, 2, GREEN, BLACK, "FFT");
    }
}

// 血液数据更新
void blood_wave_update(void) {
    static DC_FilterData hbdc  = {.w = 0, .init = 0, .a = 0};
    static DC_FilterData hbodc = {.w = 0, .init = 0, .a = 0};

    float hbag  = 0;
    float hboag = 0;

    float hbag_d  = 0;
    float hboag_d = 0;

    // 前8次求平均值
    for (int i = 0; i < 8; i++) {
        hbag += s1[g_fft_index - 8 + i].real;
        hboag += s2[g_fft_index - 8 + i].real;
    }

    // 直流滤波
    hbag_d  = dc_filter(hbag, &hbdc) / 8;
    hboag_d = dc_filter(hboag, &hbodc) / 8;

    // 高度数据
    float hbhight  = 0;
    float hbohight = 0;

    // 比例与偏置
    hbhight  = (-hbag_d / 40.0) + 5;
    hbohight = (-hboag_d / 40.0) + 5;

    // 高度数据幅度限制
    hbhight = (hbhight > 27) ? 27 : hbhight;
    hbhight = (hbhight < 0) ? 0 : hbhight;

    hbohight = (hbohight > 27) ? 27 : hbohight;
    hbohight = (hbohight < 0) ? 0 : hbohight;

    // 将数据发布到全局
    g_BloodWave.Hp   = hbhight;
    g_BloodWave.HpO2 = hbohight;

    // 血液状态检查
    if (hbag < 5000 || hbag < 5000) {
        // 清空数据
        g_blooddata.heart = 0;
        g_blooddata.SpO2  = 0;

        // 删除数据
        g_fft_index -= 7;

        if (g_blooddata.state != BLD_ERROR) {
            g_blooddata.state   = BLD_ERROR;
            g_blooddata.display = 1;
        }
    } else {
        if (g_blooddata.state == BLD_ERROR) {
            g_blooddata.state   = BLD_NORMAL;
            g_blooddata.display = 1;
        }
    }
}
/*tft display ---------------------------------------------------------------*/

// 测试显示血液信息
void tft_test_display(void) {
    uint8_t str[50];

    if (g_blooddata.display == 1) {
        g_blooddata.display = 0;

        // 显示血氧信息
        sprintf((char*)str, "heart = %3d", g_blooddata.heart);
        Gui_DrawFont_GBK16(8, 8, 0x00FF, BLACK, str);

        // 显示心率信息
        sprintf((char*)str, "SpO2 = %3.1f", g_blooddata.SpO2);
        Gui_DrawFont_GBK16(8, 26, 0x00FF, BLACK, str);

        // 显示状态信息
        if (g_blooddata.state) {
            sprintf((char*)str, "ERROR     ");
            Gui_DrawFont_GBK16(8, 44, 0xF000, BLACK, str);
        } else {
            sprintf((char*)str, "NORMAL    ");
            Gui_DrawFont_GBK16(8, 44, 0x07E0, BLACK, str);
        }
    }
}

// 状态信息显示
void tft_draw_State(uint8_t st) {
    switch (st) {
        case BLD_NORMAL: {
            tft_draw_frame(GRAY1);
            Gui_DrawFont_GBK16(3, 2, 0x001F, GRAY3, "Normal    ");
            break;
        }
        case BLD_ERROR: {
            tft_draw_frame(RED);
            Gui_DrawFont_GBK16(3, 2, 0xF000, GRAY3, "Caution   ");
            break;
        }
    }
}

// 绘制波形线段
void tft_draw_wave_line(uint8_t p, uint8_t n, uint16_t h0, uint16_t h1, uint16_t color) {
    uint16_t x = 4;                    // x 起始坐标
    uint16_t y = (p == 0) ? 122 : 89;  // y 起始坐标

    // 擦除线
    if ((n + 1) < 82) {
        Gui_DrawLine((x + n + 2), y - 28, (x + n + 2), y + 1, GRAY2);
        Gui_DrawLine((x + n + 1), y - 28, (x + n + 1), y + 1, BLACK);
    }
    // 绘制线
    Gui_DrawLine((x + n), (y - h0), (x + n + 1), (y - h1), color);
}

// 绘制波形图表
void tft_draw_wave(void) {
    static uint16_t hist_n = 0;  // 同步历史序号

    static uint16_t hbhist_h  = 0;  // 血红蛋白历史数值
    static uint16_t hbohist_h = 0;  // 氧合血红蛋白历史数值

    // 绘制线
    tft_draw_wave_line(1, hist_n, hbhist_h, g_BloodWave.Hp, 0x8FF0);
    tft_draw_wave_line(0, hist_n, hbohist_h, g_BloodWave.HpO2, 0xFC1F);

    // 移动下标
    hist_n = (hist_n < 81) ? (hist_n + 1) : 0;

    // 保存历史
    hbhist_h  = g_BloodWave.Hp;
    hbohist_h = g_BloodWave.HpO2;
}

// 心率血氧数据刷新
void tft_draw_hrsp(void) {
    uint8_t str[50];
    // 心率信息显示
    sprintf((char*)str, "%3d  ", g_blooddata.heart);
    Gui_DrawFont_GBK16(40, 20, 0xffe0, GRAY3, str);
    // 血氧饱和度显示
    g_blooddata.SpO2 = (g_blooddata.SpO2 > 99.99) ? 99.99 : g_blooddata.SpO2;
    sprintf((char*)str, " %2.2f%%  ", g_blooddata.SpO2);
    Gui_DrawFont_GBK16(40, 38, 0x07ff, GRAY3, str);
}

// tft显示刷新
void tft_display_update(void) {
    // 图表更新
    if (g_blooddata.update >= 8) {
        // 清除图标更新标志位
        g_blooddata.update = 0;
        // 血液波形数据更新
        blood_wave_update();
        // 绘制波形
        tft_draw_wave();
    }

    // 转换后的数据更新
    if (g_blooddata.display >= 1) {
        // 清除更新标志位
        g_blooddata.display = 0;
        // 显示血液状态信息
        tft_draw_State(g_blooddata.state);
        // 心率血氧数据刷新
        tft_draw_hrsp();
    }
}
/*Beep and LED func -------------------------------------------------------*/

// LED和蜂鸣器状态更新
void Led_beep_update(void) {
    static uint32_t   errorStartTick = 0;           // 错误发生点
    static BloodState blhist         = BLD_NORMAL;  // 历史状态
    static uint8_t    beepstate      = 0;           // 蜂鸣器状态

    switch (g_blooddata.state) {
        case BLD_NORMAL: {
            // LED指示灯
            Led_Control((g_BloodWave.Hp > 10) * 20);
            // 蜂鸣器
            Beep_Control((g_BloodWave.HpO2 > 10) * 50);
            break;
        }
        case BLD_ERROR: {
            if (blhist == BLD_NORMAL) {
                errorStartTick = HAL_GetTick();
            }

            // 错误前0-3秒 快速闪烁
            if ((HAL_GetTick() - errorStartTick) < 3000) {
                if (((HAL_GetTick() / 100) %
                     (4 - (HAL_GetTick() - errorStartTick) / 1000)) == 0) {
                    Led_Control(20);
                    Beep_Control(50);
                } else {
                    Led_Control(0);
                    Beep_Control(0);
                }

            }
            // 错误前3-6秒 常亮
            else if (3000 < (HAL_GetTick() - errorStartTick) &&
                     6000 > (HAL_GetTick() - errorStartTick)) {
                Led_Control(20);
                Beep_Control(50);
            }
            // 错误到6秒结束
            else if ((HAL_GetTick() - errorStartTick) > 6000) {
                Led_Control(0);
                Beep_Control(0);
            }
            break;
        }
        default: break;
    }

    blhist = g_blooddata.state;
}

/*Setup and loop func -----------------------------------------------------*/

void blood_Setup(void) {
    // 初始化血液信息
    g_blooddata.heart   = 0;
    g_blooddata.SpO2    = 0;
    g_blooddata.display = 1;

    // 绘制窗口
    tft_draw_windows();

    // 初始化指示灯和蜂鸣器
    settone(14);
}

void blood_Loop(void) {
    // 血液信息获取
    blood_data_update();
    // 血液信息转换
    blood_data_translate();
    // tft显示刷新
    tft_display_update();
    // LED 蜂鸣器信息更新
    Led_beep_update();
}

void blood_Interrupt(void) {
    static int16_t div  = 0;
    static int16_t div2 = 0;
    div++;
    if (div > 10) {
        div = 0;
        // 更新血液信息
        // blood_data_update();
    }

    div2++;
    if (div > 50) {
        div2 = 0;
        // tft显示刷新
        // tft_display_update();
    }
}
