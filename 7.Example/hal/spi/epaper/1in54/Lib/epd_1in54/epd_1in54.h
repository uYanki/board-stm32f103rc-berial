#ifndef __EPD_1IN54_H__
#define __EPD_1IN54_H__

#include "fonts.h"
#include "spi.h"

//

#define EPD_WIDTH  200
#define EPD_HEIGHT 200

#define EPD_SPI_PORT hspi3

#define EPD_NSS_SIGNAL    1
#define EPD_UPDATE_SIGNAL 0

#define EPD_MODE_FULL_UPDATE    0
#define EPD_MODE_PARTIAL_UPDATE 1

#define EPD_COLOR_BLACK 0x00
#define EPD_COLOR_WHITE 0xFF
// 黑白条纹：0xF0 0x0F 0x33 0x55 0xAA

///////////////////////////////////////////

#define epd_select()   HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET)
#define epd_unselect() HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET)

#define epd_start_cmd()  HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET)
#define epd_start_data() HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET)

#define epd_rst_l() HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET)
#define epd_rst_h() HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET)

#define epd_transmit(buff, size) HAL_SPI_Transmit(&EPD_SPI_PORT, buff, size, 0xFF)
// #define epd_receive(buff, size)                    HAL_SPI_Receive(&EPD_SPI_PORT, buff, size, 0xFF)
// #define epd_transmit_receive(txbuff, rxbuff, size) HAL_SPI_TransmitReceive(&EPD_SPI_PORT, txbuff, rxbuff, size, 0xFF)

#define epd_delay_ms(n) HAL_Delay(n)

// low: idle, high: busy

#if 0
#define epd_wait_until_idle()                                                    \
    while (HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_SET) { \
        epd_delay_ms(100);                                                       \
        printf("waiting\r\n");                                                   \
    }
#else
#define epd_wait_until_idle() \
    while (HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_SET) epd_delay_ms(100)
#endif

///////////////////////////////////////////

void epd_write_cmd(uint8_t cmd);
void epd_write_data(uint8_t dat);
void epd_write_ndata(uint8_t* dat, uint16_t len);

void epd_init(void);
void epd_reset(void);
void epd_sleep(void);

void epd_set_cursor(uint16_t x, uint16_t y);
void epd_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void epd_update(void);

void epd_set_mode(uint8_t mode);

// call the following functions before calling epd_select() and then call epd_unselect() if not use EPD_AUTO_SELECT
void epd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color);
void epd_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data);
void epd_clear(uint8_t color);

#endif