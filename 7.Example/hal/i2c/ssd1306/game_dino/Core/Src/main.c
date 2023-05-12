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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306/ssd1306_i2c.h"
#include "images.h"

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

#define ARRSZIE(arr) (sizeof(arr) / sizeof(arr[0]))

uint8_t get_key_val()
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
        HAL_Delay(10);
        return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET;
    }
}

int32_t _min(int32_t a, int32_t b) { return a < b ? a : b; }
int32_t _max(int32_t a, int32_t b) { return a > b ? a : b; }

// 云朵
int16_t cloud_offset_x;
uint8_t cloud_offset_y;
// 仙人掌
uint8_t       cactus_index   = 0;
uint8_t       cactus_width[] = {8, 16, 24, 24};  // sizeof(CACTUS_x) / 2;
uint8_t*      cactus_image[] = {CACTUS_1, CACTUS_2, CACTUS_3, CACTUS_4};
int16_t       cactus_offset_x;
const uint8_t cactus_height = 16;
const uint8_t cactus_speed  = 5;
// 小恐龙
const uint8_t dino_width    = 16;
const uint8_t dino_height   = 16;
const uint8_t dino_offset_x = dino_width;
int8_t        dino_offset_y;
// 空字节
const uint8_t zerobuff[128] = {0x00};

uint8_t game_draw_image(
    int16_t offset_x,  // col span
    int16_t offset_y,  // row span
    uint8_t image_w,
    uint8_t image_h,
    uint8_t image_p[],
    uint8_t clear_w)
{
    if (offset_y >= SSD1306_ROW_COUNT ||
        offset_y <= -image_h) return 1;  // y out of range

    if (offset_x >= SSD1306_COL_COUNT) return 2;  // x out of range

    uint8_t dst_x, dst_y;
    uint8_t dst_w, dst_h;
    uint8_t clr_x, clr_w;

    clr_w = _min(clear_w, SSD1306_COL_COUNT);

    if (offset_y < 0) {
        image_p += -offset_y * image_w;
        dst_h = image_h + offset_y;
        dst_y = 0;
    } else {
        dst_y = offset_y;
        dst_h = _min(image_h, SSD1306_ROW_COUNT - dst_y);
    }

    if (offset_x < 0) {
        if (offset_x <= -image_w) {
            if (clr_w) ssd1306_show_img(0, dst_y, clr_w, dst_h, zerobuff);
            return 2;  // x out of range
        }
        image_p -= offset_x;
        dst_w = image_w + offset_x;
        dst_x = 0;
    } else {  // dst_x>0
        dst_x = offset_x;
        dst_w = _min(image_w, SSD1306_COL_COUNT - dst_x);
    }

    clr_x = dst_x + dst_w;

    if (clr_x >= SSD1306_COL_COUNT) {
        clr_w = 0;
    } else if (clr_w > (SSD1306_COL_COUNT - clr_x)) {
        clr_w = (SSD1306_COL_COUNT - clr_x);
    }

    while (dst_h--) {
        ssd1306_show_img(dst_x, dst_y, dst_w, 1, image_p);
        if (clr_w) ssd1306_write_ndata(zerobuff, clr_w);
        ++dst_y;
        image_p += image_w;
    }

    return 0;
}

// 绘制云朵

void game_reset_cloud()
{
    cloud_offset_x = SSD1306_COL_COUNT;
    cloud_offset_y = rand() % 3;
}

void game_draw_cloud()
{
    const uint8_t speed = 3;

    // ssd1306_show_str(score) -> clear_w = speed
    // ssd1306_show_str_ex(score) -> clear_w = SSD1306_COL_COUNT
    if (game_draw_image(cloud_offset_x, cloud_offset_y, sizeof(CLOUD), 1, CLOUD, speed))
        game_reset_cloud();

    cloud_offset_x -= speed;
}

// 绘制仙人掌

void game_reset_cactus()
{
    cactus_index    = rand() % ARRSZIE(cactus_image);
    cactus_offset_x = SSD1306_COL_COUNT;
}

