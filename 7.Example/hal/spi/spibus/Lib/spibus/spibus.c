#include "spibus.h"

#if CONFIG_SOFTWARE_SPI

#if CONFIG_MCU_STM32

// static spi_t spi = {
//     .ID        = SOFT_SPI_ID_1,
//     .cpol      = SOFT_SPI_CPOL_HIGH,
//     .cpha      = SOFT_SPI_CPHA_2EDGE,
//     .datasize  = SOFT_SPI_DATASIZE_8BITS,
//     .firstbit  = SOFT_SPI_FIRSTBIT_MSB,
//     .SCLK_Port = SPI_SCLK_GPIO_Port,
//     .SCLK_Pin  = SPI_SCLK_Pin,
// #if CONFIG_ENABLE_TRANSMIT
//     .MOSI_Port = SPI_MOSI_GPIO_Port,
//     .MOSI_Pin  = SPI_MOSI_Pin,
// #endif
// #if CONFIG_ENABLE_RECEIVE
//     .MISO_Port = SPI_MISO_GPIO_Port,
//     .MISO_Pin  = SPI_MISO_Pin,
// #endif
// #if CONFIG_NSS_SIGNAL
//     .NSS_Port = SPI_CS_GPIO_Port,
//     .NSS_Pin  = SPI_CS_Pin,
// #endif
//     .Interval = 1,
// };

#define Delay(spi) HAL_Delay_us(spi->Interval)

#define SCLK_L(spi) HAL_GPIO_WritePin(spi->SCLK_Port, spi->SCLK_Pin, GPIO_PIN_RESET)
#define SCLK_H(spi) HAL_GPIO_WritePin(spi->SCLK_Port, spi->SCLK_Pin, GPIO_PIN_SET)

#if CONFIG_ENABLE_TRANSMIT
#define MOSI_L(spi) HAL_GPIO_WritePin(spi->MOSI_Port, spi->MOSI_Pin, GPIO_PIN_RESET)
#define MOSI_H(spi) HAL_GPIO_WritePin(spi->MOSI_Port, spi->MOSI_Pin, GPIO_PIN_SET)
#endif

#if CONFIG_ENABLE_RECEIVE
#define MISO(spi) HAL_GPIO_ReadPin(spi->MISO_Port, spi->MISO_Pin)
#endif

#if CONFIG_NSS_SIGNAL
#define NSS_L(spi) HAL_GPIO_WritePin(spi->NSS_Port, spi->NSS_Pin, GPIO_PIN_RESET)
#define NSS_H(spi) HAL_GPIO_WritePin(spi->NSS_Port, spi->NSS_Pin, GPIO_PIN_SET)
#endif

#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040

#endif

#if CONFIG_ENABLE_RECEIVE
uint8_t
#else
void
#endif
spi_transfer_1byte(spi_t* spi, uint8_t data)
{
#if CONFIG_ENABLE_RECEIVE
    uint16_t out = 0;
#endif
    uint8_t  len  = 8;
    uint16_t mask = spi->firstbit ? 0x01 /*lsb*/ : 0x80 /*msb*/;
#if CONFIG_NSS_SIGNAL
    // NSS_L(spi);
#if CONFIG_CLKDIV
    // Delay(spi);
#endif
#endif
    while (len--) {
        if (spi->cpha) {
            // CPHA_2EDGE
#if CONFIG_ENABLE_TRANSMIT
            if (data & mask) {
                MOSI_H(spi);
            } else {
                MOSI_L(spi);
            }
#if CONFIG_CLKDIV
            Delay(spi);
#endif
#endif
        }
#if CONFIG_ENABLE_RECEIVE
        else if (MISO(spi)) {
            out |= mask;
        }
#endif
        if (spi->cpol) {
            // CPOL_HIGH
            SCLK_L(spi);
        } else {
            // CPOL_LOW
            SCLK_H(spi);
        }
#if CONFIG_CLKDIV
        Delay(spi);
#endif
        if (!spi->cpha) {
            // CPHA_1EDGE
#if CONFIG_ENABLE_TRANSMIT
            if (data & mask) {
                MOSI_H(spi);
            } else {
                MOSI_L(spi);
            }
#if CONFIG_CLKDIV
            Delay(spi);
#endif
#endif
        }
#if CONFIG_ENABLE_RECEIVE
        else if (MISO(spi)) {
            out |= mask;
        }
#endif
        if (spi->cpol) {
            // CPOL_HIGH
            SCLK_H(spi);
        } else {
            // CPOL_LOW
            SCLK_L(spi);
        }
#if CONFIG_NO_CLKDIV
        Delay(spi);
#endif
        mask = spi->firstbit ? (mask << 1) : (mask >> 1);
    }
#if CONFIG_NSS_SIGNAL
    // NSS_H(spi);
#endif
#if CONFIG_ENABLE_RECEIVE
    return out;
#endif
}

#if CONFIG_ENABLE_RECEIVE
uint16_t
#else
void
#endif
spi_transfer_2byte(spi_t* spi, uint16_t data)
{
    union {
        uint16_t val;
        struct {
            uint8_t lsb;
            uint8_t msb;
        };
    } in
#if CONFIG_ENABLE_RECEIVE
        ,
        out
#endif
        ;

    in.val = data;

    if (spi->firstbit == SOFT_SPI_FIRSTBIT_MSB) {
#if CONFIG_ENABLE_RECEIVE
        out.msb =
#endif
            spi_transfer_1byte(spi, in.msb);
#if CONFIG_ENABLE_RECEIVE
        out.lsb =
#endif
            spi_transfer_1byte(spi, in.lsb);
    } else {
#if CONFIG_ENABLE_RECEIVE
        out.lsb =
#endif
            spi_transfer_1byte(spi, in.lsb);
#if CONFIG_ENABLE_RECEIVE
        out.msb =
#endif
            spi_transfer_1byte(spi, in.msb);
    }
#if CONFIG_ENABLE_RECEIVE
    return out.val;
#endif
}

void spi_transfer_1bytes(
    spi_t*   spi,
    uint8_t* in,
#if CONFIG_ENABLE_RECEIVE
    uint8_t* out,
#endif
    uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i)
#if CONFIG_ENABLE_RECEIVE
        out[i] =
#endif
            spi_transfer_1byte(spi, in[i]);
}

void spi_transfer_2bytes(
    spi_t*    spi,
    uint16_t* in,
#if CONFIG_ENABLE_RECEIVE
    uint16_t* out,
#endif
    uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i)
#if CONFIG_ENABLE_RECEIVE
        out[i] =
#endif
            spi_transfer_2byte(spi, in[i]);
}

#if CONFIG_ENABLE_TRANSMIT && CONFIG_ENABLE_RECEIVE

void spi_transmit_1bytes(spi_t* spi, uint8_t* in, uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i) spi_transfer_1byte(spi, in[i]);
}

void spi_transmit_2bytes(spi_t* spi, uint16_t* in, uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i) spi_transfer_2byte(spi, in[i]);
}

void spi_receive_1bytes(spi_t* spi, uint8_t* out, uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i) out[i] = spi_transfer_1byte(spi, 0x00);
}

void spi_receive_2bytes(spi_t* spi, uint16_t* out, uint16_t len)
{
    for (uint16_t i = 0; i < len; ++i) out[i] = spi_transfer_2byte(spi, 0x0000);
}

#endif

#if CONFIG_NSS_SIGNAL
void spi_nss_select(spi_t* spi)
{
    NSS_L(spi);
}
void spi_nss_unselect(spi_t* spi)
{
    NSS_H(spi);
}
#endif

#endif