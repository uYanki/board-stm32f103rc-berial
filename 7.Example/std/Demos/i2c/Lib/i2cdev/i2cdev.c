#include "i2cdev.h"

#if CONFIG_USING_I2C

//===============================================================================//

#if CONFIG_USING_SOFTWARE_I2C

struct i2c_drv swi2c_drv = {
    (i2cdrv_init)soft_i2c_init,
    (i2cdrv_check)soft_i2c_check,
    (i2cdrv_recv)soft_i2c_receive,
    (i2cdrv_send)soft_i2c_transmit,
    (i2cdrv_read)soft_i2c_read_mem,
    (i2cdrv_write)soft_i2c_write_mem,
};

//====================================//

#define SDA_L(bus) GPIO_ResetBits(bus->SDA_Port, bus->SDA_Pin)
#define SDA_H(bus) GPIO_SetBits(bus->SDA_Port, bus->SDA_Pin)

#define SCL_L(bus) GPIO_ResetBits(bus->SCL_Port, bus->SCL_Pin)
#define SCL_H(bus) GPIO_SetBits(bus->SCL_Port, bus->SCL_Pin)

#define SDA(bus)   GPIO_ReadInputDataBit(bus->SDA_Port, bus->SDA_Pin)

#define Delay(bus) delay_us(bus->Interval)

// 每个 Pin 占 4-bit，Port.CRL(32-bit) 配置 Pin0 ~ Pin7, Port.CRH(32-bit) 配置 Pin8 ~ Pin15

static void SDA_IN(soft_i2c_t bus)
{
    // GPIO_Mode_IN_FLOATING

    switch (bus->SDA_Pin) {
        case GPIO_Pin_0:
            bus->SDA_Port->CRL &= 0xFFFFFFF0;
            bus->SDA_Port->CRL |= 4 << 0;
            break;
        case GPIO_Pin_1:
            bus->SDA_Port->CRL &= 0xFFFFFF0F;
            bus->SDA_Port->CRL |= 4 << 4;
            break;
        case GPIO_Pin_2:
            bus->SDA_Port->CRL &= 0xFFFFF0FF;
            bus->SDA_Port->CRL |= 4 << 8;
            break;
        case GPIO_Pin_3:
            bus->SDA_Port->CRL &= 0xFFFF0FFF;
            bus->SDA_Port->CRL |= 4 << 12;
            break;
        case GPIO_Pin_4:
            bus->SDA_Port->CRL &= 0xFFF0FFFF;
            bus->SDA_Port->CRL |= 4 << 16;
            break;
        case GPIO_Pin_5:
            bus->SDA_Port->CRL &= 0xFF0FFFFF;
            bus->SDA_Port->CRL |= 4 << 20;
            break;
        case GPIO_Pin_6:
            bus->SDA_Port->CRL &= 0xF0FFFFFF;
            bus->SDA_Port->CRL |= 4 << 24;
            break;
        case GPIO_Pin_7:
            bus->SDA_Port->CRL &= 0x0FFFFFFF;
            bus->SDA_Port->CRL |= 4 << 28;
            break;
        case GPIO_Pin_8:
            bus->SDA_Port->CRH &= 0xFFFFFFF0;
            bus->SDA_Port->CRH |= 4 << 0;
            break;
        case GPIO_Pin_9:
            bus->SDA_Port->CRH &= 0xFFFFFF0F;
            bus->SDA_Port->CRH |= 4 << 4;
            break;
        case GPIO_Pin_10:
            bus->SDA_Port->CRH &= 0xFFFFF0FF;
            bus->SDA_Port->CRH |= 4 << 8;
            break;
        case GPIO_Pin_11:
            bus->SDA_Port->CRH &= 0xFFFF0FFF;
            bus->SDA_Port->CRH |= 4 << 12;
            break;
        case GPIO_Pin_12:
            bus->SDA_Port->CRH &= 0xFFF0FFFF;
            bus->SDA_Port->CRH |= 4 << 16;
            break;
        case GPIO_Pin_13:
            bus->SDA_Port->CRH &= 0xFF0FFFFF;
            bus->SDA_Port->CRH |= 4 << 20;
            break;
        case GPIO_Pin_14:
            bus->SDA_Port->CRH &= 0xF0FFFFFF;
            bus->SDA_Port->CRH |= 4 << 24;
            break;
        case GPIO_Pin_15:
            bus->SDA_Port->CRH &= 0x0FFFFFFF;
            bus->SDA_Port->CRH |= 4 << 28;
            break;
    }
}

