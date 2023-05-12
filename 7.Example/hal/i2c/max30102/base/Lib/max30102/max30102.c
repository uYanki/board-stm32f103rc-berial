#include "MAX30102.h"

bool max30102_is_ppg_data_ready(uint32_t timeout)
{
#if MAX30102_CHECK_DATA_READY
    // 读取模块中断引脚电平(需连接模块的INT引脚到IO口)
    while (timeout) {
        if (HAL_GPIO_ReadPin(MAX30102_INT_GPIO_Port, MAX30102_INT_Pin) == GPIO_PIN_SET) {
            // read and clear status register
            max30102_read_byte(MAX30102_REG_INTR_STATUS_1);
            return true;
        }
        --timeout;
        HAL_Delay(1);
    }
    return false;
#else
    // 读取中断状态寄存器(需调用 max30102_set_interrupt_enable(MAX30102_INTERRUPT_PPG_RDY, true) 使能该中断)
    return max30102_wait_interrupt_status(MAX30102_INTERRUPT_STATUS_PPG_RDY, true, timeout);
#endif
}

void max30102_test_ppg_mini(void)
{
    // check device
    if (max30102_check_device()) {
        // init device
        uint8_t cmds_init[] = {
            MAX30102_REG_MODE_CONFIG, 0x40,    // Reset
            MAX30102_REG_INTR_ENABLE_1, 0xc0,  // INTR1 setting
            MAX30102_REG_INTR_ENABLE_2, 0x00,  // INTR2 setting
            MAX30102_REG_FIFO_WR_PTR, 0x00,    // FIFO_WR_PTR[4:0]
            MAX30102_REG_OVF_COUNTER, 0x00,    // OVF_COUNTER[4:0]
            MAX30102_REG_FIFO_RD_PTR, 0x00,    // FIFO_RD_PTR[4:0]
            MAX30102_REG_FIFO_CONFIG, 0x0f,    // sample avg = 1, fifo rollover=false, fifo almost full = 17
            MAX30102_REG_MODE_CONFIG, 0x03,    // 0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
            MAX30102_REG_SPO2_CONFIG, 0x27,    // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
            MAX30102_REG_LED1_PA, 0x24,        // Choose value for ~ 7mA for LED1
            MAX30102_REG_LED2_PA, 0x24,        // Choose value for ~ 7mA for LED2
            MAX30102_REG_PILOT_PA, 0x7f,       // Choose value for ~ 25mA for Pilot LED
        };

        i2cdev_mapset(&max30102, cmds_init, ARRAY_SIZE(cmds_init));
        // max30102_set_interrupt_enable(MAX30102_INTERRUPT_PPG_RDY_EN, true);

        // read data
        uint8_t  data_fifo[6];
        uint32_t red, ir;
        while (1) {
            // wait interrupt
            if (max30102_wait_interrupt_status(MAX30102_INTERRUPT_STATUS_A_FULL, true, 50)) {
                // read fifo
                if (max30102_read_bytes(MAX30102_REG_FIFO_DATA, data_fifo, 6)) {
                    red = ((data_fifo[0] & 0x03) << 16) | (data_fifo[1] << 8) | (data_fifo[2]);  // Mask MSB [23:18], 0x03FFFF
                    ir  = ((data_fifo[3] & 0x03) << 16) | (data_fifo[4] << 8) | (data_fifo[5]);
                    println("%i,%i", red, ir);
                    // println("red = %i, ir = %i", red, ir);
                }
            }
        }
    }
}

