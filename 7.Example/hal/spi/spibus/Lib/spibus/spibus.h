#ifndef __SPI_BUS__
#define __SPI_BUS__

#include "config.h"
#include "utils.h"

#define CONFIG_SOFTWARE_SPI 1
#define CONFIG_HARDWARE_SPI 0

#define CONFIG_NSS_SIGNAL 1

#if CONFIG_SOFTWARE_SPI

#define CONFIG_CLKDIV 0  // enable delay

#define CONFIG_SOFTSPI_FULL_DUPLEX        0  // 全双工
#define CONFIG_SOFTSPI_MODE_TRANSMIT_ONLY 0  // 仅发送
#define CONFIG_SOFTSPI_MODE_RECEIVE_ONLY  1  // 仅接收

// #define CONFIG_SOFTSPI_HALF_DUPLEX 0  // 半双工(未支持)

#endif

#if CONFIG_HARDWARE_SPI  // hwspi

#if CONFIG_MCU_STM32
#include "spi.h"
typedef SPI_HandleTypeDef spi_t;
#elif CONFIG_MCU_ESP32
#elif CONFIG_MCU_RP2040
#endif

#elif CONFIG_SOFTWARE_SPI  // swspi

#include "gpio.h"

// receive
#if CONFIG_SOFTSPI_FULL_DUPLEX || CONFIG_SOFTSPI_MODE_TRANSMIT_ONLY
#define CONFIG_ENABLE_TRANSMIT 1
#endif

// transmit
#if CONFIG_SOFTSPI_FULL_DUPLEX || CONFIG_SOFTSPI_MODE_RECEIVE_ONLY
#define CONFIG_ENABLE_RECEIVE 1
#endif

typedef enum {
    SOFT_SPI_ID_1 = 1,
    SOFT_SPI_ID_2,
    SOFT_SPI_ID_3,
    SOFT_SPI_ID_4,
    SOFT_SPI_ID_5,
    SOFT_SPI_ID_6,
    SOFT_SPI_ID_7,
    SOFT_SPI_ID_8,
} soft_spi_id;  // unused

typedef enum {
    SOFT_SPI_FIRSTBIT_MSB = 0,
    SOFT_SPI_FIRSTBIT_LSB = 1,
} soft_spi_first_bit_t;

typedef enum {
    SOFT_SPI_DATASIZE_8BITS  = 0,
    SOFT_SPI_DATASIZE_16BITS = 1,
} soft_spi_data_size_t;  // unused

typedef enum {
    SOFT_SPI_CPOL_LOW  = 0,
    SOFT_SPI_CPOL_HIGH = 1,
} soft_spi_clock_polarity_t;

typedef enum {
    SOFT_SPI_CPHA_1EDGE = 0,
    SOFT_SPI_CPHA_2EDGE = 1,
} soft_spi_clock_phase_t;

typedef struct {  // id
    soft_spi_id ID;

    soft_spi_first_bit_t firstbit;
    soft_spi_data_size_t datasize;

    soft_spi_clock_phase_t    cpha;  // cke
    soft_spi_clock_polarity_t cpol;  // ckp

#if CONFIG_MCU_STM32
    GPIO_TypeDef*             SCLK_Port;
    uint32_t                  SCLK_Pin;
#if CONFIG_ENABLE_TRANSMIT
    GPIO_TypeDef*             MOSI_Port;
    uint32_t                  MOSI_Pin;
#endif
#if CONFIG_ENABLE_RECEIVE
    GPIO_TypeDef*             MISO_Port;
    uint32_t                  MISO_Pin;
#endif
#if CONFIG_NSS_SIGNAL
    GPIO_TypeDef*             NSS_Port;
    uint32_t                  NSS_Pin;
#endif

#elif CONFIG_MCU_ESP32

#elif CONFIG_MCU_RP2040

#endif

    uint32_t Interval;  // us
} soft_spi;

typedef soft_spi spi_t;

#endif

#if CONFIG_SOFTWARE_SPI

#if CONFIG_ENABLE_RECEIVE
uint8_t
#else
void
#endif
spi_transfer_1byte(
    spi_t*  spi,
    uint8_t data);

#if CONFIG_ENABLE_RECEIVE
uint16_t
#else
void
#endif
spi_transfer_2byte(
    spi_t*   spi,
    uint16_t data);

void spi_transfer_1bytes(
    spi_t*   spi,
    uint8_t* in,
#if CONFIG_ENABLE_RECEIVE
    uint8_t* out,
#endif
    uint16_t len);

void spi_transfer_2bytes(
    spi_t*    spi,
    uint16_t* in,
#if CONFIG_ENABLE_RECEIVE
    uint16_t* out,
#endif
    uint16_t len);

// transfer -> transmit / receive

#define spi_transmit_1byte(spi, data) spi_transfer_1byte(spi, data)
#define spi_transmit_2byte(spi, data) spi_transfer_2byte(spi, data)

#define spi_receive_1byte(spi) spi_transfer_1byte(spi, 0x00)
#define spi_receive_2byte(spi) spi_transfer_2byte(spi, 0x0000)

#if CONFIG_ENABLE_TRANSMIT && CONFIG_ENABLE_RECEIVE
void spi_transmit_1bytes(spi_t* spi, uint8_t* in, uint16_t len);
void spi_transmit_2bytes(spi_t* spi, uint16_t* in, uint16_t len);
void spi_receive_1bytes(spi_t* spi, uint8_t* out, uint16_t len);
void spi_receive_2bytes(spi_t* spi, uint16_t* out, uint16_t len);
#elif CONFIG_ENABLE_TRANSMIT
#define spi_transmit_1bytes spi_transfer_1bytes
#define spi_transmit_2bytes spi_transfer_2bytes
#elif CONFIG_ENABLE_RECEIVE
#define spi_receive_1bytes spi_receive_1bytes
#define spi_receive_2bytes spi_receive_2bytes
#endif

// nss

#if CONFIG_NSS_SIGNAL
void spi_nss_select(spi_t* spi);
void spi_nss_unselect(spi_t* spi);
#endif

#endif

#endif