static void SDA_OUT(soft_i2c_t bus)
{
    // GPIO_Mode_Out_OD, GPIO_Speed_50MHz

    switch (bus->SDA_Pin) {
        case GPIO_Pin_0:
            bus->SDA_Port->CRL &= 0xFFFFFFF0;
            bus->SDA_Port->CRL |= 7 << 0;
            break;
        case GPIO_Pin_1:
            bus->SDA_Port->CRL &= 0xFFFFFF0F;
            bus->SDA_Port->CRL |= 7 << 4;
            break;
        case GPIO_Pin_2:
            bus->SDA_Port->CRL &= 0xFFFFF0FF;
            bus->SDA_Port->CRL |= 7 << 8;
            break;
        case GPIO_Pin_3:
            bus->SDA_Port->CRL &= 0xFFFF0FFF;
            bus->SDA_Port->CRL |= 7 << 12;
            break;
        case GPIO_Pin_4:
            bus->SDA_Port->CRL &= 0xFFF0FFFF;
            bus->SDA_Port->CRL |= 7 << 16;
            break;
        case GPIO_Pin_5:
            bus->SDA_Port->CRL &= 0xFF0FFFFF;
            bus->SDA_Port->CRL |= 7 << 20;
            break;
        case GPIO_Pin_6:
            bus->SDA_Port->CRL &= 0xF0FFFFFF;
            bus->SDA_Port->CRL |= 7 << 24;
            break;
        case GPIO_Pin_7:
            bus->SDA_Port->CRL &= 0x0FFFFFFF;
            bus->SDA_Port->CRL |= 7 << 28;
            break;
        case GPIO_Pin_8:
            bus->SDA_Port->CRH &= 0xFFFFFFF0;
            bus->SDA_Port->CRH |= 7 << 0;
            break;
        case GPIO_Pin_9:
            bus->SDA_Port->CRH &= 0xFFFFFF0F;
            bus->SDA_Port->CRH |= 7 << 4;
            break;
        case GPIO_Pin_10:
            bus->SDA_Port->CRH &= 0xFFFFF0FF;
            bus->SDA_Port->CRH |= 7 << 8;
            break;
        case GPIO_Pin_11:
            bus->SDA_Port->CRH &= 0xFFFF0FFF;
            bus->SDA_Port->CRH |= 7 << 12;
            break;
        case GPIO_Pin_12:
            bus->SDA_Port->CRH &= 0xFFF0FFFF;
            bus->SDA_Port->CRH |= 7 << 16;
            break;
        case GPIO_Pin_13:
            bus->SDA_Port->CRH &= 0xFF0FFFFF;
            bus->SDA_Port->CRH |= 7 << 20;
            break;
        case GPIO_Pin_14:
            bus->SDA_Port->CRH &= 0xF0FFFFFF;
            bus->SDA_Port->CRH |= 7 << 24;
            break;
        case GPIO_Pin_15:
            bus->SDA_Port->CRH &= 0x0FFFFFFF;
            bus->SDA_Port->CRH |= 7 << 28;
            break;
    }
}

//====================================//

typedef enum {
    SOFT_I2C_ACK  = 0,
    SOFT_I2C_NACK = 1,
} soft_i2c_ack_t;