void max30102_test_ppg_full(void)
{
    if (!max30102_check_device()) return;
    // Reset all configuration, threshold, and data registers to POR values
    max30102_reset();
    // The chip will average multiple samples of same type together if you wish
    max30102_set_fifo_sample_average(MAX30102_SAMPLE_AVERAGE_4);
    // Set to 17 samples to trigger an 'Almost Full' interrupt
    max30102_set_fifo_almost_full(15);
    // Allow FIFO to wrap/roll over
    max30102_set_fifo_roll_over(true);
    // Enable data ready intr
    max30102_set_interrupt_enable(MAX30102_INTERRUPT_PPG_RDY_EN, true);
    // Enable proximity intr
    max30102_set_interrupt_enable(MAX30102_INTERRUPT_PROX_INT_EN, false);
    // led mode
    max30102_set_mode(MAX30102_MODE_MULTI_LED);
    // spo2 config
    max30102_set_spo2_adc_range(MAX30102_ADC_RANGE_2048);
    max30102_set_spo2_sample_rate(MAX30102_SAMPLE_RATE_800_HZ);
    max30102_set_spo2_adc_resolution(MAX30102_ADC_RESOLUTION_17_BIT);
    // led current
    max30102_set_led_current(MAX30102_LED_RED, MAX30102_LED_CURRENT_6_4MA);
    max30102_set_led_current(MAX30102_LED_IR, MAX30102_LED_CURRENT_6_4MA);
    max30102_set_led_current(MAX30102_LED_PILOT, MAX30102_LED_CURRENT_25_4MA);
    // samples order (vaild when mode = MAX30102_MODE_MULTI_LED)
    max30102_set_slot(MAX30102_SLOT_NUM_1, MAX30102_SLOT_LED_RED);
    max30102_set_slot(MAX30102_SLOT_NUM_2, MAX30102_SLOT_LED_IR);
    max30102_set_slot(MAX30102_SLOT_NUM_3, MAX30102_SLOT_LED_RED_PILOT);
    max30102_set_slot(MAX30102_SLOT_NUM_4, MAX30102_SLOT_LED_IR_PILOT);

    // Reset the FIFO before we begin checking the sensor
    max30102_clear_fifo();

    uint32_t               buff[64];
    max30102_fifo_params_t p = max30102_generate_fifo_params(sizeof(buff) / sizeof(uint32_t));
    while (1) {
        if (max30102_is_ppg_data_ready(50)) {
            max30102_read_fifo(buff, &p);
            for (uint8_t i = 0, j = 0; i < p.length; ++i, j += p.unitsize) {
                switch (p.unitsize) {
                    case 1: println("%i", buff[j + 0]); break;
                    case 2: println("%i,%i", buff[j + 0], buff[j + 1]); break;
                    case 3: println("%i,%i,%i", buff[j + 0], buff[j + 1], buff[j + 2]); break;
                    case 4: println("%i,%i,%i,%i", buff[j + 0], buff[j + 1], buff[j + 2], buff[j + 3]); break;
                }
            }
        }
    }
}

void max30102_test_die_temperature_c()
{
    // check device
    if (max30102_check_device()) {
        max30102_reset();
        max30102_set_interrupt_enable(MAX30102_INTERRUPT_DIE_TEMP_RDY_EN, true);
        while (1) {
            println("%.2f", max30102_read_die_temperature_c());
            // println("temp = %.2f", max30102_read_die_temperature_c());
            HAL_Delay(50);
        }
    }
}

void max30102_test_calc_sample_frequency(void)
{
    // 实际采样频率 ( freq = SAMPLE_RATE / SAMPLE_AVG )

    // Setup to sense up to 18 inches, max LED brightness

    if (max30102_check_device()) {
        max30102_set_interrupt_enable(MAX30102_INTERRUPT_PPG_RDY_EN, true);
        max30102_set_mode(MAX30102_MODE_SPO2);
        max30102_set_spo2_adc_range(MAX30102_ADC_RANGE_2048);
        // max30102_set_fifo_sample_average(MAX30102_SAMPLE_AVERAGE_2);
        max30102_set_fifo_sample_average(MAX30102_SAMPLE_AVERAGE_4);
        max30102_set_spo2_sample_rate(MAX30102_SAMPLE_RATE_400_HZ);
        // max30102_set_spo2_sample_rate(MAX30102_SAMPLE_RATE_800_HZ);
        max30102_set_spo2_adc_resolution(MAX30102_ADC_RESOLUTION_16_BIT);
        max30102_set_led_current(MAX30102_LED_RED, MAX30102_LED_CURRENT_0MA);
        max30102_set_led_current(MAX30102_LED_IR, MAX30102_LED_CURRENT_50MA);
        max30102_set_led_current(MAX30102_LED_PILOT, MAX30102_LED_CURRENT_0MA);

        uint32_t               buff[2];  // red + ir
        max30102_fifo_params_t p  = max30102_generate_fifo_params(sizeof(buff) / sizeof(uint32_t));

        uint32_t average_value_ir = 0;  // Average IR at power up

        uint8_t x                 = 32;
        while (x) {
            if (max30102_is_ppg_data_ready(50)) {
                max30102_read_fifo(buff, &p);
                average_value_ir += p.buffaddr[1];
                --x;
            }
        }
        average_value_ir /= 32;

        uint32_t samplesTaken = 0;              // Counter for calculating the Hz or read rate
        uint32_t startTime    = HAL_GetTick();  // Used to calculate measurement rate

        while (1) {
            if (max30102_is_ppg_data_ready(50)) {
                max30102_read_fifo(buff, &p);
                println("IR[%i] delta[%i] Hz[%.2f]",
                        p.buffaddr[1], p.buffaddr[1] - average_value_ir,
                        (float)++samplesTaken / ((HAL_GetTick() - startTime) / 1000.0));
                // Hz ≈ SAMPLERATE_400 / SAMPLEAVG_4 = 100
            }
        }
    }
}

