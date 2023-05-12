/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "math.h"
#include "apps/base.h"
// #include "apps/menulist.h"
// #include "apps/iconlist.h"
// #include "apps/textedit.h"
// #include "apps/adcchart.h"

#define CONFIG_PADDING_X 4

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#if 0

typedef enum {
    PAGE_ID_NONE = -1,
    //
    PAGE_ID_FIRST = 0,

    PAGE_ID_MAIN_MENULIST = PAGE_ID_FIRST,
    PAGE_ID_PID_MENULIST,
    PAGE_ID_DEVS_MENULIST,
    PAGE_ID_ICON_ICONLIST,

    PAGE_ID_LOGO,
    PAGE_ID_ADC_CHART,
    PAGE_ID_TEXTEDIT,
    PAGE_ID_ABOUT,
    PAGE_ID_EASING_CHART,

    PAGE_ID_LAST,
    //
    FUNC_ID_RETURN = PAGE_ID_LAST + 1,
    FUNC_ID_ANIMATION1,
    FUNC_ID_ANIMATION2,
    FUNC_ID_ANIMATION3,
    FUNC_ID_ANIMATION4,
} page_id_t;

page_t*   pages[PAGE_ID_LAST] = {0};
page_id_t page_selected;  // index of rendering

// 注册
#define PAGE_REGISTER(ID, page) pages[ID] = (page_t*)&page
// 切换
#define PAGE_SWITCH(ID)                 \
    if (pages[page_selected = ID] != 0) \
    pages[page_selected = ID]->bRepaint = true
// 渲染
#define PAGE_RENDER()                                            \
    pages[page_selected]->lpfnHandler(pages[page_selected]);     \
    if (pages[page_selected]->bRepaint) {                        \
        println("repaint");                                      \
        pages[page_selected]->bRepaint = 0;                      \
        pages[page_selected]->lpfnPainter(pages[page_selected]); \
    }

///

void _painter_logo(page_t* p)
{
    _clear();
    _set_color(0);
    _draw_str(CONFIG_PADDING_X, 12, "Hello World");
    _set_color(1);
    _draw_str(CONFIG_PADDING_X, 24, "Hello World");
    _set_color(2);
    _draw_str(CONFIG_PADDING_X, 36, "Hello World");
    _update();
}

