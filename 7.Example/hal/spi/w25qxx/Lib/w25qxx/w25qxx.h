#ifndef __W25QXX_H
#define __W25QXX_H

#include "spi.h"

#define W25QXX_SPI hspi3

// Configuration

// 仅支持 128Mbits 及以下，因为 256Mbits 及以上的是 4字节地址

// 1 Block = 16 sectors；1 sector = 16 pages = 4KB

#define W25Q64_BLOCK_COUNT 128
#define W25Q128_BLOCK_COUNT 256

#define W25QXX_FLASH_SIZE 0x1000000 /* 64 MBits => 8MBytes */
#define W25QXX_BLOCK_SIZE 0x10000   /* 64K Bytes */
#define W25QXX_SECTOR_SIZE 0x1000   /* 4k Bytes */
#define W25QXX_PAGE_SIZE 0x100      /* 256 bytes */

#define W25QXX_DUMMY_CYCLES_READ 4
#define W25QXX_DUMMY_CYCLES_READ_QUAD 10

#define W25QXX_BULK_ERASE_MAX_TIME 250000
#define W25QXX_SECTOR_ERASE_MAX_TIME 3000
#define W25QXX_SUBSECTOR_ERASE_MAX_TIME 800
#define W25Qx_TIMEOUT_VALUE 1000

// Commands

/* Reset Operations */
#define RESET_ENABLE_CMD 0x66
#define RESET_MEMORY_CMD 0x99

#define ENTER_QPI_MODE_CMD 0x38
#define EXIT_QPI_MODE_CMD 0xFF

/* Identification Operations */
#define READ_ID_CMD 0x90
#define DUAL_READ_ID_CMD 0x92
#define QUAD_READ_ID_CMD 0x94
#define READ_JEDEC_ID_CMD 0x9F

/* Read Operations */
#define READ_CMD 0x03
#define FAST_READ_CMD 0x0B
#define DUAL_OUT_FAST_READ_CMD 0x3B
#define DUAL_INOUT_FAST_READ_CMD 0xBB
#define QUAD_OUT_FAST_READ_CMD 0x6B
#define QUAD_INOUT_FAST_READ_CMD 0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD 0x06
#define WRITE_DISABLE_CMD 0x04

/* Register Operations */
#define READ_STATUS_REG1_CMD 0x05
#define READ_STATUS_REG2_CMD 0x35
#define READ_STATUS_REG3_CMD 0x15

#define WRITE_STATUS_REG1_CMD 0x01
#define WRITE_STATUS_REG2_CMD 0x31
#define WRITE_STATUS_REG3_CMD 0x11

/* Program Operations */
#define PAGE_PROG_CMD 0x02
#define QUAD_INPUT_PAGE_PROG_CMD 0x32

/* Erase Operations */
#define SECTOR_ERASE_CMD 0x20
#define CHIP_ERASE_CMD 0xC7

#define PROG_ERASE_RESUME_CMD 0x7A
#define PROG_ERASE_SUSPEND_CMD 0x75

/* Flag Status Register */
#define W25QXX_FSR_BUSY 0x01 /*!< busy */
#define W25QXX_FSR_WREN 0x02 /*!< write enable */
#define W25QXX_FSR_QE 0x02   /*!< quad enable */

#define W25Qx_Enable() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define W25Qx_Disable() HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)

#define W25Qx_OK 0x00
#define W25Qx_ERROR 0x01
#define W25Qx_BUSY 0x02
#define W25Qx_TIMEOUT 0x03

uint8_t        W25QXX_Init(void);
static void    W25QXX_Reset(void);
static uint8_t W25QXX_GetStatus(void);
uint8_t        W25QXX_WriteEnable(void);
void           W25QXX_Read_ID(uint8_t* ID);
uint8_t        W25QXX_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t        W25QXX_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t        W25QXX_Erase_Block(uint32_t Address);
uint8_t        W25QXX_Erase_Chip(void);

#endif /* __W25QXX_H */