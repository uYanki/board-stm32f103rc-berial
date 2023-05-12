/**
 * @file    bsp_w25q64.c
 * @brief   W25Q64驱动程序
 * @author  Mculover666(http://www.mculover666.cn)
 * @note    此驱动程序基于STM32CubeMX生成的QSPI初始化工程
*/


#include "bsp_w25q64.h"

/**
 * @brief        向SPI Flash发送指令
 * @param        instruction —— 要发送的指令
 * @param        address     —— 要发送的地址
 * @param        dummyCycles    —— 空指令周期数
 * @param        instructionMode —— 指令发送模式
 * @param        addressMode —— 地址发送模式
 * @param        addressSize    —— 地址大小
 * @param        dataMode    —— 数据发送模式
 * @retval        成功返回HAL_OK
*/
HAL_StatusTypeDef QSPI_Send_Command(uint32_t instruction, 
                                    uint32_t address, 
                                    uint32_t dummyCycles, 
                                    uint32_t instructionMode, 
                                    uint32_t addressMode, 
                                    uint32_t addressSize, 
                                    uint32_t dataMode)
{
    QSPI_CommandTypeDef cmd;

    cmd.Instruction = instruction;                     //指令
    cmd.Address = address;                          //地址
    cmd.DummyCycles = dummyCycles;                  //设置空指令周期数
    cmd.InstructionMode = instructionMode;            //指令模式
    cmd.AddressMode = addressMode;                   //地址模式
    cmd.AddressSize = addressSize;                   //地址长度
    cmd.DataMode = dataMode;                         //数据模式
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;           //每次都发送指令
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; //无交替字节
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;               //关闭DDR模式
    cmd.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;

    return HAL_QSPI_Command(&hqspi, &cmd, 5000);
}

/**
* @brief    QSPI发送指定长度的数据
* @param    buf  —— 发送数据缓冲区首地址
* @param    size —— 要发送数据的字节数
 * @retval    成功返回HAL_OK
 */
HAL_StatusTypeDef QSPI_Transmit(uint8_t* send_buf, uint32_t size)
{
    hqspi.Instance->DLR = size - 1;                         //配置数据长度
    return HAL_QSPI_Transmit(&hqspi, send_buf, 5000);        //接收数据
}

/**
 * @brief   QSPI接收指定长度的数据
 * @param   buf  —— 接收数据缓冲区首地址
 * @param   size —— 要接收数据的字节数
 * @retval  成功返回HAL_OK
 */
HAL_StatusTypeDef QSPI_Receive(uint8_t* recv_buf, uint32_t size)
{
    hqspi.Instance->DLR = size - 1;                       //配置数据长度
    return HAL_QSPI_Receive(&hqspi, recv_buf, 5000);            //接收数据
}


/**
 * @brief   读取Flash内部的ID
 * @param   none
 * @retval    成功返回device_id
 */
uint16_t W25QXX_ReadID(void)
{
    uint8_t recv_buf[2] = {0};    //recv_buf[0]存放Manufacture ID, recv_buf[1]存放Device ID
    uint16_t device_id = 0;
    if(HAL_OK == QSPI_Send_Command(ManufactDeviceID_CMD, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_24_BITS, QSPI_DATA_1_LINE))
    {
        //读取ID
        if(HAL_OK == QSPI_Receive(recv_buf, 2))
        {
            device_id = (recv_buf[0] << 8) | recv_buf[1];
            return device_id;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/**
 * @brief    读取SPI FLASH数据
 * @param   dat_buffer —— 数据存储区
 * @param   start_read_addr —— 开始读取的地址(最大32bit)
 * @param   byte_to_read —— 要读取的字节数(最大65535)
 * @retval  none
 */
void W25QXX_Read(uint8_t* dat_buffer, uint32_t start_read_addr, uint16_t byte_to_read)
{
	  W25QXX_Wait_Busy();
    QSPI_Send_Command(READ_DATA_CMD, start_read_addr, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_24_BITS, QSPI_DATA_1_LINE);
    QSPI_Receive(dat_buffer, byte_to_read);
}

/**
 * @brief    读取W25QXX的状态寄存器，W25Q64一共有2个状态寄存器
 * @param     reg  —— 状态寄存器编号(1~2)
 * @retval    状态寄存器的值
 */
uint8_t W25QXX_ReadSR(uint8_t reg)
{
    uint8_t cmd = 0, result = 0;    
    switch(reg)
    {
        case 1:
            /* 读取状态寄存器1的值 */
            cmd = READ_STATU_REGISTER_1;
        case 2:
            cmd = READ_STATU_REGISTER_2;
        case 0:
        default:
            cmd = READ_STATU_REGISTER_1;
    }
    QSPI_Send_Command(cmd, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_24_BITS, QSPI_DATA_1_LINE);
    QSPI_Receive(&result, 1);

    return result;
}

/**
 * @brief    阻塞等待Flash处于空闲状态
 * @param   none
 * @retval  none
 */
void W25QXX_Wait_Busy(void)
{
    while((W25QXX_ReadSR(1) & 0x01) == 0x01); // 等待BUSY位清空
}

/**
 * @brief    W25QXX写使能,将S1寄存器的WEL置位
 * @param    none
 * @retval
 */
void W25QXX_Write_Enable(void)
{
    QSPI_Send_Command(WRITE_ENABLE_CMD, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE);
        W25QXX_Wait_Busy();
}

/**
 * @brief    W25QXX写禁止,将WEL清零
 * @param    none
 * @retval    none
 */
void W25QXX_Write_Disable(void)
{
    QSPI_Send_Command(WRITE_DISABLE_CMD, 0, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_NONE, QSPI_ADDRESS_8_BITS, QSPI_DATA_NONE);
        W25QXX_Wait_Busy();
}

/**
 * @brief    W25QXX擦除一个扇区
 * @param   sector_addr    —— 扇区地址 根据实际容量设置
 * @retval  none
 * @note    阻塞操作
 */
void W25QXX_Erase_Sector(uint32_t sector_addr)
{
    sector_addr *= 4096;    //每个块有16个扇区，每个扇区的大小是4KB，需要换算为实际地址
    W25QXX_Write_Enable();  //擦除操作即写入0xFF，需要开启写使能
    W25QXX_Wait_Busy();        //等待写使能完成
    QSPI_Send_Command(SECTOR_ERASE_CMD, sector_addr, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_24_BITS, QSPI_DATA_NONE);
    W25QXX_Wait_Busy();       //等待扇区擦除完成
}

/**
 * @brief    页写入操作
 * @param    dat —— 要写入的数据缓冲区首地址
 * @param    WriteAddr —— 要写入的地址
 * @param   byte_to_write —— 要写入的字节数（0-256）
 * @retval    none
 */
void W25QXX_Page_Program(uint8_t* dat, uint32_t WriteAddr, uint16_t byte_to_write)
{
    W25QXX_Write_Enable();
    QSPI_Send_Command(PAGE_PROGRAM_CMD, WriteAddr, 0, QSPI_INSTRUCTION_1_LINE, QSPI_ADDRESS_1_LINE, QSPI_ADDRESS_24_BITS, QSPI_DATA_1_LINE);
    QSPI_Transmit(dat, byte_to_write);
    W25QXX_Wait_Busy();
}
