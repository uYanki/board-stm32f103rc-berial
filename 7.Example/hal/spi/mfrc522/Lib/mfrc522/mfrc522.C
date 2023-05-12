#include "mfrc522.h"

uint8_t RC522_ReadRegister(uint8_t Address) {
    uint8_t val, none = 0x00;
    RC522_CS_RESET;
    Address = ((Address << 1) & 0x7E) | 0x80;
    HAL_SPI_Transmit(&hspi3, &Address, 1, 0xFF);
    HAL_SPI_TransmitReceive(&hspi3, &none, &val, 1, 0xFF);
    RC522_CS_SET;
    return val;
}

void RC522_WriteRegister(uint8_t Address, uint8_t value) {
    uint8_t buff[2] = {(Address << 1) & 0x7E, value};
    RC522_CS_RESET;
    HAL_SPI_Transmit(&hspi3, buff, 2, 0xFF);
    RC522_CS_SET;
}

void RC522_SetBitMask(uint8_t reg, uint8_t mask) {
    RC522_WriteRegister(reg, RC522_ReadRegister(reg) | mask);
}

void RC522_ClearBitMask(uint8_t reg, uint8_t mask) {
    RC522_WriteRegister(reg, RC522_ReadRegister(reg) & (~mask));  // clear bit mask
}

uint8_t RC522_Reset(void) {
    RC522_CS_SET;
    HAL_Delay(8);
    RC522_RST_RESET;
    HAL_Delay(1);
    RC522_RST_SET;
    HAL_Delay(1);
    RC522_WriteRegister(COMMAND_REGISTER, RC522_RESETPHASE);
    HAL_Delay(1);

    // 配置模式
    RC522_WriteRegister(MODE_REGISTER, 0x29);

    // 配置16位定时器的高低位
    RC522_WriteRegister(TIMER_RELOAD_H_REGISTER, 0);
    RC522_WriteRegister(TIMER_RELOAD_L_REGISTER, 30);

    RC522_WriteRegister(TMODE_REGISTER, 0x8D);  // Tauto=1; f(Timer) = 6.78MHz/TPreScaler

    // 设置定时器分频系数
    RC522_WriteRegister(TIMER_PRESCALER_REGISTER, 0x3E);

    // 调制发送信号为100%ASK
    RC522_WriteRegister(TX_ASK_REGISTER, 0x40);

    return MI_OK;
}

/**
  * @brief 寻卡
  * @param  req_code，寻卡方式 = 0x52，寻感应区内所有符合14443A标准的卡；
            寻卡方式= 0x26，寻未进入休眠状态的卡
  * @param  pTagType，卡片类型代码
             = 0x4400，Mifare_UltraLight
             = 0x0400，Mifare_One(S50)
             = 0x0200，Mifare_One(S70)
             = 0x0800，Mifare_Pro(X))
             = 0x4403，Mifare_DESFire
  * @retval 状态值 = MI_OK，成功
  */
uint8_t RC522_Request(uint8_t req_code, uint8_t* pTagType) {
    uint8_t  status = MI_ERR;
    uint16_t Length;
    uint8_t  Buffer[MAXRLEN];

    // 清理指示MIFARECyptol单元接通以及所有卡的数据通信被加密的情况
    RC522_ClearBitMask(STATUS2_REGISTER, 0x08);
    // 发送的最后一个字节的 七位
    RC522_WriteRegister(BIT_FRAMING_REGISTER, 0x07);
    // TX1,TX2管脚的输出信号传递经发送调制的13.56的能量载波信号
    RC522_SetBitMask(TX_CONTROL_REGISTER, 0x03);

    Buffer[0] = req_code;  // 存入 卡片命令字

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 1, Buffer, &Length);  // 寻卡

    if ((status == MI_OK) && (Length == 0x10)) {
        // 寻卡成功返回卡类型
        *pTagType       = Buffer[0];
        *(pTagType + 1) = Buffer[1];
    }

    return status;
}

