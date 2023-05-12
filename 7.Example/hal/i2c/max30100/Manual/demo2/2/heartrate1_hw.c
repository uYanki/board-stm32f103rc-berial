#include "heartrate1_hw.h"

void hr_shut_down(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(MODE_CONFIG, 1, &temp);

    if (enable == true)
        temp |= (1 << 7);
    else
        temp &= ~(1 << 7);

    heartrate1_i2c_hal_write(MODE_CONFIG, 1, &temp);
}

void hr_reset() {
    uint8_t temp;
    heartrate1_i2c_hal_read(MODE_CONFIG, 1, &temp);

    temp |= (1 << 6);
    heartrate1_i2c_hal_write(MODE_CONFIG, 1, &temp);
}

void hr_temp_enable(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(MODE_CONFIG, 1, &temp);

    if (enable == true)
        temp |= (1 << 3);
    else
        temp &= ~(1 << 3);

    heartrate1_i2c_hal_write(MODE_CONFIG, 1, &temp);
}

int8_t hr_set_mode(uint8_t mode) {
    uint8_t temp;

    if (mode > 7)
        return -1;
    heartrate1_i2c_hal_read(MODE_CONFIG, 1, &temp);

    temp |= mode;
    heartrate1_i2c_hal_write(MODE_CONFIG, 1, &temp);
    return 0;
}

void hr_spo2_high_res_enable(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(SPO2_CONFIG, 1, &temp);

    if (enable == true)
        temp |= (1 << 6);

    else
        temp &= ~(1 << 6);
    heartrate1_i2c_hal_write(SPO2_CONFIG, 1, &temp);
}

int8_t hr_spo2_set_sample_rate(uint8_t sample_rate) {
    uint8_t temp;
    if (sample_rate > 7)
        return -1;

    heartrate1_i2c_hal_read(SPO2_CONFIG, 1, &temp);
    temp |= (sample_rate << 2);

    heartrate1_i2c_hal_write(SPO2_CONFIG, 1, &temp);
    return 0;
}

int8_t hr_set_led_pulse_width(uint8_t _value) {
    uint8_t temp;
    if (_value > 3) return -1;
    heartrate1_i2c_hal_read(SPO2_CONFIG, 1, &temp);

    temp |= _value;
    heartrate1_i2c_hal_write(SPO2_CONFIG, 1, &temp);
    return 0;
}

int8_t hr_set_red_current_level(uint8_t level) {
    uint8_t temp;
    if (level > 15) return -1;
    heartrate1_i2c_hal_read(LED_CONFIG, 1, &temp);

    temp |= (level << 4);
    heartrate1_i2c_hal_write(LED_CONFIG, 1, &temp);
    return 0;
}

int8_t hr_set_ir_current_level(uint8_t level) {
    uint8_t temp;
    if (level > 15) return -1;
    heartrate1_i2c_hal_read(LED_CONFIG, 1, &temp);

    temp |= level;
    heartrate1_i2c_hal_write(LED_CONFIG, 1, &temp);
    return 0;
}

// INTERRUPTS

void hr_fifo_almost_full_int_enable(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(INT_ENABLE, 1, &temp);
    if (enable == true)
        temp |= (1 << 7);
    else
        temp &= ~(1 << 7);

    heartrate1_i2c_hal_write(INT_ENABLE, 1, &temp);
}

void hr_temp_ready_int_enable(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(INT_ENABLE, 1, &temp);
    if (enable == true)
        temp |= (1 << 6);
    else
        temp &= ~(1 << 6);

    heartrate1_i2c_hal_write(INT_ENABLE, 1, &temp);
}

void hr_heartrate_data_ready_int_enable(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(INT_ENABLE, 1, &temp);
    if (enable == true)
        temp |= (1 << 5);
    else
        temp &= ~(1 << 5);

    heartrate1_i2c_hal_write(INT_ENABLE, 1, &temp);
}

void hr_heartrate_spo2_data_ready_int_enable(bool enable) {
    uint8_t temp;
    heartrate1_i2c_hal_read(INT_ENABLE, 1, &temp);
    if (enable == true)
        temp |= (1 << 4);
    else
        temp &= ~(1 << 4);

    heartrate1_i2c_hal_write(INT_ENABLE, 1, &temp);
}

uint8_t hr_read_diodes(uint16_t* ir_buff, uint16_t* red_buff) {
    uint8_t i, sampleNum = 0;
    uint8_t wrPtr = 0, rdPtr = 0, ovPtr, _ptr, samples[4];

    heartrate1_i2c_hal_read(FIFO_WRITE_PTR, 1, &wrPtr);
    heartrate1_i2c_hal_read(FIFO_READ_PTR, 1, &rdPtr);

    sampleNum = abs(16 + wrPtr - rdPtr) % 16;

    if (sampleNum >= 1) {
        for (i = 0; i < sampleNum; ++i) {
            // read data
            heartrate1_i2c_hal_read(FIFO_DATA_REG, 4, &samples);

            *(ir_buff) = (uint16_t)samples[1];
            *(ir_buff++) |= (uint16_t)samples[0] << 8;
            *(red_buff) = (uint16_t)samples[3];
            *(red_buff++) |= (uint16_t)samples[2] << 8;
        }
    }
    return sampleNum;
}

uint8_t hr_get_status() {
    uint8_t read_data;
    heartrate1_i2c_hal_read(INT_STATUS, 1, &read_data);
    return read_data;
}

void hr_init(uint8_t slave_address) {
    uint8_t temp;
    uint8_t test;
    heartrate1_i2c_init(slave_address);
    hr_reset();
    do {
        heartrate1_i2c_hal_read(MODE_CONFIG, 1, &temp);
        temp &= (1 << 6);
    } while (temp != 0);

    hr_set_mode(3);
    hr_temp_enable(true);
    hr_spo2_high_res_enable(true);
    hr_spo2_set_sample_rate(0);
    hr_set_led_pulse_width(3);
    hr_set_red_current_level(0b00001111);
    hr_set_ir_current_level(0b00001110);
    hr_heartrate_data_ready_int_enable(true);
    hr_heartrate_spo2_data_ready_int_enable(true);
    hr_temp_ready_int_enable(true);
}
