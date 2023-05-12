#include "MAX30102.h"

uint8_t MAX30102_ReadReg(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&MAX30102_I2C, MAX30102_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, HAL_MAX_DELAY);
    return buff;
}

void MAX30102_WriteReg(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&MAX30102_I2C, MAX30102_DEV, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}

void MAX30102_MaskBits(uint8_t reg, uint8_t mask, uint8_t newval)
{
    // Zero-out the portions of the register we're interested in, and then set new value
    MAX30102_WriteReg(reg, MAX30102_ReadReg(reg) & ~mask | newval);
}

uint8_t MAX30102_WaitBits(uint8_t reg, uint8_t mask, uint32_t timeout)
{
    uint32_t start = HAL_GetTick();
    while (HAL_GetTick() - start < timeout) {
        if ((MAX30102_ReadReg(reg) & mask) == mask)
            return HAL_OK;
        HAL_Delay(1);
    }
    return HAL_TIMEOUT;
}

//////////////////////////////////////////////////////////////////

uint8_t activeLEDs = 0;

uint8_t MAX30102_CheckDevice()
{
    // if (HAL_I2C_IsDeviceReady(&MAX30102_I2C, MAX30102_DEV, 3, HAL_MAX_DELAY) != HAL_OK) return HAL_ERROR;
    if (MAX30102_ReadPartID() != 0x15) return HAL_ERROR;
    printf("MAX3010x Revision ID: 0x%2x", MAX30102_ReadRevisionID());
    return HAL_OK;
}

void MAX30102_Init(
    uint8_t powerLevel,  // 0=Off, 255=50mA, I≈50.0mA/powerLevel
    uint8_t sampleAverage,
    uint8_t ledMode,
    int     sampleRate,
    int     pulseWidth,
    int     adcRange)
{
    // Reset all configuration, threshold, and data registers to POR values
    MAX30102_Reset();

    /******************** FIFO Configuration ********************/

    // The chip will average multiple samples of same type together if you wish
    MAX30102_SetSampleAverage(sampleAverage);
    // Set to 17 samples to trigger an 'Almost Full' interrupt
    MAX30102_SetAlmostFull(15);
    // Allow FIFO to wrap/roll over
    MAX30102_SetRollOverEnable(1);
    // Enable data ready intr
    MAX30102_SetIntEnable1(INT_DATA_RDY, 1);

    /******************** Led Mode Configuration ********************/

    // used to control how many bytes to read from FIFO buffer
    switch (ledMode) {
        case LED_MODE_REDONLY: activeLEDs = 1; break;
        case LED_MODE_REDIRONLY: activeLEDs = 2; break;
        case LED_MODE_MULTILED: activeLEDs = 3; break;
    }

    // Set which LEDs are used for sampling -- Red only, RED+IR only, or custom.
    MAX30102_SetLedMode(ledMode);

    /******************** Particle Sensing Configuration ********************/

    // - The longer the pulse width the longer range of detection you'll have, 脉冲宽度越长，检测范围越长
    // - At 69us and 0.4mA it's about 2 inches
    // - At 411us and 0.4mA it's about 6 inches

    MAX30102_WriteReg(REG_SPO2_CONFIG, adcRange | sampleRate | pulseWidth);

    /**************** LED Pulse Amplitude Configuration ****************/

    // - 0x02, 0.4mA - Presence detection of ~4 inch
    // - 0x1F, 6.4mA - Presence detection of ~8 inch (default)
    // - 0x7F, 25.4mA - Presence detection of ~8 inch
    // - 0xFF, 50.0mA - Presence detection of ~12 inch (typical)

    MAX30102_WriteReg(REG_LED1_PA, powerLevel);                      // Red
    if (activeLEDs > 1) MAX30102_WriteReg(REG_LED2_PA, powerLevel);  // IR
    if (activeLEDs > 2) MAX30102_WriteReg(REG_LED3_PA, powerLevel);  // Green
    MAX30102_WriteReg(REG_PILOT_PA, powerLevel);                     // Proximity

    // 设置采样值保存的位置

    MAX30102_SetSlot(1, SLOT_RED_LED);                        // RED_LED 放在前3个字节
    if (activeLEDs > 1) MAX30102_SetSlot(2, SLOT_IR_LED);     // IR_LED 放在中间3个字节
    if (activeLEDs > 2) MAX30102_SetSlot(3, SLOT_GREEN_LED);  // GREEN_LED 放在后3个字节

    // MAX30102_SetDevSlot(4, SLOT_NONE);

    /**************** Clear FIFO ****************/

    // Reset the FIFO before we begin checking the sensor
    MAX30102_ClearFIFO();
}

// Resets all points to start in a known state
// recommends clearing FIFO before beginning a read
void MAX30102_ClearFIFO(void)
{
    MAX30102_WriteReg(REG_FIFO_WR_PTR, 0);
    MAX30102_WriteReg(REG_OVF_COUNTER, 0);
    MAX30102_WriteReg(REG_FIFO_RD_PTR, 0);
}