void _handler_logo(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

void _handler_chart(adc_chart_t* p)
{
    println("cus");

    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            adcchart_repaint_border(p);
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

void _handler_textedit(texteditor_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: {
            texteditor_callback_handle_set_prevchar(p);
            break;
        }
        case KEY_ID_NEXT: {
            texteditor_callback_handle_set_nextchar(p);
            break;
        }
        case KEY_ID_OK: {
            if (!texteditor_callback_handle_switch_editmode(p)) {
                effect_disappear();
                PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            }
            break;
        }
    }
}

void _painter_about(page_t* p)
{
    _clear();
    _set_color(1);
    _draw_str(2, 12, "MCU : STM32");
    _draw_str(2, 28, "FLASH : 512KB");
    _draw_str(2, 44, "SRAM : 256KB");
    _draw_str(2, 60, "RTC SRAM : 16KB");
    _update();
}

void _handler_about(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

#define _PI2 1.57
#define _PI  3.14
#define _2PI 6.28

void effect_win10_loading()
{
    uint8_t r   = 16;
    uint8_t cnt = 6;  // number of points

    easing_t e = easing_create(EASING_MODE_NTIMES(2) | EASING_DIR_FORWARD, _easing_calc_Linear, 0, 15, 30);

    easing_start_absolute(&e, 0, 1);  // generate linear time order

    while (!easing_isok(&e)) {
        easing_update(&e);
        _clear();
        for (uint8_t i = 0; i < cnt; ++i) {
            float t = e.nCurrent - i * 0.05;
            if (t < 0) t += 1;
            float rad = -_easing_calc_InOutCubic(t) * _2PI + _PI;  // angle (rad, 3.14rad -> 180°)
            _draw_circle_full_at_screen_center(r * sin(rad), r * cos(rad), 1);
        }
        _update();
    }
}

void effect_rotating_rect()
{
    easing_t e = easing_create(EASING_MODE_NTIMES(2) | EASING_DIR_BACKANDFORTH, _easing_calc_OutQuart, 0, 15, 50);

    easing_start_absolute(&e, 0, 1);  // generate linear time order

    float w, h;

    while (!easing_isok(&e)) {
        easing_update(&e);
        _clear();
        w = h = easing_curpos(&e) * 10,
#if 0
        _set_color(1);
        _draw_rotated_rect(64, 32, w, h, easing_curpos(&e) * _PI2);
        _set_color(2);
        _draw_rotated_rect(64, 32, w, h, (easing_curpos(&e) + 0.5) * _PI2);
#else
        _draw_rotated_rect(64, 32, w, h, easing_curpos(&e) * _PI2);
        _draw_rotated_rect(64, 32, w, h, easing_curpos(&e) * _PI2 * 1.5);
#endif
        _update();
    }
    _set_color(1);
}

const item_menu_t menu_main[] = {
    {PAGE_ID_LOGO, "MainUI"},
    {PAGE_ID_PID_MENULIST, "+PID Editor"},
    {PAGE_ID_ICON_ICONLIST, "-Icon Test"},
    {PAGE_ID_ADC_CHART, "-Chart Test"},
    {PAGE_ID_EASING_CHART, "-Easing Chart"},
    {PAGE_ID_DEVS_MENULIST, "-Device List"},
    {PAGE_ID_TEXTEDIT, "-Text Edit"},
    {PAGE_ID_NONE, "-Play Video"},
    {PAGE_ID_ABOUT, "{ About }"},
};

void _hander_main(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        case KEY_ID_OK:

            switch (p->pItems[p->nSelectedIndex].ID) {
                case PAGE_ID_LOGO:
                case PAGE_ID_EASING_CHART:
                case PAGE_ID_ABOUT: {
                    effect_disappear();
                    PAGE_SWITCH(p->pItems[p->nSelectedIndex].ID);
                    break;
                }
                case PAGE_ID_ADC_CHART:
                case PAGE_ID_TEXTEDIT: {
                    effect_rotating_rect();
                    PAGE_SWITCH(p->pItems[p->nSelectedIndex].ID);
                    break;
                }
                case PAGE_ID_PID_MENULIST:
                case PAGE_ID_DEVS_MENULIST:
                case PAGE_ID_ICON_ICONLIST: {
                    effect_win10_loading();
                    PAGE_SWITCH(p->pItems[p->nSelectedIndex].ID);
                    break;
                }

                default:
                    break;
            }

            p->bRepaint = true;

            break;
    }
}

enum {
    FUNC_ID_EDIT_KP = FUNC_ID_RETURN + 1,
    FUNC_ID_EDIT_KI,
    FUNC_ID_EDIT_KD,
};

struct range_f {
    char* desc;
    float min;
    float max;
    float cur;
    float step;
} kp = {"Editing Kp", 0.f, 10.f, 1.f, 1.2f},
  ki = {"Editing Ki", -1.2f, 1.2f, 0.f, 0.1f},
  kd = {"Editing Kd", 4.f, 10.f, 4.f, 1.2f};

typedef struct range_f range_f;

const item_menu_t menu_pid[] = {
    {FUNC_ID_EDIT_KP, "-Proportion", &kp},
    {FUNC_ID_EDIT_KI, "-Integral", &ki},
    {FUNC_ID_EDIT_KD, "-Derivative", &kd},
    {FUNC_ID_RETURN, "Return"},
};

void _hander_pid(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        case KEY_ID_OK:

            switch (p->pItems[p->nSelectedIndex].ID) {
                case FUNC_ID_EDIT_KP:
                case FUNC_ID_EDIT_KD:
                case FUNC_ID_EDIT_KI: {
                    bool bRepaint = true;
                    for (uint16_t i = 0; i < buf_len; ++i)
                        buf_ptr[i] &= (i % 2 == 0 ? 0x55 : 0xAA);
                    _update();
                    range_f* range;
                    while (1) {
                        switch (key_scan()) {
                            case KEY_ID_NONE: break;
                            case KEY_ID_PREV:
                                range = (range_f*)(p->pItems[p->nSelectedIndex].arg);
                                if (range->cur > (range->min + range->step)) {
                                    range->cur -= range->step;
                                } else {
                                    range->cur = range->min;
                                }
                                bRepaint = true;
                                break;
                            case KEY_ID_NEXT:
                                range = (range_f*)(p->pItems[p->nSelectedIndex].arg);
                                if (range->cur < (range->max - range->step)) {
                                    range->cur += range->step;
                                } else {
                                    range->cur = range->max;
                                }
                                bRepaint = true;
                                break;
                            case KEY_ID_OK:
                                p->bRepaint = true;
                                return;
                        }

                        if (bRepaint) {
                            bRepaint = false;

                            _draw_rect(16, 16, 96, 31);
                            _set_color(2);
                            _draw_rect(17, 17, 94, 29);
                            _set_color(1);

                            _fiil_rect(18, 36, 60, 8);

                            range = (range_f*)(p->pItems[p->nSelectedIndex].arg);

                            _draw_rect(20, 38, (range->cur - range->min) / (range->max - range->min) * 56, 4);

                            _draw_str(22, 30, range->desc);

                            _draw_num(81, 44, "%.2f", range->cur);

                            _update();
                        }
                    }
                    break;
                }
                case FUNC_ID_RETURN: {
                    effect_disappear();
                    PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
                    break;
                }
            }
    }
}

