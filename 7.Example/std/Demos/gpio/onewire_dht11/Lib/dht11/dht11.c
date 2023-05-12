

#include "dht11.h"

static uint8_t dht11_read_byte(void);
static void    dht11_set_output_mode(void);
static void    dht11_set_input_mode(void);

void dht11_init(void)
{
    DHT11_GPIO_CLK_EN();
    dht11_set_output_mode();
    DHT11_DATA_1();
}

static void dht11_set_output_mode(void)
{
    // dht11 不能像 DS18B20 一样,使用开漏模式，开始几次能正常读, 后面就读取失败了
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

static void dht11_set_input_mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

static uint8_t dht11_read_byte(void)
{
    uint8_t i, temp = 0;

    for (i = 0; i < 8; i++) {
        // 每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束
        while (Bit_RESET == DHT11_DATA_N()) {}

        /**
         * DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
         * 通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时
         */
        DHT11_DELAY_US(40);  // 延时x us 这个延时需要大于数据0持续的时间即可

        // 数据 1
        if (Bit_SET == DHT11_DATA_N()) {
            // 等待数据1的高电平结束
            while (Bit_SET == DHT11_DATA_N()) {}
            temp |= (uint8_t)(0x01 << (7 - i));  // MSB
        }
        // 数据 0
        else {
            temp &= (uint8_t) ~(0x01 << (7 - i));  // MSB
        }
    }

    return temp;
}

bool dht11_read_temp_and_humidity(dht11_data_t* data)
{
    dht11_set_output_mode();

    // 主机拉低 18ms
    DHT11_DATA_0();
    DHT11_DELAY_MS(18);

    // 总线拉高 30us
    DHT11_DATA_1();
    DHT11_DELAY_US(30);

    // 判断从机是否有低电平响应信号

    dht11_set_input_mode();

    if (DHT11_DATA_N() == Bit_RESET) {
        // 轮询直到从机发出 的80us 低电平 响应信号结束
        while (DHT11_DATA_N() == Bit_RESET) {}

        // 轮询直到从机发出的 80us 高电平 标置信号结束
        while (DHT11_DATA_N() == Bit_SET) {}

        /* 数据接收
         * 一次完整的数据传输为40bit，高位先出
         * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和
         */
        data->humi_int  = dht11_read_byte();
        data->humi_deci = dht11_read_byte();
        data->temp_int  = dht11_read_byte();
        data->temp_deci = dht11_read_byte();
        data->check_sum = dht11_read_byte();

        // 主机拉高
        DHT11_DATA_1();

        // 数据校验
        return data->check_sum == (data->humi_int + data->humi_deci + data->temp_int + data->temp_deci);
    }

    return false;
}