void MAX30102_SetSlot(uint8_t index /*1~4*/, uint8_t device)
{
    switch (index) {
        case 1: MAX30102_MaskBits(REG_MULTI_LED_CTRL1, SLOT1_MASK, device); break;
        case 2: MAX30102_MaskBits(REG_MULTI_LED_CTRL1, SLOT2_MASK, device << 4); break;
        case 3: MAX30102_MaskBits(REG_MULTI_LED_CTRL2, SLOT1_MASK, device); break;
        case 4: MAX30102_MaskBits(REG_MULTI_LED_CTRL2, SLOT2_MASK, device << 4); break;
        default: break;
    }
}

// Clears all slot assignments
void MAX30102_ClearSlot(void)
{
    MAX30102_WriteReg(REG_MULTI_LED_CTRL1, 0);
    MAX30102_WriteReg(REG_MULTI_LED_CTRL2, 0);
}

// Die Temperature
// Returns temp in C 返回摄氏度
float MAX30102_ReadTemperature()
{
    // 使用前需调用函数 MAX30102_SetIntEnable2(INT_DIE_TEMP_RDY, 1) 使能中断
    // Config die temperature register to take 1 temperature sample
    MAX30102_WriteReg(REG_TEMP_CONFIG, 0x01);
    // Poll for bit to clear, reading is then complete
    if (MAX30102_WaitIntState2(INT_DIE_TEMP_RDY)) return -999.0;
    //  Read die temperature register (integer)
    int8_t  tempInt  = MAX30102_ReadReg(REG_TEMP_INTR);
    uint8_t tempFrac = MAX30102_ReadReg(REG_TEMP_FRAC);  // Causes the clearing of the DIE_TEMP_RDY interrupt
    // Calculate temperature
    return (float)tempInt + ((float)tempFrac * 0.0625);

    // 华氏度计算如下：
    // float temp = MAX30102_ReadTemperature();
    // if (temp != -999.0) temp = temp * 1.8 + 32.0;
    // return temp;
}

uint8_t MAX30102_CheckFIFO()
{
    // 等待采样结束
#if HowToCheckFIFO == 1
    // 读取已采样的数据数
    int8_t count;
    do {
        // Calculate the number of readings we need to get from sensor
        count = MAX30102_GetWritePointer() - MAX30102_GetReadPointer();
        if (count < 0) count += 32;  // Wrap condition
    } while (count >= activeLEDs * 3);
#elif HowToCheckFIFO == 2
    // 读取模块中断引脚电平(需连接模块的INT引脚到IO口)
    while (HAL_GPIO_ReadPin(MAX30102_INT_GPIO_Port, MAX30102_INT_Pin) == GPIO_PIN_SET) {}
    // read and clear status register
    MAX30102_ReadReg(REG_INTR_STATUS_1);
#else
    // 读取中断状态寄存器(需调用MAX30102_SetIntEnable1(INT_DATA_RDY, 1)使能该中断)
    if (MAX30102_WaitIntState1(INT_DATA_RDY) != HAL_OK) return HAL_TIMEOUT;
#endif
    return HAL_OK;
}

uint8_t MAX30102_ReadFIFO(uint32_t* red /*out*/, uint32_t* ir /*out*/, uint32_t* green)
{
    uint8_t data[9];

    // 读出的顺序与 REG_MULTI_LED_CTRL1, REG_MULTI_LED_CTRL2 有关

    // 需调用以下函数以确保数据读出的顺序为:Red,IR,Green
    // MAX30102_SetSlot(1, SLOT_RED_LED);
    // MAX30102_SetSlot(2, SLOT_IR_LED);
    // MAX30102_SetSlot(3, SLOT_GREEN_LED);

    if (HAL_I2C_Mem_Read(&MAX30102_I2C, MAX30102_DEV, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT, data, activeLEDs * 3, HAL_MAX_DELAY) != HAL_OK) return HAL_ERROR;

    // 因为采样宽度最大位 18bit，所以部分比特位是无效的 (注：采样宽度为16bit时也是以下写法)

    *red   = ((data[0] & 0x03) << 16) | (data[1] << 8) | (data[2]);  // Zero MSB [31:18], 0x0003FFFF
    *ir    = (activeLEDs > 1) ? (((data[3] & 0x03) << 16) | (data[4] << 8) | (data[5])) : 0;
    *green = (activeLEDs > 2) ? (((data[6] & 0x03) << 16) | (data[7] << 8) | (data[8])) : 0;

    return HAL_OK;
}

uint8_t MAX30102_ReadFIFO2(max30102_fifo_t* fifo)
{
    return MAX30102_ReadFIFO(&(fifo->Red), &(fifo->IR), &(fifo->Green));
}

//////////////////////////////////////////////////////////////////

