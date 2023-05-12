
#include "epd_1in54.h"

// 8 pixel = 1 byte
#if 1
#define epd_8p1b(w) ((w & 0xF8) / 8)
#else
#define epd_8p1b(w) ((w / 8) + ((w & 0xF8) ? 1 : 0))
#endif

const uint16_t epd_buff_width  = epd_8p1b(EPD_WIDTH);
const uint16_t epd_buff_height = EPD_HEIGHT;
const uint16_t epd_buff_size   = epd_buff_width * epd_buff_height;

// commands
#define DRIVER_OUTPUT_CONTROL                0x01
#define BOOSTER_SOFT_START_CONTROL           0x0C
#define GATE_SCAN_START_POSITION             0x0F
#define DEEP_SLEEP_MODE                      0x10
#define DATA_ENTRY_MODE_SETTING              0x11
#define SW_RESET                             0x12
#define TEMPERATURE_SENSOR_CONTROL           0x1A
#define MASTER_ACTIVATION                    0x20
#define DISPLAY_UPDATE_CONTROL_1             0x21
#define DISPLAY_UPDATE_CONTROL_2             0x22
#define WRITE_RAM                            0x24
#define WRITE_VCOM_REGISTER                  0x2C
#define WRITE_LUT_REGISTER                   0x32
#define SET_DUMMY_LINE_PERIOD                0x3A
#define SET_GATE_TIME                        0x3B  //  not in datasheet
#define BORDER_WAVEFORM_CONTROL              0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION 0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION 0x45
#define SET_RAM_X_ADDRESS_COUNTER            0x4E
#define SET_RAM_Y_ADDRESS_COUNTER            0x4F
#define TERMINATE_FRAME_READ_WRITE           0xFF  // aka NOOP

int _min(int a, int b)
{
    return a < b ? a : b;
}

static const uint8_t EPD_1IN54_lut_full_update[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00};

static const uint8_t EPD_1IN54_lut_partial_update[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void epd_write_cmd(uint8_t cmd)
{
    epd_start_cmd();
    epd_transmit(&cmd, 1);
}

void epd_write_data(uint8_t dat)
{
    epd_start_data();
    epd_transmit(&dat, 1);
}

void epd_write_ndata(uint8_t* dat, uint16_t len)
{
    epd_start_data();
    epd_transmit(dat, len);
}

void epd_reset(void)
{
    epd_rst_l();
    epd_delay_ms(200);
    epd_rst_h();
    epd_delay_ms(200);
}

void epd_init(void)
{
    epd_reset();

    epd_select();

    epd_write_cmd(DRIVER_OUTPUT_CONTROL);
    epd_write_data(((EPD_HEIGHT - 1) >> 0) & 0xFF);
    epd_write_data(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    epd_write_data(0x00);  // GD = 0; SM = 0; TB = 0;

    epd_write_cmd(BOOSTER_SOFT_START_CONTROL);
    epd_write_data(0xD7);
    epd_write_data(0xD6);
    epd_write_data(0x9D);

    epd_write_cmd(WRITE_VCOM_REGISTER);
    epd_write_data(0xAF);  // VCOM 7C

    epd_write_cmd(SET_DUMMY_LINE_PERIOD);
    epd_write_data(0x1A);  // 4 dummy lines per gate

    epd_write_cmd(SET_GATE_TIME);
    epd_write_data(0x08);  // 2us per line

    epd_write_cmd(DATA_ENTRY_MODE_SETTING);
    epd_write_data(0x03);

    // epd_set_mode(EPD_MODE_FULL_UPDATE);
    epd_set_mode(EPD_MODE_PARTIAL_UPDATE);

    epd_unselect();
}

void epd_update(void)
{
#if EPD_NSS_SIGNAL
    epd_select();
#endif

    epd_write_cmd(DISPLAY_UPDATE_CONTROL_2);
    epd_write_data(0xC4);
    epd_write_cmd(MASTER_ACTIVATION);
    epd_write_cmd(TERMINATE_FRAME_READ_WRITE);

#if EPD_NSS_SIGNAL
    epd_unselect();
#endif

    epd_wait_until_idle();
}

//////////////////////////////////

// to wake call reset() or init()
void epd_sleep(void)
{
    epd_select();
    epd_write_cmd(DEEP_SLEEP_MODE);
    epd_write_data(0x01);  // enter deep sleep A0=1, A0=0 power on
    epd_unselect();
}

void epd_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t data[2] = {(x1 >> 3) & 0xFF, (x2 >> 3) & 0xFF};
    epd_write_cmd(SET_RAM_X_ADDRESS_START_END_POSITION);
    epd_write_ndata(data, 2);

    epd_write_cmd(SET_RAM_Y_ADDRESS_START_END_POSITION);
    epd_start_data();
    epd_transmit((uint8_t*)&y1, 2);
    epd_transmit((uint8_t*)&y2, 2);
}

void epd_set_cursor(uint16_t x, uint16_t y)
{
    epd_write_cmd(SET_RAM_X_ADDRESS_COUNTER);
    epd_write_data((x >> 3) & 0xFF);

    epd_write_cmd(SET_RAM_Y_ADDRESS_COUNTER);
    epd_write_ndata((uint8_t*)&y, 2);
}

void epd_set_mode(uint8_t mode)  // set lut
{
    epd_write_cmd(WRITE_LUT_REGISTER);
    switch (mode) {
        case EPD_MODE_FULL_UPDATE: epd_write_ndata((uint8_t*)EPD_1IN54_lut_full_update, sizeof(EPD_1IN54_lut_full_update)); break;
        case EPD_MODE_PARTIAL_UPDATE: epd_write_ndata((uint8_t*)EPD_1IN54_lut_partial_update, sizeof(EPD_1IN54_lut_partial_update)); break;
    }
}

//////////////////////////////////

void epd_clear(uint8_t color)
{
    epd_fill_rect(0, 0, EPD_WIDTH, EPD_HEIGHT, color);
}

void epd_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data)
{
#if 1

    if (x >= EPD_WIDTH ||
        y >= EPD_HEIGHT ||
        (x + w) > EPD_WIDTH ||
        (y + h) > EPD_HEIGHT)
        return;

#endif

#if EPD_NSS_SIGNAL
    epd_select();
#endif

    epd_set_window(x, y, x + w - 1, y + h - 1);
    epd_set_cursor(x, y);
    epd_write_cmd(WRITE_RAM);
    epd_wait_until_idle();
    epd_write_ndata(data, w / 8 * h);

#if EPD_NSS_SIGNAL
    epd_unselect();
#endif

#if EPD_UPDATE_SIGNAL
    epd_update();
#endif
}

void epd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color)
{
#if 1

    if (x >= EPD_WIDTH ||
        y >= EPD_HEIGHT)
        return;

    if (w > (EPD_WIDTH - x))
        w = EPD_WIDTH - x;
    if (h > (EPD_HEIGHT - y))
        h = EPD_HEIGHT - y;

#endif

#if EPD_NSS_SIGNAL
    epd_select();
#endif

    epd_set_window(x, y, x + w - 1, y + h - 1);
    epd_set_cursor(x, y);
    epd_write_cmd(WRITE_RAM);
    epd_wait_until_idle();

    uint32_t size = w / 8 * h;

    epd_start_data();
    while (size--)
        epd_transmit(&color, 1);

#if EPD_NSS_SIGNAL
    epd_unselect();
#endif

#if EPD_UPDATE_SIGNAL
    epd_update();
#endif
}