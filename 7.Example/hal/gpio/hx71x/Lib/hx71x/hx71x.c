
#include "hx71x.h"

static hx71x_mode_t m_mode = HX71X_MODE_CHANNEL_A_GAIN_128;

#define Delay_us(us) HAL_Delay_us(us)

#define DT() HAL_GPIO_ReadPin(HX71X_DT_GPIO_Port, HX71X_DT_Pin)

#define SCK_L() HAL_GPIO_WritePin(HX71X_SCK_GPIO_Port, HX71X_SCK_Pin, GPIO_PIN_RESET)
#define SCK_H() HAL_GPIO_WritePin(HX71X_SCK_GPIO_Port, HX71X_SCK_Pin, GPIO_PIN_SET)

////////////////////////////////////////////

int32_t hx71x_read_raw(void)
{
    uint8_t  i    = 0;
    uint32_t data = 0;

    SCK_L();
    Delay_us(10);

    // Wait for the chip to become ready. (DT=0)
    while (DT()) {}

    // Pulse the clock pin 24 times to read the data. (read 3 bytes, msb first)

    for (i = 0; i < 24; ++i) {
        SCK_H();
        Delay_us(10);
        SCK_L();
        Delay_us(10);
        data <<= 1;
        data |= DT();
    }

    data ^= 0x800000;  // 最高位取反

    // Set the channel and the gain factor for the next reading using the clock pin.

    for (i = 0; i < m_mode; ++i) {
        SCK_H();
        Delay_us(10);
        SCK_L();
        Delay_us(10);
    }

    // printv("%d", data);

    return data;
}

int32_t hx71x_read_raw_average(uint8_t times)
{
    int32_t sum = 0;
    for (uint8_t i = 0; i < times; ++i)
        sum += hx71x_read_raw();
    return sum / times;
}

float hx71x_convert_to_voltage(int32_t raw)
{
    float factor;
    switch (m_mode) {
        case HX71X_MODE_CHANNEL_A_GAIN_128: factor = 20; break;
        case HX71X_MODE_CHANNEL_A_GAIN_64: factor = 40; break;
#if CONFIG_HX711
        case HX71X_MODE_CHANNEL_B_GAIN_32: voltage = 80; break;
#endif
    }
    return raw * (factor / (1 << 24)) / 1000;
}

float hx71x_convert_to_pressure(int32_t raw)
{
    // 海拔越高，气压越低

    float pascal = raw * 2.98023e-7 * 200 + 500;
    float atm    = pascal * 9.86923E-6;
    float mmHg   = pascal * 0.00750062;
    float psi    = pascal * 0.000145038;

    // println("%.4f\r\n", pascal);

    printv("%.4f", pascal);
    printv("%.4f", atm);
    printv("%.4f", mmHg);
    printv("%.4f", psi);
    println(SEPARATOR60);

    return pascal;
}

void hx71x_set_mode(hx71x_mode_t mode)
{
    m_mode = mode;
    hx71x_read_raw();
}

hx71x_mode_t hx71x_get_mode(void)
{
    return m_mode;
}