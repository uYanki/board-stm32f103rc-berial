#include "i2cdev.h"

#if CONFIG_USING_I2C

//===============================================================================//

#if CONFIG_USING_SOFTWARE_I2C

//====================================//

#define SDA_L(bus) HAL_GPIO_WritePin(bus->SDA_Port, bus->SDA_Pin, GPIO_PIN_RESET)
#define SDA_H(bus) HAL_GPIO_WritePin(bus->SDA_Port, bus->SDA_Pin, GPIO_PIN_SET)

#define SCL_L(bus) HAL_GPIO_WritePin(bus->SCL_Port, bus->SCL_Pin, GPIO_PIN_RESET)
#define SCL_H(bus) HAL_GPIO_WritePin(bus->SCL_Port, bus->SCL_Pin, GPIO_PIN_SET)

#define SDA(bus)   HAL_GPIO_ReadPin(bus->SDA_Port, bus->SDA_Pin)

#define Delay(bus) HAL_Delay_us(bus->Interval)

static GPIO_InitTypeDef GPIO_InitStruct = {
    .Pull  = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

static void SDA_IN(soft_i2c_t* bus)
{
    GPIO_InitStruct.Pin  = bus->SDA_Pin,
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    HAL_GPIO_Init(bus->SDA_Port, &GPIO_InitStruct);
}

static void SDA_OUT(soft_i2c_t* bus)
{
    GPIO_InitStruct.Pin  = bus->SDA_Pin,
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD,
    HAL_GPIO_Init(bus->SDA_Port, &GPIO_InitStruct);
}

//====================================//

typedef enum {
    SOFT_I2C_ACK  = 0,
    SOFT_I2C_NACK = 1,
} soft_i2c_ack_t;

bool soft_i2c_init(soft_i2c_t* bus)
{
    SDA_OUT(bus);
    SDA_H(bus);
    SCL_H(bus);
    return true;
}

void soft_i2c_start(soft_i2c_t* bus)
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

void soft_i2c_stop(soft_i2c_t* bus)
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

uint8_t soft_i2c_recv(soft_i2c_t* bus, soft_i2c_ack_t ack)
{
    /* recv byte */

    uint8_t mask = 0x80, dat = 0x00;

    SDA_IN(bus);
    do {
        SCL_H(bus);
        Delay(bus);
        if (SDA(bus)) {
            dat |= mask;
        }
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

bool soft_i2c_send(soft_i2c_t* bus, uint8_t dat)
{
    /* send byte */

    uint8_t mask = 0x80;

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

    uint8_t timeout = 0;

    soft_i2c_ack_t ack = SOFT_I2C_ACK;  // ack

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

void soft_i2c_recvs(soft_i2c_t* bus, uint8_t* dat, uint16_t len, soft_i2c_ack_t ack)
{
    if (len > 0) {
        while (--len) {
            *dat++ = soft_i2c_recv(bus, SOFT_I2C_ACK);
        }
        *dat = soft_i2c_recv(bus, ack);  // last
    }
}

bool soft_i2c_sends(soft_i2c_t* bus, uint8_t* dat, uint16_t len)
{
    while (len--) {
        if (false == soft_i2c_send(bus, *dat++)) {
            return false;
        }
    }
    return true;
}

//====================================//

/**
 * i2c write: lsb is 0
 * i2c read:  lsb is 1
 */

bool soft_i2c_check(soft_i2c_t* bus, uint8_t dev)
{
    bool ret;
    soft_i2c_start(bus);
    ret = soft_i2c_send(bus, dev);
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_receive(soft_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev | 0x01))) {
        goto exit;
    }
    soft_i2c_recvs(bus, dat, len, SOFT_I2C_NACK);
exit:
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_transmit(soft_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev & 0xFE))) {
        goto exit;
    }
    if (false == (ret = soft_i2c_sends(bus, dat, len))) {
        goto exit;
    }
exit:
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_read_mem(soft_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev & 0xFE))) {
        goto exit;
    }
    if (false == (ret = soft_i2c_sends(bus, (uint8_t*)&reg, ops & I2C_REG_16BIT ? 2 : 1))) {
        goto exit;
    }
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev | 0x01))) {
        goto exit;
    }
    soft_i2c_recvs(bus, dat, len, SOFT_I2C_NACK);
exit:
    soft_i2c_stop(bus);
    return ret;
}

bool soft_i2c_write_mem(soft_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    bool ret;
    soft_i2c_start(bus);
    if (false == (ret = soft_i2c_send(bus, dev & 0xFE))) {
        goto exit;
    }
    if (false == (ret = soft_i2c_sends(bus, (uint8_t*)&reg, ops & I2C_REG_16BIT ? 2 : 1))) {
        goto exit;
    }
    if (false == (ret = soft_i2c_sends(bus, dat, len))) {
        goto exit;
    }
exit:
    soft_i2c_stop(bus);
    return ret;
}