bool soft_i2c_init(soft_i2c_t bus)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(bus->RCC_APB2, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin  = bus->SCL_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏
    GPIO_Init(bus->SCL_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = bus->SDA_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏
    GPIO_Init(bus->SDA_Port, &GPIO_InitStructure);

    SDA_OUT(bus);
    SDA_H(bus);
    SCL_H(bus);
    return true;
}

void soft_i2c_start(soft_i2c_t bus)
{
    SDA_OUT(bus);
    SDA_H(bus);  // sda=1
    Delay(bus);
    SCL_H(bus);  // scl=1
    Delay(bus);
    SDA_L(bus);  // sda=0
    Delay(bus);
    SCL_L(bus);  // scl=0
    Delay(bus);
}

void soft_i2c_stop(soft_i2c_t bus)
{
    SDA_OUT(bus);
    SCL_L(bus);  // scl=0
    Delay(bus);
    SDA_L(bus);  // sda=0
    Delay(bus);
    SCL_H(bus);  // scl=1
    Delay(bus);
    SDA_H(bus);  // sda=1
    Delay(bus);
}

uint8_t soft_i2c_recv(soft_i2c_t bus, soft_i2c_ack_t ack)
{
    /* recv byte */

    uint8_t mask = 0x80, dat = 0x00;

    SDA_IN(bus);
    do {
        SCL_H(bus);
        Delay(bus);
        if (SDA(bus))
            dat |= mask;
        SCL_L(bus);
        Delay(bus);
    } while (mask >>= 1);

    /* send ack or nack */

    SDA_OUT(bus);
    SCL_L(bus);  // scl=0
    Delay(bus);
    switch (ack) {
        case SOFT_I2C_NACK:
            SDA_H(bus);  // sda=1
            break;
        default:
        case SOFT_I2C_ACK:
            SDA_L(bus);  // sda=0
            break;
    }
    Delay(bus);
    SCL_H(bus);  // scl=1
    Delay(bus);
    SCL_L(bus);  // scl=0
    Delay(bus);

    return dat;
}

bool soft_i2c_send(soft_i2c_t bus, uint8_t dat)
{
    uint8_t mask = 0x80;

    uint8_t timeout = 0;

    soft_i2c_ack_t ack = SOFT_I2C_ACK;  // ack

    /* send byte */

    SDA_OUT(bus);
    do {
        (dat & mask) ? SDA_H(bus) :  // sda = 1
            SDA_L(bus);              // sda = 0
        Delay(bus);
        SCL_H(bus);  // scl = 1
        Delay(bus);
        SCL_L(bus);  // scl = 0
        Delay(bus);
    } while (mask >>= 1);

    /* recv ack or nack */

    SDA_H(bus);  // sda=1
    Delay(bus);
    SCL_H(bus);  // scl=1
    Delay(bus);

    SDA_IN(bus);
    while (SDA(bus)) {
        if (++timeout > 250) {  // nack
            ack = SOFT_I2C_NACK;
            goto exit;
        }
    }

    SCL_L(bus);  // scl=0
    Delay(bus);

exit:
    return (ack == SOFT_I2C_ACK) ? true : false;
}

void soft_i2c_recvs(soft_i2c_t bus, uint8_t* dat, uint16_t len, soft_i2c_ack_t ack)
{
    if (len > 0) {
        while (--len)
            *dat++ = soft_i2c_recv(bus, SOFT_I2C_ACK);
        *dat = soft_i2c_recv(bus, ack);  // last
    }
}

bool soft_i2c_sends(soft_i2c_t bus, uint8_t* dat, uint16_t len)
{
    while (len--) {
        if (false == soft_i2c_send(bus, *dat++))
            return false;
    }
    return true;
}

//====================================//

/**
 * i2c write: lsb is 0
 * i2c read:  lsb is 1
 */

bool soft_i2c_check(soft_i2c_t bus, uint8_t dev)
{
    bool ret;
    soft_i2c_start(bus);
    ret = soft_i2c_send(bus, dev);
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_receive(soft_i2c_t bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev | 0x01))) goto exit;
    soft_i2c_recvs(bus, dat, len, SOFT_I2C_NACK);
exit:
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_transmit(soft_i2c_t bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev & 0xFE))) goto exit;
    if (false == (ret = soft_i2c_sends(bus, dat, len))) goto exit;
exit:
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_read_mem(soft_i2c_t bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev & 0xFE))) goto exit;
    if (false == (ret = soft_i2c_sends(bus, (uint8_t*)&reg, ops & I2C_REG_16BIT ? 2 : 1))) goto exit;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev | 0x01))) goto exit;
    soft_i2c_recvs(bus, dat, len, SOFT_I2C_NACK);