void MAX30102Test_PPG()
{  // mini test
    // check device
    if (MAX30102_CheckDevice() != HAL_OK) return;
    // init device
    uint8_t cmds_init[] = {
        REG_MODE_CONFIG, 0x40,    // Reset
        REG_INTR_ENABLE_1, 0xc0,  // INTR1 setting
        REG_INTR_ENABLE_2, 0x00,  // INTR2 setting
        REG_FIFO_WR_PTR, 0x00,    // FIFO_WR_PTR[4:0]
        REG_OVF_COUNTER, 0x00,    // OVF_COUNTER[4:0]
        REG_FIFO_RD_PTR, 0x00,    // FIFO_RD_PTR[4:0]
        REG_FIFO_CONFIG, 0x0f,    // sample avg = 1, fifo rollover=false, fifo almost full = 17
        REG_MODE_CONFIG, 0x03,    // 0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
        REG_SPO2_CONFIG, 0x27,    // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
        REG_LED1_PA, 0x24,        // Choose value for ~ 7mA for LED1
        REG_LED2_PA, 0x24,        // Choose value for ~ 7mA for LED2
        REG_PILOT_PA, 0x7f,       // Choose value for ~ 25mA for Pilot LED
    };
    for (uint8_t i = 0; i < sizeof(cmds_init); i += 2)
        MAX30102_WriteReg(cmds_init[i], cmds_init[i + 1]);
    // read data
    uint8_t  data_fifo[6];
    uint32_t red, ir;
    while (1) {
        // wait interrupt
        if (MAX30102_WaitIntState1(INT_DATA_RDY) != HAL_OK) continue;
        // read fifo
        if (HAL_I2C_Mem_Read(&MAX30102_I2C, MAX30102_DEV, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT, data_fifo, 6, HAL_MAX_DELAY) != HAL_OK) return;
        red = ((data_fifo[0] & 0x03) << 16) | (data_fifo[1] << 8) | (data_fifo[2]);  // Mask MSB [23:18], 0x03FFFF
        ir  = ((data_fifo[3] & 0x03) << 16) | (data_fifo[4] << 8) | (data_fifo[5]);
        printf("%i,%i\r\n", red, ir);  // printf("red = %i, ir = %i\r\n", red, ir);
    }
}

void MAX30102Test_PPG2()
{
    // check device
    if (MAX30102_CheckDevice() != HAL_OK) return;
    // init device
    MAX30102_Init(0x1F, SAMPLE_AVERAGE_4, LED_MODE_MULTILED, SAMPLE_RATE_100, PULSE_WIDTH_118, ADC_RANGE_4096);
    // read data
    max30102_fifo_t fifo;
    while (1) {
        if (MAX30102_CheckFIFO() == HAL_OK)
            if (MAX30102_ReadFIFO2(&fifo) == HAL_OK)
                printf("%i,%i,%i\r\n", fifo.Red, fifo.IR, fifo.Green);
    }
}

void MAX30102Test_TEMP()
{
    // check device
    if (MAX30102_CheckDevice() != HAL_OK) return;
    MAX30102_Reset();
    MAX30102_SetIntEnable2(INT_DIE_TEMP_RDY, 1);
    while (1) {
        printf("%.2f\r\n", MAX30102_ReadTemperature());
        // printf("temp = %.2f\r\n", MAX30102_ReadTemperature());
        HAL_Delay(50);
    }
}

void MAX30102Test_CalcSampleFreq(void)
{  // 实际采样频率 ( freq = SAMPLE_RATE / SAMPLE_AVG )

    // Setup to sense up to 18 inches, max LED brightness

    MAX30102_Init(0xFF, SAMPLE_AVERAGE_4, LED_MODE_REDIRONLY, SAMPLE_RATE_400, PULSE_WIDTH_411, ADC_RANGE_2048);
    MAX30102_SetPulseAmplitude(1, 0);  // Turn off Red LED
    MAX30102_SetPulseAmplitude(3, 0);  // Turn off Green LED

    // Take an average of IR readings at power up
    uint32_t unblockedValue = 0;  // Average IR at power up

    uint32_t red, ir, green;
    for (uint8_t x = 0; x < 32; x++)
        if (MAX30102_CheckFIFO() == HAL_OK)
            if (MAX30102_ReadFIFO(&red, &ir, &green) == HAL_OK)
                unblockedValue += ir;
    unblockedValue /= 32;

    uint32_t samplesTaken = 0;              // Counter for calculating the Hz or read rate
    uint32_t startTime    = HAL_GetTick();  // Used to calculate measurement rate

    while (1) {
        if (MAX30102_CheckFIFO() == HAL_OK) {
            if (MAX30102_ReadFIFO(&red, &ir, &green) == HAL_OK) {
                ++samplesTaken;
                printf("IR[%i] Hz[%.2f] delta[%i]\r\n", ir, (float)samplesTaken / ((HAL_GetTick() - startTime) / 1000.0), ir - unblockedValue);
                // Hz ≈ SAMPLERATE_400 / SAMPLEAVG_4 = 100
            }
        }
    }
}