const item_icon_t menu_icon[] = {
    {FUNC_ID_ANIMATION1, "Likes", 36, 35, res_likes},
    {FUNC_ID_RETURN, "Collection", 36, 37, res_collect},
    {FUNC_ID_ANIMATION2, "Slot", 36, 36, res_coin},
    {FUNC_ID_RETURN, "Share", 36, 36, res_share},
};

void _hander_icon(iconlist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: iconlist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: iconlist_callback_handler_switch_next(p); break;
        case KEY_ID_OK:
            switch (p->pItems[p->nSelectedIndex].ID) {
                case FUNC_ID_ANIMATION1:
                    iconlist_callback_handler_shake_selected(p);
                    break;
                case FUNC_ID_ANIMATION2:
                    iconlist_callback_handler_updown_selected(p);
                    break;
                case FUNC_ID_RETURN: {
                    effect_disappear();
                    PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
                    break;
                }
                default: break;
            }
    }
}

enum {
    FUNC_ID_DEV_LED = FUNC_ID_RETURN + 1,
    FUNC_ID_DEV_BUZZER,
};

item_menu_t menu_device[] = {
    {FUNC_ID_DEV_LED, "-Led", 0},
    {FUNC_ID_DEV_BUZZER, "-Buzzer", 1},
    {FUNC_ID_RETURN, "Return"},
};

void _handler_devicelist(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        case KEY_ID_OK: {
            switch (p->pItems[p->nSelectedIndex].ID) {
                case FUNC_ID_DEV_LED:
                case FUNC_ID_DEV_BUZZER: {
                    // 取反(3个！把值限制为0和1)

                    p->pItems[p->nSelectedIndex].arg = !!!(bool)(p->pItems[p->nSelectedIndex].arg);
                    // repaint
                    p->bRepaint = true;

                    break;
                }
                case FUNC_ID_RETURN: {
                    effect_disappear();
                    PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
                    break;
                }
            }
            break;
        }
    }
}

