
#include "ad770x.h"

void ad770x_write_byte(uint8_t byte)
{
    ad770x_selcet();
    ad770x_transmit(&byte, 1);
    ad770x_unselcet();
}

uint8_t ad770x_read_byte(void)
{
    uint8_t byte;
    ad770x_selcet();
    ad770x_receive(&byte, 1);
    ad770x_unselcet();
    return byte;
}

void ad770x_read_nbyte(uint8_t* buff, uint8_t len)
{
    ad770x_selcet();
    ad770x_receive(buff, len);
    ad770x_unselcet();
}

////////////////////////////////////////

void ad770x_reset(void)
{
    ad770x_rst_l();
    ad770x_delay_ms(20);
    ad770x_rst_h();
    ad770x_delay_ms(20);
}

// 同步 spi 接口，以防模块失步
void ad770x_sync_spi(void)
{
    uint8_t n = 100, byte = 0xff;
    ad770x_selcet();
    while (n--) ad770x_transmit(&byte, 1);
    ad770x_unselcet();
#if 1
    // 复位后需等待 500us 后再访问
    ad770x_delay_ms(1);
#endif
}

void ad770x_set_operation(
    ad770x_channel_t   ch,
    ad770x_register_t  reg,
    ad770x_operation_t rw)
{
    ad770x_write_byte(((uint8_t)reg | (uint8_t)rw | (uint8_t)ch) & 0x7F);
}

void ad770x_config_channel(
    ad770x_channel_t     channel,
    ad770x_mode_t        mode,
    ad770x_clock_t       clock,
    ad770x_gain_t        gain,
    ad770x_polarity_t    polarity,
    ad770x_update_rate_t rate)
{
    ad770x_set_operation(channel, AD770X_REG_CLOCK, AD770X_WRITE);
    ad770x_write_byte(((uint8_t)clock | (uint8_t)rate) & 0x1F);

    ad770x_set_operation(channel, AD770X_REG_SETUP, AD770X_WRITE);
    ad770x_write_byte(
        (uint8_t)mode |
        (uint8_t)gain |
        (uint8_t)polarity |
        (0 << 1) |  // 内部缓冲器 0:disable,1:enable
        (0 << 1)    // 过滤器同步 0:disable,1:enable
    );

    ad770x_wait_data_ready(channel);

#if 1
    printf("zero: %d\r\n", ad770x_read_zero_cal(channel));
    printf("full: %d\r\n", ad770x_read_full_cal(channel));
#endif
}

void ad770x_wait_data_ready(ad770x_channel_t ch)
{
#if ENABLE_AD770X_DRDY_REG
    do {
        ad770x_set_operation(ch, AD770X_REG_CMM, AD770X_READ);
    } while (ad770x_read_byte() & 0x80);
#endif
}

// Q1: PIN_DRDY 是所有 CHANNEL 都完成转换后才拉低的吗？
// Q2: 数据是自动采集的还是需要调用 ad770x_set_operation(ch, AD770X_REG_DATA, AD770X_READ) 之后才开始采集的？

uint16_t ad770x_read_adc(ad770x_channel_t ch)
{
    uint8_t buff[2];
    ad770x_set_operation(ch, AD770X_REG_DATA, AD770X_READ);
#if ENABLE_AD770X_DRDY_PIN
    while (!ad770x_drdy_read()) {}
#endif
    ad770x_read_nbyte(buff, 2);
    return (buff[0] << 8) | buff[1];
}

// 读取零标度寄存器
uint32_t ad770x_read_zero_cal(ad770x_channel_t ch)
{
    uint8_t buff[3];
    ad770x_set_operation(ch, AD770X_REG_OFFSET, AD770X_READ);
    ad770x_read_nbyte(buff, 3);
    return (buff[2] << 16) | (buff[1] << 8) | buff[0];
}

// 读取满标度寄存器
uint32_t ad770x_read_full_cal(ad770x_channel_t ch)
{
    uint8_t buff[3];
    ad770x_set_operation(ch, AD770X_REG_GAIN, AD770X_READ);
    ad770x_read_nbyte(buff, 3);
    return (buff[2] << 16) | (buff[1] << 8) | buff[0];
}