exit:
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_write_mem(soft_i2c_t bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev & 0xFE))) goto exit;
    if (false == (ret = soft_i2c_sends(bus, (uint8_t*)&reg, ops & I2C_REG_16BIT ? 2 : 1))) goto exit;
    if (false == (ret = soft_i2c_sends(bus, dat, len))) goto exit;
exit:
    soft_i2c_stop(bus);
    return ret;
}

#endif

//===============================================================================//

#if CONFIG_USING_HARDWARE_I2C

struct i2c_drv hwi2c_drv = {
    (i2cdrv_init)hard_i2c_init,
    (i2cdrv_check)hard_i2c_check,
    (i2cdrv_recv)hard_i2c_receive,
    (i2cdrv_send)hard_i2c_transmit,
    (i2cdrv_read)hard_i2c_read_mem,
    (i2cdrv_write)hard_i2c_write_mem,
};

// bind interfaces

/**
 * I2C1: PB6(SCL),PB7(SDA)
 *       PB8(SCL),PB9(SDA)
 * I2C2: PB10(SCL),PB11(SDA)
 **/

bool hard_i2c_init(hard_i2c_t bus)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;

    switch ((int)(bus->I2C)) {
        case (int)I2C1: {
            // rcc
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            // gpio
            if (!bus->RemapIO) {
                GPIO_PinRemapConfig(GPIO_Remap_I2C1, DISABLE);
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
            } else {
                RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
                GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
            }
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;  // open drain
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(GPIOB, &GPIO_InitStructure);
            // i2c
            I2C_DeInit(I2C1);
            I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
            I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
            I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // or 10bit
            I2C_InitStructure.I2C_ClockSpeed          = bus->Speed;
            I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
            I2C_InitStructure.I2C_OwnAddress1         = bus->OwnAddr;  // self 7-bit or 10-bit addr
            I2C_Init(I2C1, &I2C_InitStructure);
            I2C_Cmd(I2C1, ENABLE);
            break;
        }
        case (int)I2C2: {
            // rcc
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
            // gpio
            GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
            GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(GPIOB, &GPIO_InitStructure);
            // i2c
            I2C_DeInit(I2C2);
            I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
            I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
            I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
            I2C_InitStructure.I2C_ClockSpeed          = bus->Speed;
            I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
            I2C_InitStructure.I2C_OwnAddress1         = bus->OwnAddr;
            I2C_Init(I2C2, &I2C_InitStructure);
            I2C_Cmd(I2C2, ENABLE);
            break;
        }
        default: {
            return false;
        }
    }

    return true;
}

bool hard_i2c_check(hard_i2c_t bus, uint16_t dev)
{
    uint32_t timeout;

    bool ret = false;

    /* busy? */
    timeout = bus->Timeout;
    while (I2C_GetFlagStatus(bus->I2C, I2C_FLAG_BUSY)) {
        if (timeout-- == 0) goto exit;
    }

    /* start */
    timeout = bus->Timeout;
    I2C_GenerateSTART(bus->I2C, ENABLE);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (timeout-- == 0) goto exit;
    }

    /* send addr */
    timeout = bus->Timeout;
    I2C_Send7bitAddress(bus->I2C, dev, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (timeout-- == 0) goto exit;
    }

    /* stop */
    ret = true;
exit:
    I2C_GenerateSTOP(bus->I2C, ENABLE);
    return ret;
}

bool hard_i2c_receive(hard_i2c_t bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    uint32_t timeout;

    bool ret = false;

    if (len == 0) return ret;

    dev &= 0xFE;

#if 1

    /* busy? */
    timeout = bus->Timeout;
    while (I2C_GetFlagStatus(bus->I2C, I2C_FLAG_BUSY)) {
        if (timeout-- == 0) goto exit;
    }

#endif

    /* start */
    timeout = bus->Timeout;
    I2C_GenerateSTART(bus->I2C, ENABLE);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (timeout-- == 0) goto exit;
    }

    /* send addr */
    timeout = bus->Timeout;
    I2C_Send7bitAddress(bus->I2C, dev, I2C_Direction_Receiver);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if (timeout-- == 0) goto exit;
    }

    /* recv data */

    while (--len) {
        timeout = bus->Timeout;
        *dat++  = I2C_ReceiveData(bus->I2C);
        while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
            if (timeout-- == 0) goto exit;
        }
    }

    I2C_AcknowledgeConfig(bus->I2C, DISABLE);  // NACK

    timeout = bus->Timeout;
    *dat    = I2C_ReceiveData(bus->I2C);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if (timeout-- == 0) goto exit;
    }

    /* stop */
    ret = true;

    if (ops & I2C_NOSTOP) return ret;