/**
 * @brief  防冲撞
 * @param  pSnr，卡片序列号，4字节
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_Anticoll(uint8_t* pSnr) {
    uint8_t  status;
    uint8_t  i, snr_check = 0;
    uint16_t Length;
    uint8_t  Buffer[MAXRLEN];

    // 清MFCryptol On位 只有成功执行MFAuthent命令后，该位才能置位
    RC522_ClearBitMask(STATUS2_REGISTER, 0x08);
    // 清理寄存器 停止收发
    RC522_WriteRegister(BIT_FRAMING_REGISTER, 0x00);
    // 清ValuesAfterColl所有接收的位在冲突后被清除
    RC522_ClearBitMask(COLLISION_REGISTER, 0x80);

    Buffer[0] = PICC_ANTICOLL1;  // 卡片防冲突命令
    Buffer[1] = 0x20;

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 2, Buffer, &Length);  // 与卡片通信

    if (status == MI_OK) {         // 通信成功
        for (i = 0; i < 4; i++) {  // 读 UID
            *(pSnr + i) = Buffer[i];
            snr_check ^= Buffer[i];
        }
        if (snr_check != Buffer[i]) { status = MI_ERR; }
    }

    RC522_SetBitMask(COLLISION_REGISTER, 0x80);
    return status;
}

/**
 * @brief  选定卡片
 * @param  pSnr，卡片序列号，4字节
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_Select(uint8_t* pSnr) {
    uint8_t  status;
    uint8_t  i;
    uint16_t Length;
    uint8_t  Buffer[MAXRLEN];

    Buffer[0] = PICC_ANTICOLL1;
    Buffer[1] = 0x70;
    Buffer[6] = 0;
    for (i = 0; i < 4; i++) {
        Buffer[i + 2] = *(pSnr + i);
        Buffer[6] ^= *(pSnr + i);
    }
    CalulateCRC(Buffer, 7, &Buffer[7]);

    RC522_ClearBitMask(STATUS2_REGISTER, 0x08);

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 9, Buffer, &Length);

    if ((status == MI_OK) && (Length == 0x18)) {
        status = MI_OK;
    } else {
        status = MI_ERR;
    }

    return status;
}

/**
 * @brief  验证卡片密码
 * @param  auth_mode，密码验证模式= 0x60，验证A密钥，
 *                    密码验证模式= 0x61，验证B密钥
 * @param  addr，块地址
 * @param  pKey，密码
 * @param  pSnr，卡片序列号，4字节
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_AuthState(uint8_t auth_mode, uint8_t addr, uint8_t* pKey, uint8_t* pSnr) {
    uint8_t  status;
    uint16_t Length;
    uint8_t  i, Buffer[MAXRLEN];

    Buffer[0] = auth_mode;
    Buffer[1] = addr;
    for (i = 0; i < 6; i++)
        Buffer[i + 2] = *(pKey + i);
    for (i = 0; i < 4; i++)
        Buffer[i + 8] = *(pSnr + i);
    status = RC522_ComMF522(RC522_AUTHENT, Buffer, 12, Buffer, &Length);
    if ((status != MI_OK) || (!(RC522_ReadRegister(STATUS2_REGISTER) & 0x08))) status = MI_ERR;

    return status;
}

/**
 * @brief  读取M1卡一块数据
 * @param  addr，块地址（0-63）。M1卡总共有16个扇区(每个扇区有：3个数据块+1个控制块),共64个块
 * @param  pData，读出的数据，16字节
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_Read(uint8_t addr, uint8_t* pData) {
    uint8_t  status;
    uint16_t Length;
    uint8_t  i, Buffer[MAXRLEN];

    Buffer[0] = PICC_READ;
    Buffer[1] = addr;
    CalulateCRC(Buffer, 2, &Buffer[2]);

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);
    if ((status == MI_OK) && (Length == 0x90)) {
        for (i = 0; i < 16; i++) *(pData + i) = Buffer[i];
    } else {
        status = MI_ERR;
    }

    return status;
}

/**
 * @brief  写数据到M1卡一块
 * @param  addr，块地址（0-63）。M1卡总共有16个扇区(每个扇区有：3个数据块+1个控制块),共64个块
 * @param  pData，写入的数据，16字节
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_Write(uint8_t addr, uint8_t* pData) {
    uint8_t  status;
    uint16_t Length;
    uint8_t  i, Buffer[MAXRLEN];

    Buffer[0] = PICC_WRITE;
    Buffer[1] = addr;
    CalulateCRC(Buffer, 2, &Buffer[2]);

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);

    if ((status != MI_OK) || (Length != 4) || ((Buffer[0] & 0x0F) != 0x0A)) status = MI_ERR;

    if (status == MI_OK) {
        for (i = 0; i < 16; ++i) Buffer[i] = *(pData + i);
        CalulateCRC(Buffer, 16, &Buffer[16]);
        status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 18, Buffer, &Length);
        if ((status != MI_OK) || (Length != 4) || ((Buffer[0] & 0x0F) != 0x0A)) status = MI_ERR;
    }

    return status;
}

uint8_t RC522_Value(uint8_t dd_mode, uint8_t addr, uint8_t* pValue) {
    uint8_t  status;
    uint16_t Length;
    uint8_t  i, Buffer[MAXRLEN];

    Buffer[0] = dd_mode;
    Buffer[1] = addr;
    CalulateCRC(Buffer, 2, &Buffer[2]);

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);

    if ((status != MI_OK) || (Length != 4) || ((Buffer[0] & 0x0F) != 0x0A)) { status = MI_ERR; }

    if (status == MI_OK) {
        for (i = 0; i < 16; i++) { Buffer[i] = *(pValue + i); }
        CalulateCRC(Buffer, 4, &Buffer[4]);
        Length = 0;
        status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 6, Buffer, &Length);
        if (status != MI_ERR) { status = MI_OK; }
    }

    if (status == MI_OK) {
        Buffer[0] = PICC_TRANSFER;
        Buffer[1] = addr;
        CalulateCRC(Buffer, 2, &Buffer[2]);

        status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);

        if ((status != MI_OK) || (Length != 4) || ((Buffer[0] & 0x0F) != 0x0A)) { status = MI_ERR; }
    }
    return status;
}

uint8_t RC522_BakValue(uint8_t sourceaddr, uint8_t goaladdr) {
    uint8_t  status;
    uint16_t Length;
    uint8_t  Buffer[MAXRLEN];

    Buffer[0] = PICC_RESTORE;
    Buffer[1] = sourceaddr;
    CalulateCRC(Buffer, 2, &Buffer[2]);

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);

    if ((status != MI_OK) || (Length != 4) || ((Buffer[0] & 0x0F) != 0x0A)) { status = MI_ERR; }

    if (status == MI_OK) {
        Buffer[0] = 0;
        Buffer[1] = 0;
        Buffer[2] = 0;
        Buffer[3] = 0;
        CalulateCRC(Buffer, 4, &Buffer[4]);

        status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 6, Buffer, &Length);
        if (status != MI_ERR) { status = MI_OK; }
    }

    if (status != MI_OK) { return MI_ERR; }

    Buffer[0] = PICC_TRANSFER;
    Buffer[1] = goaladdr;

    CalulateCRC(Buffer, 2, &Buffer[2]);

    status = RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);

    if ((status != MI_OK) || (Length != 4) || ((Buffer[0] & 0x0F) != 0x0A)) { status = MI_ERR; }

    return status;
}

/**
 * @brief  命令卡片进入休眠状态
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_Halt(void) {
    uint16_t Length;
    uint8_t  Buffer[MAXRLEN];
    Buffer[0] = PICC_HALT;
    Buffer[1] = 0;
    CalulateCRC(Buffer, 2, &Buffer[2]);
    // Ignore the returned status
    RC522_ComMF522(RC522_TRANSCEIVE, Buffer, 4, Buffer, &Length);
    return MI_OK;
}

void CalulateCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData) {
    uint8_t i, n;
    RC522_ClearBitMask(DIV_IRQ_REGISTER, 0x04);
    RC522_WriteRegister(COMMAND_REGISTER, RC522_IDLE);
    RC522_SetBitMask(FIFO_LEVEL_REGISTER, 0x80);
    for (i = 0; i < len; i++) { RC522_WriteRegister(FIFO_DATA_REGISTER, *(pIndata + i)); }
    RC522_WriteRegister(COMMAND_REGISTER, RC522_CALCCRC);
    i = 0xFF;
    do {
        n = RC522_ReadRegister(DIV_IRQ_REGISTER);
        i--;
    } while ((i != 0) && !(n & 0x04));
    pOutData[0] = RC522_ReadRegister(CRC_RESULT_L_REGISTER);
    pOutData[1] = RC522_ReadRegister(CRC_RESULT_M_REGISTER);
}

/**
 * @brief  通过RC522和ISO14443卡通讯
 * @param  Command，RC522命令字
 * @param  pInData，通过RC522发送到卡片的数据
 * @param  ucInLenByte，发送数据的字节长度
 * @param  pOutData，接收到的卡片返回数据
 * @param  pOutLenBit，返回数据的位长度
 * @retval 状态值 = MI_OK，成功
 */