void game_draw_cactus()
{
    if (game_draw_image(cactus_offset_x, 5, cactus_width[cactus_index], 2, cactus_image[cactus_index], cactus_speed))
        game_reset_cactus();
    cactus_offset_x -= cactus_speed;
}

// 绘制地面 ok
void game_draw_ground()
{
    static uint16_t x     = 0;
    const uint8_t   speed = 5;

    uint8_t remain = sizeof(GROUND) - x;

    if (remain >= SSD1306_COL_COUNT) {
        ssd1306_show_img(0, 7, SSD1306_COL_COUNT, 1, GROUND + x);
    } else {
        ssd1306_show_img(0, 7, remain, 1, GROUND + x);
        ssd1306_show_img(remain, 7, SSD1306_COL_COUNT - remain, 1, GROUND);
    }

    x += speed;
    if (x >= sizeof(GROUND))
        x = 0;
}

// 绘制小恐龙

void game_reset_dino()
{
    dino_offset_y = 0;
}

// 绘制跑步小恐龙 ok
void game_draw_dino()
{
    static uint8_t dino_dir = 0;
    ssd1306_show_img(dino_offset_x, 5, dino_width, 2, DINO[dino_dir = !dino_dir]);
}

// 绘制跳跃小恐龙 ok
void game_draw_jump_dino()
{
    const int8_t   speed_arr[] = {1, 1, 3, 3, 4, 4, 5, 6, 7};
    static uint8_t speed_idx   = sizeof(speed_arr) - 1;
    static uint8_t dir         = 0;

    if (dir == 0) {
        dino_offset_y += speed_arr[speed_idx--];

        if (dino_offset_y >= 31) {
            dir           = 1;
            dino_offset_y = 31;
        }

        if (speed_idx < 0) speed_idx = 0;

    } else {
        dino_offset_y -= speed_arr[speed_idx++];

        if (dino_offset_y <= 0) {
            dir           = 0;
            dino_offset_y = 0;
        }

        if (speed_idx > sizeof(speed_arr) - 1)
            speed_idx = sizeof(speed_arr) - 1;
    }

    uint8_t offset = dino_offset_y / 8;

    ssd1306_show_img(dino_offset_x, 4 - offset, dino_width, 3, DINO_JUMP[dino_offset_y % 8]);

    uint8_t y = (dir ? 3 : 7) - offset;

    // ignore score area and ground area
    if (y > 1 && y < 7) ssd1306_show_img(dino_offset_x, y, dino_width, 1, zerobuff);
}

//

void game_draw_start()
{
    ssd1306_fill_img(COVER);  // cover
}

void game_draw_restart()
{
    ssd1306_show_img(52, 2, 24, 3, RESTART);
    ssd1306_show_str(10, 3, "GAME", 2);
    ssd1306_show_str(86, 3, "OVER", 2);
}

//

void game_reset()
{
    game_reset_cactus();
    game_reset_dino();
    game_reset_cloud();
    ssd1306_clear();
    HAL_Delay(10);
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */
    uint32_t current_score = 0;
    uint32_t highest_score = 0;
    int8_t   failed        = 0;
    char     buff[10];
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
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    ssd1306_init();
    game_draw_start();
    while (!get_key_val()) {}
    game_reset();  // reset params
    while (1) {
        if (failed) {
            game_draw_restart();
            if (get_key_val()) {
                if (current_score > highest_score)
                    highest_score = current_score;
                current_score = 0;
                failed        = 0;
                game_reset();
            }
            continue;
        }

        ++current_score;

        (dino_offset_y > 0 || get_key_val()) ? game_draw_jump_dino() : game_draw_dino();

        game_draw_ground();
        game_draw_cloud();
        game_draw_cactus();

        if (dino_offset_y <= cactus_height) {  // hittest
            failed = !(cactus_offset_x > (dino_offset_x + dino_width) ||
                       dino_offset_x > (cactus_offset_x + cactus_width[cactus_index]));
        }

        // score
        sprintf(buff, "%4d/%d", current_score, highest_score);
        ssd1306_show_str(0, 0, buff, 1);

        if (current_score == 9999) {
            // success
            while (1) {}
        }

        HAL_Delay(30 - _min(current_score / 20, 29));  // play speed

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
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
