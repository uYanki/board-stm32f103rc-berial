#include "dht11.h"

GPIO_InitTypeDef GPIO_InitStruct = {
    .Pin   = DHT11_Pin,
    .Pull  = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_LOW,
};

void DQ_OUT(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP,
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

#define DQ_H() HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)
#define DQ_L() HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET)

void DQ_IN(void)
{
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT,
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

#define DQ_GET()  HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)
#define DQ_IS_L() (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET)
#define DQ_IS_H() (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_SET)

void HAL_Delay_us(uint32_t us)
{
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(us - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}

#define delay_ms(ms) HAL_Delay(ms)
#define delay_us(us) HAL_Delay_us(us)

/*********************************************************************************************************/

static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, k;
    uint8_t read = 0;

    for (i = 0; i < 8; i++) {
        read <<= 1;

        /* 等待DQ电平变高 (超时100us) */
        for (k = 0; k < 10; k++) {
            if (DQ_IS_H()) break;
            HAL_Delay_us(10);
        }

        /* 超时无应答，失败 */
        if (k >= 10) return 0;

        /* 等待DQ电平变低，统计DQ高电平时长 (超时100us) */
        for (k = 0; k < 10; k++) {
            if (DQ_IS_L()) break;
            HAL_Delay_us(10);
        }

        /* 高脉冲持续时间大于40us ，认为是1，否则是0 */
        if (k > 3) read++;
    }

    return read;
}

// temp:温度(范围:0~50°)
// humi:湿度(范围:20%~90%)
uint8_t dht11_read(float* temp, float* humi)
{
    /*
        时序:
        1. MCU拉低QD持续时间大于 18ms, 然后释放QD = 1
    */
    uint8_t i;
    uint8_t k;
    uint8_t sum;
    uint8_t buf[5];

    *temp = *humi = 0;

    /* 1. 主机发送起始信号, DQ拉低时间 ＞ 18ms */
    DQ_OUT();

    DQ_L();
    HAL_Delay(20);
    DQ_H();

    /* 等待15us */
    HAL_Delay_us(15);

    DQ_IN();

    /* 2. 等待DQ电平变低 ( 超时100us) */
    for (k = 0; k < 10; k++) {
        if (DQ_IS_L()) break;
        HAL_Delay_us(10);
    }

    /* 超时无应答，失败 */
    if (k >= 10) return 0;

    /* 3.等待DQ电平变高 (超时100us) */
    for (k = 0; k < 10; k++) {
        if (DQ_IS_H()) break;
        HAL_Delay_us(10);
    }

    /* 超时无应答，失败 */
    if (k >= 10) return 0;

    /* 4.等待DQ电平变低 (超时100us) */
    for (k = 0; k < 10; k++) {
        if (DQ_IS_L()) break;
        HAL_Delay_us(10);
    }

    /* 超时无应答，失败 */
    if (k >= 10) return 0;

    /* 读40bit 数据 */
    for (i = 0; i < 5; i++)
        buf[i] = DHT11_ReadByte();
    HAL_Delay_us(100);

    if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
        *humi = buf[0];
        *temp = buf[2];
        // *humi = ((buf[0] << 8) + buf[1]) / 10.0;
        // *temp = ((buf[2] << 8) + buf[3]) / 10.0;
        printf("huml=%d.%d,temp=%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    }

    return 1;
}

/*!
 *  @brief  Converts Celcius to Fahrenheit
 *  @param  c
 *					value in Celcius
 *	@return float value in Fahrenheit
 */
float convert_c_to_f(float c) { return c * 1.8 + 32; }

/*!
 *  @brief  Converts Fahrenheit to Celcius
 *  @param  f
 *					value in Fahrenheit
 *	@return float value in Celcius
 */
float convert_f_to_c(float f) { return (f - 32) * 0.55555; }

/*!
 *  @brief  Compute Heat Index
 *  		Using both Rothfusz and Steadman's equations
 *			(http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml)
 *  @param  temperature
 *          temperature in selected scale
 *  @param  percentHumidity
 *          humidity in percent
 *  @param  isFahrenheit
 * 					true if fahrenheit, false if celcius
 *	@return float heat index
 */
float compute_heat_index(float temperature, float percentHumidity, bool isFahrenheit /* true */)
{
    float hi;

    if (!isFahrenheit)
        temperature = convert_c_to_f(temperature);

    hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) +
                (percentHumidity * 0.094));

    if (hi > 79) {
        hi = -42.379 + 2.04901523 * temperature + 10.14333127 * percentHumidity +
             -0.22475541 * temperature * percentHumidity +
             -0.00683783 * pow(temperature, 2) +
             -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature * pow(percentHumidity, 2) +
             -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

        if ((percentHumidity < 13) && (temperature >= 80.0) &&
            (temperature <= 112.0))
            hi -= ((13.0 - percentHumidity) * 0.25) *
                  sqrt((17.0 - fabs(temperature - 95.0)) * 0.05882);

        else if ((percentHumidity > 85.0) && (temperature >= 80.0) &&
                 (temperature <= 87.0))
            hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }

    return isFahrenheit ? hi : convert_f_to_c(hi);
}