void _painter_devicelist(menulist_t* p)
{
    // easing_core
    menulist_callback_painter_update_easing(p);
    // new frame
    _clear();
    // title

    static uint8_t w_on  = 0;
    static uint8_t w_off = 0;
    if (w_on == 0) w_on = _str_w_("[on]");
    if (w_off == 0) w_off = _str_w_("[off]");

    uint8_t      index    = easing_curpos(&p->yTitleOffset) / p->hLine;
    easing_pos_t y_offset = index * p->hLine - easing_curpos(&p->yTitleOffset);
    while (index < p->nSize && y_offset < p->nHeight) {
        _draw_str(p->xPadding, y_offset + p->yPadding, p->pItems[index].title);
        if (p->pItems[index].ID > FUNC_ID_RETURN) {
            if ((bool)(p->pItems[index].arg)) {
                _draw_str(p->xSlider - p->xPadding * 2 - w_on, y_offset + p->yPadding, "[on]");
            } else {
                _draw_str(p->xSlider - p->xPadding * 2 - w_off, y_offset + p->yPadding, "[off]");
            }
        }
        ++index, y_offset += p->hLine;
    }

    // scroll
    menulist_callback_painter_draw_scroll(p);
    // mask
    menulist_callback_painter_draw_item_mask(p);
    // update screen
    _update();
}

enum {
    FUNC_ID_EASING = FUNC_ID_RETURN + 1,
};

float _easing_calc_Custom(const float t)
{
    if (t < 0.3) return t;
    if (t < 0.6) return t * t;
    return t;
}

static const item_menu_t menu_easing[] = {
    {FUNC_ID_EASING, "- Custom", _easing_calc_Custom},
    {FUNC_ID_EASING, "- Linear  ", _easing_calc_Linear},
    {FUNC_ID_EASING, "- InQuad  ", _easing_calc_InQuad},
    {FUNC_ID_EASING, "- OutQuad", _easing_calc_OutQuad},
    {FUNC_ID_EASING, "- InOutQuad", _easing_calc_InOutQuad},
    {FUNC_ID_EASING, "- InCubic  ", _easing_calc_InCubic},
    {FUNC_ID_EASING, "- OutCubic", _easing_calc_OutCubic},
    {FUNC_ID_EASING, "- InOutCubic", _easing_calc_InOutCubic},
    {FUNC_ID_EASING, "- InQuart  ", _easing_calc_InQuart},
    {FUNC_ID_EASING, "- OutQuart", _easing_calc_OutQuart},
    {FUNC_ID_EASING, "- InOutQuart", _easing_calc_InOutQuart},
    {FUNC_ID_EASING, "- InQuint  ", _easing_calc_InQuint},
    {FUNC_ID_EASING, "- OutQuint", _easing_calc_OutQuint},
    {FUNC_ID_EASING, "- InOutQuint", _easing_calc_InOutQuint},
    {FUNC_ID_EASING, "- InSine  ", _easing_calc_InSine},
    {FUNC_ID_EASING, "- OutSine", _easing_calc_OutSine},
    {FUNC_ID_EASING, "- InOutSine", _easing_calc_InOutSine},
    {FUNC_ID_EASING, "- InExpo  ", _easing_calc_InExpo},
    {FUNC_ID_EASING, "- OutExpo", _easing_calc_OutExpo},
    {FUNC_ID_EASING, "- InOutExpo", _easing_calc_InOutExpo},
    {FUNC_ID_EASING, "- InCirc  ", _easing_calc_InCirc},
    {FUNC_ID_EASING, "- OutCirc", _easing_calc_OutCirc},
    {FUNC_ID_EASING, "- InOutCirc", _easing_calc_InOutCirc},
    {FUNC_ID_EASING, "- InBack  ", _easing_calc_InBack},
    {FUNC_ID_EASING, "- OutBack", _easing_calc_OutBack},
    {FUNC_ID_EASING, "- InOutBack", _easing_calc_InOutBack},
    {FUNC_ID_EASING, "- InElastic  ", _easing_calc_InElastic},
    {FUNC_ID_EASING, "- OutElastic", _easing_calc_OutElastic},
    {FUNC_ID_EASING, "- InOutElastic", _easing_calc_InOutElastic},
    {FUNC_ID_EASING, "- InBounce  ", _easing_calc_InBounce},
    {FUNC_ID_EASING, "- OutBounce", _easing_calc_OutBounce},
    {FUNC_ID_EASING, "- InOutBounce", _easing_calc_InOutBounce},
    {FUNC_ID_RETURN, "Return", 0},
};

