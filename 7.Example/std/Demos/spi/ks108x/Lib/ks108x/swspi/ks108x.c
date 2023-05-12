#include "ks108x.h"

#define RESET             0xFE
#define RDATA             0x01
#define RREG              0x10
#define WREG              0x20

#define KS108X_REG_CH1SET 0x00
#define KS108X_REG_CH2SET 0x01

void ks108x_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    PIN_KS108X_NSS  = 1;
    PIN_KS108X_MOSI = 0;
    PIN_KS108X_SCLK = 0;
}

void ks108x_send_byte(uint8_t byte)
{
    uint8_t i;

    // println("tx: 0x%02x", byte);

    for (i = 0; i < 8; ++i) {
        PIN_KS108X_SCLK = 1;
        delay_us(5);
        PIN_KS108X_MOSI = byte & 0x80 ? 1 : 0;
        byte <<= 1;
        delay_us(5);
        PIN_KS108X_SCLK = 0;
        delay_us(5);
    }
}

uint8_t ks108x_read_byte()
{
    uint8_t i;
    uint8_t byte;

    for (i = 0; i < 8; ++i) {
        PIN_KS108X_SCLK = 1;
        delay_us(5);
        PIN_KS108X_SCLK = 0;
        delay_us(5);
        byte <<= 1;
        byte |= PIN_KS108X_MISO ? 1 : 0;
        delay_us(5);
    }

    // println("rx: 0x%02x", byte);

    return byte;
}

void ks108x_select()
{
    PIN_KS108X_NSS = 0;
    delay_us(5);
}

void ks108x_unselect()
{
    delay_us(5);
    PIN_KS108X_NSS = 1;
}

bool ks108x_read_regs(uint8_t reg, uint8_t* buff, uint8_t len)
{
    reg &= 0x0F;
    len &= 0x0F;

    if (!len) return false;

    ks108x_select();
    ks108x_send_byte(RREG | reg);
    ks108x_send_byte(len - 1);
    while (len--) *buff++ = ks108x_read_byte();
    ks108x_unselect();
    return true;
}

bool ks108x_write_regs(uint8_t reg, uint8_t* buff, uint8_t len)
{
    reg &= 0x0F;
    len &= 0x0F;

    if (!len) return false;

    ks108x_select();
    ks108x_send_byte(WREG | reg);
    ks108x_send_byte(len - 1);
    while (len--) ks108x_send_byte(*buff++);
    ks108x_unselect();

    return true;
}

uint8_t ks108x_read_reg(uint8_t reg)
{
    uint8_t dat;
    if (ks108x_read_regs(reg, &dat, 1)) {
#if CONFIG_PRINF_RXDAT
        println("[rx] REG 0x%02X = DAT 0x%02X", reg, dat);
#endif
        return dat;
    } else {
        return 0x00;
    }
}

bool ks108x_write_reg(uint8_t reg, uint8_t data)
{
    uint8_t retry = 5;
    do {
        ks108x_write_regs(reg, &data, 1);
    } while (data != ks108x_read_reg(reg) && retry--);
    return retry ? true : false;
}

// 寄存器恢复至默认值
void ks108x_reset(void)
{
    ks108x_select();
    ks108x_send_byte(RESET);
    ks108x_unselect();
}

void ks108x_set_ch1(ks108x_chl1en_e enable, ks108x_gain1_e gain1, ks108x_gain2_e gain2)
{
    ks108x_write_reg(KS108X_REG_CH1SET, ((uint8_t)enable << 5) | ((uint8_t)gain2 << 2) | ((uint8_t)gain1));
}

void ks108x_set_ch2(ks108x_chl2en_e enable, ks108x_gain1_e gain1, ks108x_gain2_e gain2)
{
    ks108x_write_reg(KS108X_REG_CH2SET, ((uint8_t)enable << 5) | ((uint8_t)gain2 << 2) | ((uint8_t)gain1));
}