void max30102_wakeup(void) { max30102_write_byte(MAX30102_REG_MODE_CONFIG, 0x00); }

void max30102_shutdown(void) { max30102_write_byte(MAX30102_REG_MODE_CONFIG, 0x80); }  // bit[7]

void max30102_reset(void) { max30102_write_byte(MAX30102_REG_MODE_CONFIG, 0x40); }  // bit[6]

max30102_mode_t max30102_get_mode(void) { return max30102_read_bits(MAX30102_REG_MODE_CONFIG, 0, 3); }  // bit[2:0]

void max30102_set_mode(max30102_mode_t mode) { max30102_write_bits(MAX30102_REG_MODE_CONFIG, 0, 3, mode); }

bool max30102_check_device(void)
{
    if (i2cdev_check(&max30102) && max30102_get_part_id() == 0x15) {
        println("MAX3010x Revision ID: 0x%02x", max30102_get_revision_id());
        return true;
    } else {
        println("fail to check max30102");
        i2cdrv_detector(max30102.bus, max30102.drv);
        return false;
    }
}

bool max30102_get_interrupt_enable(max30102_interrupt_enable_t type) { return max30102_read_bit(type == MAX30102_INTERRUPT_DIE_TEMP_RDY_EN ? MAX30102_REG_INTR_ENABLE_2 : MAX30102_REG_INTR_ENABLE_1, type); }

void max30102_set_interrupt_enable(max30102_interrupt_enable_t type, bool enable) { max30102_write_bit(type == MAX30102_INTERRUPT_DIE_TEMP_RDY_EN ? MAX30102_REG_INTR_ENABLE_2 : MAX30102_REG_INTR_ENABLE_1, type, enable); }

bool max30102_get_interrupt_status(max30102_interrupt_status_t type) { return max30102_read_bit((type == MAX30102_INTERRUPT_STATUS_DIE_TEMP_RDY) ? MAX30102_REG_INTR_STATUS_2 : MAX30102_REG_INTR_STATUS_1, type); }

bool max30102_wait_interrupt_status(max30102_interrupt_status_t type, bool status, uint32_t timeout /* ms */)
{
    while (timeout) {
        if (max30102_get_interrupt_status(type) == status)
            return true;
        --timeout;
        HAL_Delay(1);
    }
    return false;
}

uint8_t max30102_get_revision_id(void) { return max30102_read_byte(MAX30102_REG_REV_ID); }

uint8_t max30102_get_part_id(void) { return max30102_read_byte(MAX30102_REG_PART_ID); }  // return 0x15

max30102_adc_range_t max30102_get_spo2_adc_range(void) { return max30102_read_bits(MAX30102_REG_SPO2_CONFIG, 5, 2); }  // 2bit[6:5]

void max30102_set_spo2_adc_range(max30102_adc_range_t range) { max30102_write_bits(MAX30102_REG_SPO2_CONFIG, 5, 2, range); }

max30102_sample_rate_t max30102_get_spo2_sample_rate() { return max30102_read_bits(MAX30102_REG_SPO2_CONFIG, 2, 3); }  // 3bit[4:2]

void max30102_set_spo2_sample_rate(max30102_sample_rate_t rate) { max30102_write_bits(MAX30102_REG_SPO2_CONFIG, 2, 3, rate); }

max30102_adc_resolution_t max30102_get_spo2_adc_resolution() { return max30102_read_bits(MAX30102_REG_SPO2_CONFIG, 0, 2); }  // 2bit[1:0]

void max30102_set_spo2_adc_resolution(max30102_adc_resolution_t resolution) { max30102_write_bits(MAX30102_REG_SPO2_CONFIG, 0, 2, resolution); }

uint8_t max30102_get_led_current(max30102_led_t led)
{
    switch (led) {
        case MAX30102_LED_RED: return max30102_read_byte(MAX30102_REG_LED1_PA);
        case MAX30102_LED_IR: return max30102_read_byte(MAX30102_REG_LED2_PA);
        case MAX30102_LED_PILOT: return max30102_read_byte(MAX30102_REG_PILOT_PA);
        default: return 0;
    }
}