exit:
    I2C_GenerateSTOP(bus->I2C, ENABLE);

    I2C_AcknowledgeConfig(bus->I2C, ENABLE);  // ACK

    return ret;
}

bool hard_i2c_transmit(hard_i2c_t bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    uint32_t timeout;

    bool ret = false;

    if (len == 0) return ret;

#if 1
    /* busy? */
    timeout = bus->Timeout;
    while (I2C_GetFlagStatus(bus->I2C, I2C_FLAG_BUSY)) {
        if (timeout-- == 0) goto exit;
    }
#endif

    /* start */
    timeout = bus->Timeout;
    I2C_GenerateSTART(bus->I2C, ENABLE);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (timeout-- == 0) goto exit;
    }

    /* send addr */
    timeout = bus->Timeout;
    I2C_Send7bitAddress(bus->I2C, dev, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (timeout-- == 0) goto exit;
    }

    /* send data */
    while (len--) {
        timeout = bus->Timeout;
        I2C_SendData(bus->I2C, *dat++);
        while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
            if (timeout-- == 0) goto exit;
        }
    }

    /* stop */
    ret = true;

    if (ops & I2C_NOSTOP) return ret;

exit:
    I2C_GenerateSTOP(bus->I2C, ENABLE);
    return ret;
}

bool hard_i2c_read_mem(hard_i2c_t bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    uint32_t timeout;

    bool ret = false;
    if (len == 0) return ret;

#if 1
    /* busy? */
    timeout = bus->Timeout;
    while (I2C_GetFlagStatus(bus->I2C, I2C_FLAG_BUSY)) {
        if (timeout-- == 0) goto exit;
    }
#endif

    /* start */
    timeout = bus->Timeout;
    I2C_GenerateSTART(bus->I2C, ENABLE);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (timeout-- == 0) goto exit;
    }

    /* send addr */
    timeout = bus->Timeout;
    I2C_Send7bitAddress(bus->I2C, dev, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (timeout-- == 0) goto exit;
    }

    /* send reg */

    if (ops & I2C_REG_16BIT) {
        timeout = bus->Timeout;
        I2C_SendData(bus->I2C, reg >> 8);
        while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
            if (timeout-- == 0) goto exit;
        }
    }

    timeout = bus->Timeout;
    I2C_SendData(bus->I2C, reg & 0xFF);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
        if (timeout-- == 0) goto exit;
    }

    /* start */
    timeout = bus->Timeout;
    I2C_GenerateSTART(bus->I2C, ENABLE);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (timeout-- == 0) goto exit;
    }

    /* send addr */
    timeout = bus->Timeout;
    I2C_Send7bitAddress(bus->I2C, dev, I2C_Direction_Receiver);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if (timeout-- == 0) goto exit;
    }

    /* recv data */

    while (--len) {
        timeout = bus->Timeout;
        while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
            if (timeout-- == 0) goto exit;
        }
        *dat++ = I2C_ReceiveData(bus->I2C);
    }

    I2C_AcknowledgeConfig(bus->I2C, DISABLE);  // NACK

    timeout = bus->Timeout;
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
        if (timeout-- == 0) goto exit;
    }
    *dat = I2C_ReceiveData(bus->I2C);

    /* stop */
    ret = true;

exit:

    I2C_GenerateSTOP(bus->I2C, ENABLE);

    I2C_AcknowledgeConfig(bus->I2C, ENABLE);  // ACK

    return ret;
}