void _handler_easing_chart(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        case KEY_ID_OK: {
            switch (p->pItems[p->nSelectedIndex].ID) {
                case FUNC_ID_EASING: {
                    easing_t e = easing_create(EASING_MODE_DEFAULT, p->pItems[p->nSelectedIndex].arg, 0, 36, 0);

                    easing_start_relative(&e, e.nFrameCount);
                    float xoffset = (CONFIG_SCREEN_WIDTH - e.nStop) / 2, yoffset = (CONFIG_SCREEN_HEIGHT - e.nStop) / 2 + e.nStop;

                    _clear();
                    float x1 = -1, y1 = -1, x2, y2;
                    while (!easing_isok(&e)) {
                        easing_update(&e);
                        x2 = xoffset + e.nFrameIndex;
                        y2 = yoffset - easing_curpos(&e);
                        if (x1 == -1) x1 = x2;
                        if (y1 == -1) y1 = y2;
                        _draw_line(x1, y1, x2, y2);
                        x1 = x2;
                        y1 = y2;
                    }
                    _update();

                    while (key_scan() == KEY_ID_NONE) {}

                    // repaint
                    p->bRepaint = true;

                    break;
                }
                case FUNC_ID_RETURN: {
                    effect_disappear();
                    PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
                    break;
                }
            }
            break;
        }
    }
}

#endif

extern void app_entry(void*);
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
  
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_I2C2_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    /* USER CODE END 2 */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    HAL_ADCEx_Calibration_Start(&hadc1);

    key_init();
    u8g2_init();

    app_entry(0);

#if 0

    page_t page_logo  = page_new(_painter_logo, _handler_logo);
    page_t page_about = page_new(_painter_about, _handler_about);

    PAGE_REGISTER(PAGE_ID_LOGO, page_logo);
    PAGE_REGISTER(PAGE_ID_ABOUT, page_about);

    menulist_t menulist_main   = menulist_initialize(menu_main, ARRAY_SIZE(menu_main), 4, 0, _hander_main);
    menulist_t menulist_pid    = menulist_initialize(menu_pid, ARRAY_SIZE(menu_pid), 4, 0, _hander_pid);
    menulist_t menulist_easing = menulist_initialize(menu_easing, ARRAY_SIZE(menu_easing), 5, 0, _handler_easing_chart);
    menulist_t menulist_devs   = menulist_initialize(menu_device, ARRAY_SIZE(menu_device), 3, _painter_devicelist, _handler_devicelist);

    PAGE_REGISTER(PAGE_ID_MAIN_MENULIST, menulist_main);
    PAGE_REGISTER(PAGE_ID_PID_MENULIST, menulist_pid);
    PAGE_REGISTER(PAGE_ID_EASING_CHART, menulist_easing);
    PAGE_REGISTER(PAGE_ID_DEVS_MENULIST, menulist_devs);

    iconlist_t iconlist_icon = iconlist_initialize(menu_icon, ARRAY_SIZE(menu_icon), 0, _hander_icon);

    PAGE_REGISTER(PAGE_ID_ICON_ICONLIST, iconlist_icon);

    adc_chart_t page_adcchart = adcchart_initialize(0, _handler_chart);

    PAGE_REGISTER(PAGE_ID_ADC_CHART, page_adcchart);

    char         strbuf[]      = "AZaz World ";
    texteditor_t page_textedit = textedit_initialize(0, _handler_textedit, strbuf, strlen(strbuf));

    PAGE_REGISTER(PAGE_ID_TEXTEDIT, page_textedit);

    PAGE_SWITCH(PAGE_ID_FIRST);

#endif

    while (1) {
#if 0
        PAGE_RENDER();
#endif

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
