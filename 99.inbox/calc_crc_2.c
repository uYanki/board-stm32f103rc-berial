
#include "mlx90614.h"

uint8_t mlx90614_calculate_crc(uint8_t arr[], uint8_t len)
{
    uint8_t i, j, byte, carry, crc = 0;
#if 0
    for (i = 0; i < len; ++i) {
        byte = arr[i];
        for (j = 8; j != 0; --j) {
            carry = (crc ^ byte) & 0x80;
            crc <<= 1;
            if (carry) crc ^= 0x7;
            byte <<= 1;
        }
    }
#else
    for (i = 0; i < len; ++i) {
        byte = crc ^ arr[i];
        for (j = 0; j < 8; ++j) {
            carry = byte & 0x80;
            byte <<= 1;
            if (carry) byte ^= 0x7;
        }
        crc = byte;
    }
#endif
    return crc;
}

bool mlx90614_read_2byte_crc(uint8_t reg, uint16_t* data)
{
    uint8_t lsb, msb, pec, buff[5];

    if (i2c_read_mem(&MLX90614_I2C, MLX90614_DEV, reg, buff, 3) == I2C_OK) {
        lsb = buff[0];
        msb = buff[1];
        pec = buff[2];
        // check crc
        buff[0] = MLX90614_DEV;    // set read addr
        buff[1] = reg;             // set regsiter
        buff[2] = buff[0] | 0x01;  // set write addr
        buff[3] = lsb;
        buff[4] = msb;
        if (mlx90614_calculate_crc(buff, 5) == pec) {
            // read success
            *data = (msb << 8) | lsb;
            return true;
        }
    }

    *data = 0;
    return false;
}

bool mlx90614_write_2byte_crc(uint8_t reg, uint16_t data)
{
    uint8_t buff[5];
    buff[0] = MLX90614_DEV;
    buff[1] = reg;
    buff[2] = data & 0xff;  // lsb
    buff[3] = data >> 8;    // msb
    buff[4] = mlx90614_calculate_crc(buff, 4);
    return (i2c_write_mem(&MLX90614_I2C, MLX90614_DEV, reg, &buff[2], 3) == I2C_OK) ? true : false;
}

bool mlx90614_read_bit_crc(uint8_t reg, uint8_t bit, uint8_t* data)
{
    uint16_t buff;
    if (mlx90614_read_2byte_crc(reg, &buff)) {
        *data = (buff >> bit) & 0x01;
        return true;
    } else {
        *data = 0;
        return false;
    }
}

bool mlx90614_write_bit_crc(uint8_t reg, uint8_t bit, uint8_t data)
{
    uint16_t buff;
    if (mlx90614_read_2byte_crc(reg, &buff)) {
        buff &= ~(0x01 << bit);
        buff |= (uint16_t)(data & 0x01) << bit;
        return mlx90614_write_eeprom(reg, buff);
    } else {
        return false;
    }
}

bool mlx90614_read_bits_crc(uint8_t reg, uint8_t start, uint8_t len, uint8_t* data)
{
    uint16_t buff;
    if (mlx90614_read_2byte_crc(reg, &buff)) {
        *data = (buff & (0xFFFF >> (16 - len) << start)) >> start;
        return true;
    } else {
        *data = 0;
        return false;
    }
}

bool mlx90614_write_bits_crc(uint8_t reg, uint8_t start, uint8_t len, uint8_t data)
{
    uint16_t buff, mask;
    if (mlx90614_read_2byte_crc(reg, &buff)) {
        mask = 0xFFFF >> (16 - len) << start;
        buff &= ~mask;
        buff |= mask & ((uint16_t)data << start);
        return mlx90614_write_eeprom(reg, buff);
    } else {
        return false;
    }
}