void max30102_set_led_current(max30102_led_t led, uint8_t current)
{
    switch (led) {
        case MAX30102_LED_RED: max30102_write_byte(MAX30102_REG_LED1_PA, current); break;
        case MAX30102_LED_IR: max30102_write_byte(MAX30102_REG_LED2_PA, current); break;
        case MAX30102_LED_PILOT: max30102_write_byte(MAX30102_REG_PILOT_PA, current); break;
        default: break;
    }
}

void max30102_set_slot(max30102_slot_num_t slot, max30102_slot_led_t led /**/)
{
    switch (slot) {
        case MAX30102_SLOT_NUM_1: max30102_write_bits(MAX30102_REG_MULTI_LED_CTRL1, 0, 3, led); break;
        case MAX30102_SLOT_NUM_2: max30102_write_bits(MAX30102_REG_MULTI_LED_CTRL1, 4, 3, led); break;
        case MAX30102_SLOT_NUM_3: max30102_write_bits(MAX30102_REG_MULTI_LED_CTRL2, 0, 3, led); break;
        case MAX30102_SLOT_NUM_4: max30102_write_bits(MAX30102_REG_MULTI_LED_CTRL2, 4, 3, led); break;
    }
}

max30102_led_t max30102_get_slot(max30102_slot_num_t slot)
{
    switch (slot) {
        case MAX30102_SLOT_NUM_1: return max30102_read_bits(MAX30102_REG_MULTI_LED_CTRL1, 0, 3);
        case MAX30102_SLOT_NUM_2: return max30102_read_bits(MAX30102_REG_MULTI_LED_CTRL1, 4, 3);
        case MAX30102_SLOT_NUM_3: return max30102_read_bits(MAX30102_REG_MULTI_LED_CTRL2, 0, 3);
        case MAX30102_SLOT_NUM_4: return max30102_read_bits(MAX30102_REG_MULTI_LED_CTRL2, 4, 3);
    }
    return MAX30102_LED_NONE;
}

void max30102_clear_slot(void)
{
    max30102_write_byte(MAX30102_REG_MULTI_LED_CTRL1, 0x00);
    max30102_write_byte(MAX30102_REG_MULTI_LED_CTRL2, 0x00);
}

uint8_t max30102_get_proximity_threshold(void) { return max30102_read_byte(MAX30102_REG_PROX_INT_THRESH); }

void max30102_set_proximity_threshold(uint8_t threshold) { max30102_write_byte(MAX30102_REG_PROX_INT_THRESH, threshold); }

max30102_sample_average_t max30102_get_fifo_sample_average(void) { return max30102_read_bits(MAX30102_REG_FIFO_CONFIG, 5, 3); }  // 3bit[7:5]

void max30102_set_fifo_sample_average(max30102_sample_average_t sample) { max30102_write_bits(MAX30102_REG_FIFO_CONFIG, 5, 3, sample); }

bool max30102_get_fifo_roll_over(void) { return max30102_read_bit(MAX30102_REG_FIFO_CONFIG, 4); }  // 1bit[4]

void max30102_set_fifo_roll_over(bool enable) { max30102_write_bit(MAX30102_REG_FIFO_CONFIG, 5, enable); }

uint8_t max30102_get_fifo_almost_full(void) { return max30102_read_bits(MAX30102_REG_FIFO_CONFIG, 0, 4); }  // 4bit[3:0]

void max30102_set_fifo_almost_full(uint8_t value) { max30102_write_bits(MAX30102_REG_FIFO_CONFIG, 0, 4, value); }

void max30102_clear_fifo(void)
{
    max30102_write_byte(MAX30102_REG_FIFO_WR_PTR, 0);
    max30102_write_byte(MAX30102_REG_OVF_COUNTER, 0);
    max30102_write_byte(MAX30102_REG_FIFO_RD_PTR, 0);
}

uint8_t max30102_get_fifo_overflow_counter(void) { return max30102_read_bits(MAX30102_REG_OVF_COUNTER, 0, 5); }

void max30102_set_fifo_overflow_counter(uint8_t counter) { max30102_write_bits(MAX30102_REG_OVF_COUNTER, 0, 5, counter); }

uint8_t max30102_get_fifo_read_pointer(void) { return max30102_read_bits(MAX30102_REG_FIFO_RD_PTR, 0, 5); }