uint8_t RC522_ComMF522(
    uint8_t   Command,
    uint8_t*  pInData,
    uint8_t   InLenByte,
    uint8_t*  pOutData,
    uint16_t* pOutLenBit) {
    uint8_t  status  = MI_ERR;
    uint8_t  irqEn   = 0x00;
    uint8_t  waitFor = 0x00;
    uint8_t  lastBits;
    uint8_t  n;
    uint16_t i;
    switch (Command) {
        case RC522_AUTHENT:  // Mifare认证
            irqEn   = 0x12;  // 允许错误中断请求 ErrIEn, 允许空闲中断 IdleIEn
            waitFor = 0x10;  // 认证寻卡等待的时候 查询空闲中断标志位
            break;
        case RC522_TRANSCEIVE:  // 接收发送 发送接收
            irqEn   = 0x77;     // 允许 TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
            waitFor = 0x30;     // 寻卡等待的时候 查询接收中断标志位与 空闲中断标志位
            break;
        default:
            break;
    }

    // IRqInv置位管脚IRQ与Status1Reg的IRq位的值相反
    RC522_WriteRegister(IE_REGISTER, irqEn | 0x80);
    // Set1该位清零时，CommIRqReg的屏蔽位清零
    RC522_ClearBitMask(IRQ_REGISTER, 0x80);
    // 写空闲命令
    RC522_WriteRegister(COMMAND_REGISTER, RC522_IDLE);
    // 置位FlushBuffer清除内部FIFO的读和写指针以及ErrReg的BufferOvfl标志位被清除
    RC522_SetBitMask(FIFO_LEVEL_REGISTER, 0x80);

    // 写数据进 FIFOdata
    for (i = 0; i < InLenByte; i++)
        RC522_WriteRegister(FIFO_DATA_REGISTER, pInData[i]);

    // 写命令
    RC522_WriteRegister(COMMAND_REGISTER, Command);
    if (Command == RC522_TRANSCEIVE)
        // StartSend 置位启动数据发送 该位与收发命令使用时才有效
        RC522_SetBitMask(BIT_FRAMING_REGISTER, 0x80);

    // 根据时钟频率调整，操作M1卡最大等待时间25ms
    i = 600;  // 25ms
    // 认证 与寻卡等待时间
    do {
        n = RC522_ReadRegister(IRQ_REGISTER);  // 查询事件中断
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));
    RC522_ClearBitMask(BIT_FRAMING_REGISTER, 0x80);  // 清理允许StartSend位

    if (i != 0) {
        // 读错误标志寄存器BufferOfI CollErr ParityErr ProtocolErr
        if (!(RC522_ReadRegister(ERROR_REGISTER) & 0x1B)) {
            status = MI_OK;
            // 是否发生定时器中断
            if (n & irqEn & 0x01)
                status = MI_NOTAGERR;

            if (Command == RC522_TRANSCEIVE) {
                // 读FIFO中保存的字节数
                n = RC522_ReadRegister(FIFO_LEVEL_REGISTER);
                // 最后接收到得字节的有效位数
                lastBits = RC522_ReadRegister(CONTROL_REGISTER) & 0x07;
                if (lastBits) {
                    // N个字节数减去1（最后一个字节）+最后一位的位数 读取到的数据总位数
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } else {
                    // 最后接收到的字节整个字节有效
                    *pOutLenBit = n * 8;
                }
                if (n == 0) {
                    n = 1;
                }
                if (n > MAXRLEN) {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++) {
                    pOutData[i] = RC522_ReadRegister(FIFO_DATA_REGISTER);
                }
            }
        } else {
            status = MI_ERR;
        }
    }
    RC522_SetBitMask(CONTROL_REGISTER, 0x80);  // stop timer now
    RC522_WriteRegister(COMMAND_REGISTER, RC522_IDLE);
    return status;
}

void RC522_AntennaOn() {
    uint8_t i;
    i = RC522_ReadRegister(TX_CONTROL_REGISTER);
    if (!(i & 0x03)) {
        RC522_SetBitMask(TX_CONTROL_REGISTER, 0x03);
    }
}

void RC522_AntennaOff() {
    RC522_ClearBitMask(TX_CONTROL_REGISTER, 0x03);
}

void dumpHex(uint8_t* buffer, int len) {
    uint8_t i;
    for (i = 0; i < len; i++) {
        if (i % 16 == 0) printf(" ");
        printf("%02X \x00", (uint8_t)(*(buffer + i)));
        if (i % 16 == 15) printf("\r\n");
    }
}

/**
 * @brief  判断地址是否数据块
 * @param  addr，块绝对地址（0-63）
 * @retval 返回值 1:是数据块；0:不是数据块
 */
uint8_t RC522_IsDataBlock(uint8_t addr) {
    if (addr == 0)  // 块 0 不可更改
        return 0;
    if (addr < 64)
        if ((addr + 1) % 4 != 0)
            return 1;
    return 0;
}
