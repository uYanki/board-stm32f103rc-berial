#ifndef __KEY_MATRIX_H__
#define __KEY_MATRIX_H__

#include "gpio.h"

/**
 * GPIO:
 * - columns: output, default high level 列输出,常态高电平
 * - rows: input, pull-up 行输入,带上拉电阻
 *
 * When scanning a row of keys, the column outputs low level.
 * At this time, press a key, and the row inputs low level
 * 当扫描某行按键时, 列输出低电平, 此时按下某键, 行输入低电平
 */

#define MATRIX_KEY_ROW_COUNT 4  // number of rows
#define MATRIX_KEY_COL_COUNT 4  // number of columns

typedef GPIO_PinState key_state_t;
#define KEY_STATE_RELEASED GPIO_PIN_SET    // released
#define KEY_STATE_PRESSED  GPIO_PIN_RESET  // pressed

void        matrix_key_init();
key_state_t matrix_key_scan(uint8_t* row, uint8_t* col);

#endif