#endif

//===============================================================================//

#if CONFIG_USING_HARDWARE_I2C

// bind interfaces

INLINE bool hard_i2c_init(hard_i2c_t* bus)
{
    return true;
}

INLINE bool hard_i2c_check(hard_i2c_t* bus, uint16_t dev)
{
    return HAL_I2C_IsDeviceReady(bus, dev, 5, 0xFFFF) == HAL_OK;
}

INLINE bool hard_i2c_receive(hard_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    return HAL_I2C_Master_Receive(bus, dev, dat, len, 0xFF) == HAL_OK;
}

INLINE bool hard_i2c_transmit(hard_i2c_t* bus, uint16_t dev, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    return HAL_I2C_Master_Transmit(bus, dev, dat, len, 0xFF) == HAL_OK;
}

INLINE bool hard_i2c_read_mem(hard_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    return HAL_I2C_Mem_Read(bus, dev, reg, ops & I2C_REG_16BIT ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, dat, len, 0xFF) == HAL_OK;
}

INLINE bool hard_i2c_write_mem(hard_i2c_t* bus, uint16_t dev, uint16_t reg, uint8_t* dat, uint16_t len, i2c_ops_t ops)
{
    return HAL_I2C_Mem_Write(bus, dev, reg, ops & I2C_REG_16BIT ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT, dat, len, 0xFF) == HAL_OK;
}

#endif

//===============================================================================//

INLINE bool i2cdev_check(i2c_cli_t cli)
{
    return cli->drv->check(cli->bus, cli->dev);
}

INLINE bool i2cdev_recv_bytes(i2c_cli_t cli, uint8_t* dat, uint16_t len)
{
    return cli->drv->recv(cli->bus, cli->dev, dat, len, cli->ops);
}

INLINE bool i2cdev_send_bytes(i2c_cli_t cli, const uint8_t* dat, uint16_t len)
{
    return cli->drv->send(cli->bus, cli->dev, dat, len, cli->ops);
}

INLINE bool i2cdev_read_block(i2c_cli_t cli, uint16_t reg, uint8_t* dat, uint16_t len)
{
    return cli->drv->read(cli->bus, cli->dev, reg, dat, len, cli->ops);
}

INLINE bool i2cdev_write_block(i2c_cli_t cli, uint16_t reg, const uint8_t* dat, uint16_t len)
{
    return cli->drv->write(cli->bus, cli->dev, reg, dat, len, cli->ops);
}

INLINE bool i2cdev_recv_byte(i2c_cli_t cli, uint8_t* dat)
{
    return i2cdev_recv_bytes(cli, dat, 1);
}

INLINE bool i2cdev_send_byte(i2c_cli_t cli, const uint8_t dat)
{
    return i2cdev_send_bytes(cli, &dat, 1);
}

INLINE bool i2cdev_read_byte(i2c_cli_t cli, uint16_t reg, uint8_t* dat)
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

INLINE bool i2cdev_write_byte(i2c_cli_t cli, uint16_t reg, const uint8_t dat)
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

INLINE bool i2cdev_read_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t* dat)
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

INLINE bool i2cdev_write_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, const uint8_t dat)
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

INLINE uint8_t i2cdev_get_byte(i2c_cli_t cli, uint16_t reg, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_byte(cli, reg, &buf) ? buf : def;
}

INLINE uint16_t i2cdev_get_word(i2c_cli_t cli, uint16_t reg, const uint16_t def, uint8_t order)
{
    uint16_t buf;
    return i2cdev_read_word(cli, reg, &buf, order) ? buf : def;
}

INLINE uint8_t i2cdev_get_bit(i2c_cli_t cli, uint16_t reg, uint8_t idx, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_bit(cli, reg, idx, &buf) ? buf : def;
}

INLINE uint8_t i2cdev_get_bits(i2c_cli_t cli, uint16_t reg, uint8_t idx, uint8_t len, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_bits(cli, reg, idx, len, &buf) ? buf : def;
}

INLINE uint8_t i2cdev_get_mask(i2c_cli_t cli, uint16_t reg, uint8_t mask, const uint8_t def)
{
    uint8_t buf;
    return i2cdev_read_mask(cli, reg, mask, &buf) ? buf : def;
}

bool i2cdev_viewer(i2c_cli_t cli, uint16_t start, uint16_t end, uint8_t fmt)
{
    if (start > end) {
        swap_int(start, end);
    }

    uint8_t  dat, mask;
    uint16_t reg = start;
    uint16_t len = end - start;

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
                    uint8_t i;
                    for (mask = 0x80; mask > 0; mask >>= 1) {
                        printf("%c", dat & mask ? '1' : '0');
                    }
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