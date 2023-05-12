// 不推荐用硬件 SPI

#include "ks108x.h"

#define RESET                 0xFE
#define RDATA                 0x01
#define RREG                  0x10
#define WREG                  0x20

#define KS108X_REG_CH1SET     0x00
#define KS108X_REG_CH2SET     0x01

#define KS108X_SPI_RCCEN      RCC_APB2PeriphClockCmd
#define KS108X_SPI_RCC        RCC_APB2Periph_SPI1
#define KS108X_SPI_PORT       SPI1

#define KS108X_GPIO_RCCEN_NSS RCC_APB2PeriphClockCmd
#define KS108X_GPIO_RCC_NSS   RCC_APB2Periph_GPIOB
#define KS108X_GPIO_PORT_NSS  GPIOB
#define KS108X_GPIO_PIN_NSS   GPIO_Pin_11  // NSS

void ks108x_select(void)
{
    GPIO_WriteBit(KS108X_GPIO_PORT_NSS, KS108X_GPIO_PIN_NSS, Bit_RESET);
    delay_us(10);  // necessary
}

void ks108x_unselect(void)
{
    delay_us(10);  // necessary
    GPIO_WriteBit(KS108X_GPIO_PORT_NSS, KS108X_GPIO_PIN_NSS, Bit_SET);
}

void ks108x_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    {
        KS108X_GPIO_RCCEN_NSS(KS108X_GPIO_RCC_NSS, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = KS108X_GPIO_PIN_NSS;  // CS
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_Init(KS108X_GPIO_PORT_NSS, &GPIO_InitStructure);
        GPIO_WriteBit(KS108X_GPIO_PORT_NSS, KS108X_GPIO_PIN_NSS, Bit_SET);
    }
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;  // SCK/MISO/MOSI
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    {
        KS108X_SPI_RCCEN(KS108X_SPI_RCC, ENABLE);

        SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  // 多了少了都不行, 对时钟有要求
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_InitStructure.SPI_CRCPolynomial     = 7;
        SPI_Init(KS108X_SPI_PORT, &SPI_InitStructure);
    }

    SPI_Cmd(KS108X_SPI_PORT, ENABLE);
}

uint8_t SPI_ReadWriteByte(SPI_TypeDef* spi, uint8_t byte)
{
    while (RESET == SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_TXE))
        ;
    SPI_I2S_SendData(spi, byte);

    while (RESET == SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_RXNE))
        ;
    return SPI_I2S_ReceiveData(spi);
}

void ks108x_send_byte(uint8_t byte)
{
    while (RESET == SPI_I2S_GetFlagStatus(KS108X_SPI_PORT, SPI_I2S_FLAG_TXE))
        ;
    SPI_I2S_SendData(KS108X_SPI_PORT, byte);
}

uint8_t ks108x_read_byte()
{
    return SPI_ReadWriteByte(KS108X_SPI_PORT, 0x00);
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
