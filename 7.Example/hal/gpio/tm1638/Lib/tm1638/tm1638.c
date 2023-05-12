
#include "tm1638.h"

#define TM1638_WAIT()  delay_us(1)  // delay_ms() 会导致按键失效

#define TM1638_DIO_L() HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin, GPIO_PIN_RESET)
#define TM1638_DIO_H() HAL_GPIO_WritePin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin, GPIO_PIN_SET)
#define TM1638_CLK_L() HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port, TM1638_CLK_Pin, GPIO_PIN_RESET), TM1638_WAIT()
#define TM1638_CLK_H() HAL_GPIO_WritePin(TM1638_CLK_GPIO_Port, TM1638_CLK_Pin, GPIO_PIN_SET), TM1638_WAIT()
#define TM1638_STB_L() HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, GPIO_PIN_RESET), TM1638_WAIT()  // 指令输入
#define TM1638_STB_H() HAL_GPIO_WritePin(TM1638_STB_GPIO_Port, TM1638_STB_Pin, GPIO_PIN_SET), TM1638_WAIT()    // 指令执行

#define TM1638_DIO()   (HAL_GPIO_ReadPin(TM1638_DIO_GPIO_Port, TM1638_DIO_Pin) == GPIO_PIN_SET)

static GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin   = TM1638_DIO_Pin,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Pull  = GPIO_PULLDOWN};

static void TM1638_DIO_OUT()
{
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(TM1638_DIO_GPIO_Port, &GPIO_InitStruct);
}

static void TM1638_DIO_IN()
{
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(TM1638_DIO_GPIO_Port, &GPIO_InitStruct);
}

/*****************************************************/

#define TM1638_ASCII_OFFSET 32   // Ascii table offset to jump over first missing 32 chars
#define TM1638_DOT_MASK_DEC 128  // 0x80 Mask to  switch on decimal point in seven seg.

// 共阴数码管码表
const uint8_t SEG_TABLE[] = {
    0x00, /* (space) */
    0x86, /* ! */
    0x22, /* " */
    0x7E, /* # */
    0x6D, /* $ */
    0xD2, /* % */
    0x46, /* & */
    0x20, /* ' */
    0x29, /* ( */
    0x0B, /* ) */
    0x21, /* * */
    0x70, /* + */
    0x10, /* , */
    0x40, /* - */
    0x80, /* . */
    0x52, /* / */
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x09, /* : */
    0x0D, /* ; */
    0x61, /* < */
    0x48, /* = */
    0x43, /* > */
    0xD3, /* ? */
    0x5F, /* @ */
    0x77, /* A */
    0x7C, /* B */
    0x39, /* C */
    0x5E, /* D */
    0x79, /* E */
    0x71, /* F */
    0x3D, /* G */
    0x76, /* H */
    0x30, /* I */
    0x1E, /* J */
    0x75, /* K */
    0x38, /* L */
    0x15, /* M */
    0x37, /* N */
    0x3F, /* O */
    0x73, /* P */
    0x6B, /* Q */
    0x33, /* R */
    0x6D, /* S */
    0x78, /* T */
    0x3E, /* U */
    0x3E, /* V */
    0x2A, /* W */
    0x76, /* X */
    0x6E, /* Y */
    0x5B, /* Z */
    0x39, /* [ */
    0x64, /* \ */
    0x0F, /* ] */
    0x23, /* ^ */
    0x08, /* _ */
    0x02, /* ` */
    0x5F, /* a */
    0x7C, /* b */
    0x58, /* c */
    0x5E, /* d */
    0x7B, /* e */
    0x71, /* f */
    0x6F, /* g */
    0x74, /* h */
    0x10, /* i */
    0x0C, /* j */
    0x75, /* k */
    0x30, /* l */
    0x14, /* m */
    0x54, /* n */
    0x5C, /* o */
    0x73, /* p */
    0x67, /* q */
    0x50, /* r */
    0x6D, /* s */
    0x78, /* t */
    0x1C, /* u */
    0x1C, /* v */
    0x14, /* w */
    0x76, /* x */
    0x6E, /* y */
    0x5B, /* z */
          // Note : Removed last 4 characters to reduce program size as of v 1.3.0
    0x46, /* { */
    0x30, /* | */
    0x70, /* } */
    0x01, /* ~ */
};

/*****************************************************/

uint8_t tm1638_read_byte(void)
{
    uint8_t i, val = 0;

    for (i = 0; i < 8; ++i) {
        val >>= 1;
        TM1638_CLK_L();
        if (TM1638_DIO())
            val |= 0x80;
        TM1638_CLK_H();
    }

    return val;
}

