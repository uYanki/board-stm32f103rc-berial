#ifndef _BSP_W25Q64_H_
#define _BSP_W25Q64_H_

#include <quadspi.h>

#define ManufactDeviceID_CMD    0x90
#define READ_STATU_REGISTER_1   0x05
#define READ_STATU_REGISTER_2   0x35
#define READ_DATA_CMD           0x03
#define WRITE_ENABLE_CMD        0x06
#define WRITE_DISABLE_CMD       0x04
#define SECTOR_ERASE_CMD        0x20
#define CHIP_ERASE_CMD          0xc7
#define PAGE_PROGRAM_CMD        0x02


/* 移植相关函数 */
HAL_StatusTypeDef QSPI_Send_Command(uint32_t instruction, 
                                    uint32_t address, 
                                    uint32_t dummyCycles, 
                                    uint32_t instructionMode, 
                                    uint32_t addressMode, 
                                    uint32_t addressSize, 
                                    uint32_t dataMode);

HAL_StatusTypeDef QSPI_Transmit(uint8_t* send_buf, uint32_t size);
HAL_StatusTypeDef QSPI_Receive(uint8_t* recv_buf, uint32_t size);


/* W25Q64操作函数 */
uint16_t W25QXX_ReadID(void);
void W25QXX_Read(uint8_t* dat_buffer, uint32_t start_read_addr, uint16_t byte_to_read);
uint8_t W25QXX_ReadSR(uint8_t reg);
void W25QXX_Wait_Busy(void);
void W25QXX_Write_Enable(void);
void W25QXX_Write_Disable(void);
void W25QXX_Erase_Sector(uint32_t sector_addr);
void W25QXX_Page_Program(uint8_t* dat, uint32_t WriteAddr, uint16_t byte_to_write);



#endif /* _BSP_W25Q64_H_ */