bool hard_i2c_write_mem(hard_i2c_t bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    uint32_t timeout;

    bool ret = false;

    if (len == 0) return ret;

#if 1
    /* busy? */
    timeout = bus->Timeout;
    while (I2C_GetFlagStatus(bus->I2C, I2C_FLAG_BUSY)) {
        if (timeout-- == 0) goto exit;
    }
#endif

    /* start */
    timeout = bus->Timeout;
    I2C_GenerateSTART(bus->I2C, ENABLE);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
        if (timeout-- == 0) goto exit;
    }

    /* send addr */
    timeout = bus->Timeout;
    I2C_Send7bitAddress(bus->I2C, dev, I2C_Direction_Transmitter);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if (timeout-- == 0) goto exit;
    }

    /* send reg */

    if (ops & I2C_REG_16BIT) {
        timeout = bus->Timeout;
        I2C_SendData(bus->I2C, reg >> 8);
        while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
            if (timeout-- == 0) goto exit;
        }
    }

    timeout = bus->Timeout;
    I2C_SendData(bus->I2C, reg & 0xFF);
    while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
        if (timeout-- == 0) goto exit;
    }

    /* send data */
    while (len--) {
        timeout = bus->Timeout;
        I2C_SendData(bus->I2C, *dat++);
        while (ERROR == I2C_CheckEvent(bus->I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
            if (timeout-- == 0) goto exit;
        }
    }

    /* stop */
    ret = true;

exit:

    I2C_GenerateSTOP(bus->I2C, ENABLE);

    return ret;
}

#endif

//===============================================================================//

bool i2cdev_check(i2c_cli_t cli)
{
    return cli->drv->check(cli->bus, cli->dev);
}

bool i2cdev_recv_bytes(i2c_cli_t cli, uint8_t* dat, uint16_t len)
{
    return cli->drv->recv(cli->bus, cli->dev, dat, len, cli->ops);
}

bool i2cdev_send_bytes(i2c_cli_t cli, const uint8_t* dat, uint16_t len)
{
    return cli->drv->send(cli->bus, cli->dev, dat, len, cli->ops);
}

bool i2cdev_read_block(i2c_cli_t cli, uint16_t reg, uint8_t* dat, uint16_t len)
{
    return cli->drv->read(cli->bus, cli->dev, reg, dat, len, cli->ops);
}

bool i2cdev_write_block(i2c_cli_t cli, uint16_t reg, const uint8_t* dat, uint16_t len)
{
    return cli->drv->write(cli->bus, cli->dev, reg, dat, len, cli->ops);
}

bool i2cdev_recv_byte(i2c_cli_t cli, uint8_t* dat)
{
    return i2cdev_recv_bytes(cli, dat, 1);
}

bool i2cdev_send_byte(i2c_cli_t cli, const uint8_t dat)
{
    return i2cdev_send_bytes(cli, &dat, 1);
}

bool i2cdev_read_byte(i2c_cli_t cli, uint16_t reg, uint8_t* dat)
{
    return i2cdev_read_block(cli, reg, dat, 1);
}

bool i2cdev_read_word(i2c_cli_t cli, uint16_t reg, uint16_t* dat, uint8_t order)
{
    if (i2cdev_read_block(cli, reg, (uint8_t*)&dat, 2)) {
        switch (order) {
            default:
            case ORDER_FIRST_LOW:
                return true;
            case ORDER_FIRST_HIGH:
                *dat = (*dat << 8) | (*dat >> 8);
                return true;
        }
    }

    return false;
}

bool i2cdev_write_byte(i2c_cli_t cli, uint16_t reg, const uint8_t dat)
{
    return i2cdev_write_block(cli, reg, &dat, 1);
}

bool i2cdev_write_word(i2c_cli_t cli, uint16_t reg, const uint16_t dat, uint8_t order)
{
    if (i2cdev_write_block(cli, reg, (uint8_t*)&dat, 2)) {
        switch (order) {
            default:
            case ORDER_FIRST_LOW: {
                return i2cdev_write_block(cli, reg, (uint8_t*)&dat, 2);
            }
            case ORDER_FIRST_HIGH: {
                uint8_t buf[2];
                buf[0] = dat >> 8;
                buf[1] = dat & 0xff;
                return i2cdev_write_block(cli, reg, (const uint8_t*)buf, 2);
            }
        }
    }

    return false;
}

bool i2cdev_read_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t* dat)
{
    uint8_t buf;

    if (idx < 8 && i2cdev_read_byte(cli, reg, &buf)) {
        *dat = (buf >> idx) & 0x01;
        return true;
    }

    return false;
}