void tm1638_write_byte(uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        TM1638_CLK_L();
        (val & 0x01)
            ? TM1638_DIO_H()
            : TM1638_DIO_L();
        val >>= 1;
        TM1638_CLK_H();
    }
}

void tm1638_write_cmd(uint8_t cmd)
{
    TM1638_STB_L();
    tm1638_write_byte(cmd);
    TM1638_STB_H();
}

void tm1638_write_data(uint8_t addr, uint8_t data)
{
    TM1638_STB_L();
    tm1638_write_byte(addr);
    tm1638_write_byte(data);
    TM1638_STB_H();
}

void tm1638_init(void)
{
    TM1638_DIO_OUT();
    tm1638_write_cmd(TM1638_ACTIVATE);
    tm1638_set_brightness(0x04);
    tm1638_reset();
}

void tm1638_reset(void)
{
    uint8_t i = 16;

    // set auto increment mode
    tm1638_write_cmd(TM1638_WRITE_INC);
    TM1638_STB_L();
    // set starting address
    tm1638_write_byte(TM1638_SEG_ADR);
    // reset allleds and segs
    while (i--) tm1638_write_byte(0x00);
    TM1638_STB_H();
}

// Sets the brightness level on a scale of brightness = 0 to 7. （8级可调亮度）
// 0 is not turned off, it's just the lowest brightness.
void tm1638_set_brightness(uint8_t brightness)
{
    tm1638_write_cmd((brightness <= 0x07) ? /* on */ (TM1638_BRIGHT_ADR | brightness) : /* off */ (0x80));
}

void tm1638_set_led(uint8_t idx, uint8_t val)
{
    tm1638_write_cmd(TM1638_WRITE_LOC);
    tm1638_write_data(TM1638_LEDS_ADR | (idx << 1), !!val);
}

void tm1638_set_leds(uint8_t mask)
{
    uint8_t idx;
    for (idx = 0; idx < 8; ++idx)
        tm1638_set_led(idx, (mask >> idx) & 0x01);
}

void tm1638_set_seg(uint8_t idx, uint8_t val)
{
    tm1638_write_cmd(TM1638_WRITE_LOC);
    tm1638_write_data(TM1638_SEG_ADR | (idx << 1), val);
}

uint8_t tm1638_scan_keys(void)
{
    uint8_t i, key[4];

    TM1638_STB_L();
    tm1638_write_byte(TM1638_READ_BTN);
    TM1638_DIO_IN();
    for (i = 0; i < 4; ++i)
        key[i] = tm1638_read_byte();
    TM1638_DIO_OUT();
    TM1638_STB_H();

    // 注：有好几种版本的按键
#if 1
    // 8 keys
    uint8_t mask = 0;
    for (i = 0; i < 4; ++i)
        mask |= key[i] << i;
    return mask;
#elif 1
    // 16 keys
    uint16_t mask = 0;
    return mask;
#else
    // 24 keys
    uint24_t mask = 0;
    return mask;
#endif
}

void tm1638_display_char(uint8_t idx, uint8_t ascii, uint8_t dot)
{
    tm1638_set_seg(idx, SEG_TABLE[ascii - TM1638_ASCII_OFFSET] | (dot ? TM1638_DOT_MASK_DEC : 0));
}

void tm1638_display_string(uint8_t idx, const char* str)
{
    while (idx < 8) {
        tm1638_display_char(idx, *str, *(str + 1) == '.');
        if (*str++ == '\0') break;
        ++idx;
    }
}

void tm1638_seg_table_test(void)
{
    uint8_t i, j;

    for (i = 0; i < sizeof(SEG_TABLE); i += 8) {
        for (j = 0; j < 8; ++j)
            tm1638_set_seg(j, SEG_TABLE[i + j]);
        HAL_Delay(1000);
        while (!tm1638_scan_keys()) {}  // wait keys
    }
}

/**
 * @param [in] ms time interval
 */
void tm1638_do_loading(uint8_t ms)
{
    uint8_t i;

    tm1638_set_seg(0, 0x01);
    delay_ms(ms);

    for (i = 1; i < 8; ++i) {
        tm1638_set_seg(i - 1, 0x00);
        tm1638_set_seg(i, 0x01);
        delay_ms(ms);
    }

    tm1638_set_seg(7, 0x02);
    delay_ms(ms);
    tm1638_set_seg(7, 0x04);
    delay_ms(ms);
    tm1638_set_seg(7, 0x08);
    delay_ms(ms);
    for (i = 8; i > 0; --i) {
        tm1638_set_seg(i, 0x00);
        tm1638_set_seg(i - 1, 0x08);
        delay_ms(ms);
    }

    tm1638_set_seg(0, 0x10);
    delay_ms(ms);
    tm1638_set_seg(0, 0x20);
    delay_ms(ms);
}