void max30102_set_fifo_read_pointer(uint8_t pointer) { max30102_write_bits(MAX30102_REG_FIFO_RD_PTR, 0, 5, pointer); }

uint8_t max30102_get_fifo_write_pointer(void) { return max30102_read_bits(MAX30102_REG_FIFO_WR_PTR, 0, 5); }

void max30102_set_fifo_write_pointer(uint8_t pointer) { max30102_write_bits(MAX30102_REG_FIFO_WR_PTR, 0, 5, pointer); }

max30102_fifo_params_t max30102_generate_fifo_params(uint8_t buffsize)
{
    max30102_fifo_params_t p;

    p.buffaddr = (void*)0;

    // number of active slot

    p.unitsize = 0;

    if (p.unitsize == 0) {
        switch (max30102_get_mode()) {
            case MAX30102_MODE_HEART_RATE: p.unitsize = 1; break;  // red
            case MAX30102_MODE_SPO2: p.unitsize = 2; break;        // red + ir
            case MAX30102_MODE_MULTI_LED: {                        // custom
                if (max30102_get_slot(MAX30102_SLOT_NUM_1) != MAX30102_SLOT_LED_NONE)
                    if (++p.unitsize, max30102_get_slot(MAX30102_SLOT_NUM_2) != MAX30102_SLOT_LED_NONE)
                        if (++p.unitsize, max30102_get_slot(MAX30102_SLOT_NUM_3) != MAX30102_SLOT_LED_NONE)
                            if (++p.unitsize, max30102_get_slot(MAX30102_SLOT_NUM_4) != MAX30102_SLOT_LED_NONE)
                                ++p.unitsize;
                break;
            }
        }
    }

    // right shift amount

    switch (max30102_get_spo2_adc_resolution()) {
        case MAX30102_ADC_RESOLUTION_18_BIT: p.rshift = 0; break;
        case MAX30102_ADC_RESOLUTION_17_BIT: p.rshift = 1; break;
        case MAX30102_ADC_RESOLUTION_16_BIT: p.rshift = 2; break;
        case MAX30102_ADC_RESOLUTION_15_BIT: p.rshift = 3; break;
    }

    // maximum readable length

    p.capacity = buffsize / p.unitsize;

    return p;
}

// read raw
static uint8_t buff[32 * 12];

uint8_t max30102_read_fifo(uint32_t* data, max30102_fifo_params_t* param)
{
    param->buffaddr = data;

    // size of read

    uint8_t wp      = max30102_get_fifo_write_pointer(),
            rp      = max30102_get_fifo_read_pointer();

    uint8_t len;

    if (wp > rp) {
        len = wp - rp;
    } else {
        // The circular FIFO depth is 32
        len = 32 + wp - rp;
    }

    if (len > param->capacity)
        len = param->capacity;

    param->length = len;

    // read fifo
    max30102_read_bytes(MAX30102_REG_FIFO_DATA, buff, len * param->unitsize * 3);

    uint8_t  i, j;
    uint16_t s = 0, ss = 0;
    for (i = 0; i < len; ++i) {
        for (j = 0; j < param->unitsize; ++j) {
            data[ss] = (buff[s + 0] << 16) |
                       (buff[s + 1] << 8) |
                       (buff[s + 2] << 0);
            data[ss] >>= param->rshift;
            s += 3;
            ++ss;
        }
    }

    return len;
}

float max30102_read_die_temperature_c(void)
{
    // 使用前需调用函数 max30102_set_interrupt_enable(MAX30102_INTERRUPT_STATUS_DIE_TEMP_RDY, true); 使能温度转换完成中断
    // Config die temperature register to take 1 temperature sample
    max30102_write_bit(MAX30102_REG_TEMP_CONFIG, 0, 1);
    // Poll for bit to clear, reading is then complete
    if (!max30102_wait_interrupt_status(MAX30102_INTERRUPT_STATUS_DIE_TEMP_RDY, true, 10)) return -999.0;
    // Read die temperature register (integer)
    int8_t  tempInt  = max30102_read_byte(MAX30102_REG_TEMP_INTR);
    uint8_t tempFrac = max30102_read_bits(MAX30102_REG_TEMP_FRAC, 0, 4);  // Causes the clearing of the DIE_TEMP_RDY interrupt
    // Calculate temperature
    return (float)tempInt + ((float)tempFrac * 0.0625);

    // 华氏度计算如下：
    // float temp = MAX30102_ReadTemperature();
    // if (temp != -999.0) temp = temp * 1.8 + 32.0;
    // return temp;
}