bool i2cdev_read_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, uint8_t* dat)
{
    uint8_t buf, mask;

    mask = 0xFF >> (8 - len) << idx;

    if (mask && i2cdev_read_byte(cli, reg, &buf)) {
        *dat = (buf & mask) >> idx;
        return true;
    }

    return false;
}

bool i2cdev_read_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, uint8_t* dat)
{
    uint8_t buf;

    if (i2cdev_read_byte(cli, reg, &buf)) {
        *dat = buf & mask;
        return true;
    }

    return false;
}

bool i2cdev_write_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, const uint8_t dat)
{
    uint8_t buf;

    if (i2cdev_read_byte(cli, reg, &buf)) {
        if (dat) {
            buf |= (1 << idx);
        } else {
            buf &= ~(1 << idx);
        }
        return i2cdev_write_byte(cli, reg, buf);
    }

    return false;
}

bool i2cdev_write_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, const uint8_t dat)
{
    return i2cdev_write_mask(cli, reg, 0xFF >> (8 - len) << idx, dat << idx);
}

bool i2cdev_write_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, const uint8_t dat)
{
    uint8_t buf;

    if (mask && i2cdev_read_byte(cli, reg, &buf)) {
        buf &= ~mask;
        buf |= (mask & dat);
        return i2cdev_write_byte(cli, reg, buf);
    }

    return false;
}

uint8_t i2cdev_get_byte(i2c_cli_t cli, uint16_t reg, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_byte(cli, reg, &buf) ? buf : def;
}

uint16_t i2cdev_get_word(i2c_cli_t cli, uint16_t reg, const uint16_t def, uint8_t order)
{
    uint16_t buf;
    return i2cdev_read_word(cli, reg, &buf, order) ? buf : def;
}

uint8_t i2cdev_get_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_bit(cli, reg, idx, &buf) ? buf : def;
}

uint8_t i2cdev_get_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_bits(cli, reg, idx, len, &buf) ? buf : def;
}

uint8_t i2cdev_get_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_mask(cli, reg, mask, &buf) ? buf : def;
}

bool i2cdev_viewer(i2c_cli_t cli, uint16_t start, uint16_t end, uint8_t fmt)
{
    uint8_t  dat, mask;
    uint16_t reg, len;

    if (start > end) swap_int(start, end);

    reg = start;
    len = end - start;

    println("i2cdev (0x%02X) - memory viewer ( from 0x%04X to 0x%04X ):", cli->dev, start, end);

    do {
        // address
        printf("[0x%04X] ", reg);
        // read
        if (i2cdev_read_byte(cli, reg, &dat)) {
            // value
            switch (fmt) {
                default:
                case UINT_FORMAT_BIN: {
                    for (mask = 0x80; mask > 0; mask >>= 1)
                        printf("%c", dat & mask ? '1' : '0');
                    break;
                }
                case UINT_FORMAT_DEC: {
                    printf("%d", dat);
                    break;
                }
                case UINT_FORMAT_HEX: {
                    printf("0x%02X", dat);
                    break;
                }
            }
            // newline
            println("");
        } else {
            println("fail to read");
            return false;
        }

        ++reg;
    } while (len--);

    return true;
}

uint8_t i2cdrv_detector(i2c_bus_t bus, i2c_drv_t drv)
{
    int addr = 0, step, cnt = 0;

    println("i2cdev 7-bit address detector:");
    println("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");

    while (addr < 0x7F) {
        printf("%02x: ", addr);

        step = 0x10;
        do {
            if (drv->check(bus, addr << 1)) {
                printf("%02x ", addr);
                ++cnt;
            } else {
                printf("-- ");
            }
            ++addr;
        } while (--step);

        println("");
    }

    println(">> %d devices detected in this scan", cnt);

    return cnt;
}

bool i2cdev_mapset(i2c_cli_t cli, uint8_t* dat, uint16_t len)
{
    uint16_t idx = 0;

    len &= 0xFFFE;

    while (idx < len) {
        if (i2cdev_write_byte(cli, dat[idx], dat[idx + 1])) {
            idx += 2;
        } else {
            return false;
        }
    }

    return true;
}

#endif