bool mlx90614_write_eeprom(uint8_t reg, uint16_t data)
{
    // clear out EEPROM first
    if (mlx90614_write_2byte_crc(reg, 0x00)) {
        // delay tErase at least 5 ms
        HAL_Delay(10);
        if (mlx90614_write_2byte_crc(reg, data)) {
            // delay tErase at least 5 ms
            HAL_Delay(10);
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////

// bit[2:0]
bool mlx90614_get_iir(mlx90614_iir_t* iir)
{
    return mlx90614_read_bits_crc(MLX90614_REG_CONFIG1, 0, 3, (uint8_t*)iir);
}

bool mlx90614_set_iir(mlx90614_iir_t iir)
{
    return mlx90614_write_bits_crc(MLX90614_REG_CONFIG1, 0, 3, iir);
}

// bit[3]

bool mlx90614_get_repeat_sensor_test(bool* enable)
{
    return mlx90614_read_bit_crc(MLX90614_REG_CONFIG1, 3, (uint8_t*)enable);
}

bool mlx90614_set_repeat_sensor_test(bool enable)
{
    return mlx90614_write_bit_crc(MLX90614_REG_CONFIG1, 3, enable);
}

// bit[5,4]
bool mlx90614_get_mode(mlx90614_mode_t* mode)
{
    return mlx90614_read_bits_crc(MLX90614_REG_CONFIG1, 4, 2, (uint8_t*)mode);
}

bool mlx90614_set_mode(mlx90614_mode_t mode)
{
    return mlx90614_write_bits_crc(MLX90614_REG_CONFIG1, 4, 2, mode);
}

// bit[6]
bool mlx90614_get_ir_sensor(mlx90614_ir_sensor_t* sensor)
{
    return mlx90614_read_bit_crc(MLX90614_REG_CONFIG1, 6, (uint8_t*)sensor);
}

bool mlx90614_set_ir_sensor(mlx90614_ir_sensor_t sensor)
{
    return mlx90614_write_bit_crc(MLX90614_REG_CONFIG1, 6, sensor);
}

// bit[7]
bool mlx90614_get_ks(mlx90614_ks_t* ks)
{
    return mlx90614_read_bit_crc(MLX90614_REG_CONFIG1, 7, (uint8_t*)ks);
}

bool mlx90614_set_ks(mlx90614_ks_t ks)
{
    return mlx90614_write_bit_crc(MLX90614_REG_CONFIG1, 7, ks);
}

// bit[10,8]

bool mlx90614_get_fir_length(mlx90614_fir_length_t* len)
{
    return mlx90614_read_bits_crc(MLX90614_REG_CONFIG1, 8, 3, (uint8_t*)len);
}

bool mlx90614_set_fir_length(mlx90614_fir_length_t len)
{
    return mlx90614_write_bits_crc(MLX90614_REG_CONFIG1, 8, 3, len);
}

// bit[13:11]
bool mlx90614_get_gain(mlx90614_gain_t* gain)
{
    return mlx90614_read_bits_crc(MLX90614_REG_CONFIG1, 11, 3, (uint8_t*)gain);
}

bool mlx90614_set_gain(mlx90614_gain_t gain)
{
    return mlx90614_write_bits_crc(MLX90614_REG_CONFIG1, 11, 3, gain);
}

// bit[14]
bool mlx90614_get_kt2(mlx90614_kt2_t* kt2)
{
    return mlx90614_read_bit_crc(MLX90614_REG_CONFIG1, 14, (uint8_t*)kt2);
}

bool mlx90614_set_kt2(mlx90614_kt2_t kt2)
{
    return mlx90614_write_bit_crc(MLX90614_REG_CONFIG1, 14, kt2);
}

// bit[15]

// enable or disable the sensor test
bool mlx90614_get_sensor_test(bool* enable)
{
    return mlx90614_read_bit_crc(MLX90614_REG_CONFIG1, 15, (uint8_t*)enable);
}

// get the sensor test status
bool mlx90614_set_sensor_test(bool enable)
{
    return mlx90614_write_bit_crc(MLX90614_REG_CONFIG1, 15, enable);
}

////////////////////////////////////////////

bool mlx90614_get_raw_ir_channel_1(uint16_t* data)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_RAWIR1, data);
}

bool mlx90614_get_raw_ir_channel_2(uint16_t* data)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_RAWIR2, data);
}

bool mlx90614_get_flag(uint16_t* flag)
{
    return mlx90614_read_1byte(COMMAND_READ_FLAGS);
}

bool mlx90614_enter_sleep_mode(void)
{
    uint8_t buff[2] = {MLX90614_DEV, COMMAND_ENTER_SLEEP};
    uint8_t crc     = mlx90614_calculate_crc((uint8_t*)buff, 2);
    return mlx90614_write_1byte(COMMAND_ENTER_SLEEP, crc) == I2C_OK;
}

float mlx90614_convert_raw_to_temp(int16_t raw, mlx90614_temp_unit unit)
{
    float temp = raw;
    if (unit > MLX90614_TEMP_UNIT_RAW) {
        temp *= 0.02;
        if (unit > MLX90614_TEMP_UNIT_KELVIN) {
            temp -= 273.15;
            if (unit > MLX90614_TEMP_UNIT_CENTIGRADE) {
                temp = temp * 9 / 5 + 32;
            }
        }
    }
    return temp;
}

int16_t mlx90614_convert_temp_to_raw(float temp, mlx90614_temp_unit unit)
{
    switch (unit) {
        case MLX90614_TEMP_UNIT_FAHRENHEIT: temp = (temp - 32) * 5 / 9;
        case MLX90614_TEMP_UNIT_CENTIGRADE: temp += 273.15;
        case MLX90614_TEMP_UNIT_KELVIN: temp *= 50;  // multiply by 0.02 degK / bit
        case MLX90614_TEMP_UNIT_RAW: break;
    }
    return temp;
}

////////////////////////////////////////////

bool mlx90614_get_object1_temp(int16_t* raw)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_TOBJ1, (uint16_t*)raw) && ((*raw & 0x8000) == 0) /* check error flag */;
}

bool mlx90614_get_object2_temp(int16_t* raw)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_TOBJ2, (uint16_t*)raw) && ((*raw & 0x8000) == 0) /* check error flag */;
}
bool mlx90614_get_ambient_temp(int16_t* raw)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_TA, (uint16_t*)raw);
}

bool mlx90614_get_min_temp(int16_t* raw)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_TO_MIN, (uint16_t*)raw);
}

bool mlx90614_get_max_temp(int16_t* raw)
{
    return mlx90614_read_2byte_crc(MLX90614_REG_TO_MAX, (uint16_t*)raw);
}

bool mlx90614_set_min_temp(int16_t raw)
{
    return mlx90614_write_eeprom(MLX90614_REG_TO_MIN, raw);
}

bool mlx90614_set_max_temp(int16_t raw)
{
    return mlx90614_write_eeprom(MLX90614_REG_TO_MAX, raw);
}

bool mlx90614_get_chip_id(uint16_t ID[4])
{
    return mlx90614_read_2byte_crc(MLX90614_REG_ID1, &ID[0]) &&
           mlx90614_read_2byte_crc(MLX90614_REG_ID2, &ID[1]) &&
           mlx90614_read_2byte_crc(MLX90614_REG_ID3, &ID[2]) &&
           mlx90614_read_2byte_crc(MLX90614_REG_ID4, &ID[3]);
}

bool mlx90614_get_emissivity(float* emissivity)
{
    uint16_t buff;
    if (mlx90614_read_2byte_crc(MLX90614_REG_EMISSIVITY, &buff)) {
        *emissivity = buff / 65535.0f;  // calculate the emissivity between 0.1 and 1.0
        return true;
    } else {
        *emissivity = 0.0f;
        return false;
    }
}

uint8_t mlx90614_set_emissivity(float emissivity)
{
    if ((emissivity > 1.0f) || (emissivity < 0.1f)) return false;
    uint16_t buff = emissivity * 65535;  // 0x2000 ~ 0xFFFF
    if (buff < 0x2000) buff = 0x2000;
    return mlx90614_write_eeprom(MLX90614_REG_EMISSIVITY, buff);
}

bool mlx90614_get_address(uint8_t* address)
{
    uint16_t buff;
    if (mlx90614_read_2byte_crc(MLX90614_REG_ADDRESS, &buff)) {
        *address = (uint8_t)buff;
        return true;
    } else {
        *address = 0x00;
        return false;
    }
}

// 注：修改地址后，需修改宏定义 MLX90614_DEV 的值并重新烧录固件！！
bool mlx90614_set_address(uint8_t address)
{
    if ((address >= 0x80) || (address == 0x00)) return 0;
    return mlx90614_write_eeprom(MLX90614_REG_ADDRESS, address);
}

// change pwm mode to smbus mode
void mlx90614_pwm_to_smbus()
{
    // scl_write(0);
    // delay_ms(5);
    // scl_write(1);
}

void mlx90614_exit_sleep_mode()
{
    // scl_write(1);
    // sda_write(1);
    // delay_ms(1);
    // sda_write(0);
    // delay_ms(50);
    // sda_write(1);
    // delay_ms(260);
}
