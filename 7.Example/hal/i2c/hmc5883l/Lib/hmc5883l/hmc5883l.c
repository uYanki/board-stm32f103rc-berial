#include "hmc5883l.h"

uint8_t HMC5883Lmode;

/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */
void HMC5883L_Initialize()
{
    // write CONFIG_A register

    uint8_t tmp = (HMC5883L_AVERAGING_8 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1)) | (HMC5883L_RATE_15 << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1)) | (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1));
    HMC5883L_WriteReg(HMC5883L_RA_CONFIG_A, tmp);

    // write CONFIG_B register
    HMC5883L_SetGain(HMC5883L_GAIN_1090);

    // write MODE register
    HMC5883L_SetMode(HMC5883L_MODE_SINGLE);
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
uint8_t HMC5883L_TestConnection()
{
    uint8_t tmp[3] = {0};
    HMC5883L_ReadBytes(HMC5883L_RA_ID_A, tmp, 3);
    return tmp[0] == 'H' && tmp[1] == '4' && tmp[2] == '3';
}
// CONFIG_A register

/** Get number of samples averaged per measurement.
 * @return Current samples averaged per measurement (0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_AVERAGING_8
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
uint8_t HMC5883L_GetSampleAveraging()
{
    return HMC5883L_ReadBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH);
}

/** Set number of samples averaged per measurement.
 * @param averaging New samples averaged per measurement setting(0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
void HMC5883L_SetSampleAveraging(uint8_t averaging)
{
    HMC5883L_WriteBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_AVERAGE_BIT, HMC5883L_CRA_AVERAGE_LENGTH, averaging);
}

/** \code
 * Get data output rate value.
 * The Table below shows all selectable output rates in continuous measurement
 * mode. All three channels shall be measured within a given output rate. Other
 * output rates with maximum rate of 160 Hz can be achieved by monitoring DRDY
 * interrupt pin in single measurement mode.
 *
 * Value | Typical Data Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.75
 * 1     | 1.5
 * 2     | 3
 * 3     | 7.5
 * 4     | 15 (Default)
 * 5     | 30
 * 6     | 75
 * 7     | Not used
 * \endcode
 * @return Current rate of data output to registers
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
uint8_t HMC5883L_GetDataRate()
{
    return HMC5883L_ReadBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH);
}

/** Set data output rate value.
 * @param rate Rate of data output to registers
 * @see HMC5883L_SetDataRate()
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
void HMC5883L_SetDataRate(uint8_t rate)
{
    HMC5883L_WriteBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_RATE_BIT, HMC5883L_CRA_RATE_LENGTH, rate);
}

/** Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
uint8_t HMC5883L_GetMeasurementBias()
{
    return HMC5883L_ReadBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH);
}

/** Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
void HMC5883L_SetMeasurementBias(uint8_t bias)
{
    HMC5883L_WriteBits(HMC5883L_RA_CONFIG_A, HMC5883L_CRA_BIAS_BIT, HMC5883L_CRA_BIAS_LENGTH, bias);
}

// CONFIG_B register

/** \code
 * Get magnetic field gain value.
 * The table below shows nominal gain settings. Use the "Gain" column to convert
 * counts to Gauss. Choose a lower gain value (higher GN#) when total field
 * strength causes overflow in one of the data output registers (saturation).
 * The data output range for all settings is 0xF800-0x07FF (-2048 - 2047).
 *
 * Value | Field Range | Gain (LSB/Gauss)
 * ------+-------------+-----------------
 * 0     | +/- 0.88 Ga | 1370
 * 1     | +/- 1.3 Ga  | 1090 (Default)
 * 2     | +/- 1.9 Ga  | 820
 * 3     | +/- 2.5 Ga  | 660
 * 4     | +/- 4.0 Ga  | 440
 * 5     | +/- 4.7 Ga  | 390
 * 6     | +/- 5.6 Ga  | 330
 * 7     | +/- 8.1 Ga  | 230
 * \endcode
 * @return Current magnetic field gain value
 * @see HMC5883L_GAIN_1090
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
uint8_t HMC5883L_GetGain()
{
    return HMC5883L_ReadBits(HMC5883L_RA_CONFIG_B, HMC5883L_CRB_GAIN_BIT, HMC5883L_CRB_GAIN_LENGTH);
}

/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see HMC5883L_GetGain()
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
void HMC5883L_SetGain(uint8_t gain)
{
    // use this method to guarantee that bits 4-0 are set to zero, which is a
    // requirement specified in the datasheet;
    HMC5883L_WriteReg(HMC5883L_RA_CONFIG_B, gain << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1));
}

// MODE register

/** Get measurement mode.
 * In continuous-measurement mode, the device continuously performs measurements
 * and places the result in the data register. RDY goes high when new data is
 * placed in all three registers. After a power-on or a write to the mode or
 * configuration register, the first measurement set is available from all three
 * data output registers after a period of 2/fDO and subsequent measurements are
 * available at a frequency of fDO, where fDO is the frequency of data output.
 *
 * When single-measurement mode (default) is selected, device performs a single
 * measurement, sets RDY high and returned to idle mode. Mode register returns
 * to idle mode bit values. The measurement remains in the data output register
 * and RDY remains high until the data output register is read or another
 * measurement is performed.
 *
 * @return Current measurement mode
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
uint8_t HMC5883L_GetMode()
{
    return HMC5883L_ReadBits(HMC5883L_RA_MODE, HMC5883L_MODEREG_BIT, HMC5883L_MODEREG_LENGTH);
}

/** Set measurement mode.
 * @param newMode New measurement mode
 * @see HMC5883L_GetMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
void HMC5883L_SetMode(uint8_t newMode)
{
    // use this method to guarantee that bits 7-2 are set to zero, which is a
    // requirement specified in the datasheet;
    uint8_t tmp = HMC5883Lmode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1);
    HMC5883L_WriteReg(HMC5883L_RA_MODE, tmp);
    HMC5883Lmode = newMode;  // track to tell if we have to clear bit 7 after a read
}

// DATA* registers

/** Get 3-axis heading measurements.
 * In the event the ADC reading overflows or underflows for the given channel,
 * or if there is a math overflow during the bias measurement, this data
 * register will contain the value -4096. This register value will clear when
 * after the next valid measurement is made. Note that this method automatically
 * clears the appropriate bit in the MODE register if Single mode is active.
 * @param x 16-bit signed integer container for X,Y,Z-axis heading
 * @see HMC5883L_RA_DATAX_H
 */
void HMC5883L_GetHeading(int16_t* Mag /*x,z,y*/)
{
    uint8_t tmpbuff[6] = {0};
    HMC5883L_ReadBytes(HMC5883L_RA_DATAX_H, tmpbuff, 6);

    uint8_t tmp = HMC5883L_MODE_SINGLE << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1);
    if (HMC5883Lmode == HMC5883L_MODE_SINGLE)
        HMC5883L_WriteReg(HMC5883L_RA_MODE, tmp);

    for (int i = 0; i < 3; ++i)
        Mag[i] = ((int16_t)((uint16_t)tmpbuff[2 * i] << 8) + tmpbuff[2 * i + 1]);
}

// STATUS register

/** Get data output register lock status.
 * This bit is set when this some but not all for of the six data output
 * registers have been read. When this bit is set, the six data output registers
 * are locked and any new data will not be placed in these register until one of
 * three conditions are met: one, all six bytes have been read or the mode
 * changed, two, the mode is changed, or three, the measurement configuration is
 * changed.
 * @return Data output register lock status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_LOCK_BIT
 */
uint8_t HMC5883L_GetLockStatus()
{
    return HMC5883L_ReadBit(HMC5883L_RA_STATUS, HMC5883L_STATUS_LOCK_BIT) == 0x01;
}

/** Get data ready status.
 * This bit is set when data is written to all six data registers, and cleared
 * when the device initiates a write to the data output registers and after one
 * or more of the data output registers are written to. When RDY bit is clear it
 * shall remain cleared for 250 us. DRDY pin can be used as an alternative to
 * the status register for monitoring the device for measurement data.
 * @return Data ready status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_READY_BIT
 */
uint8_t HMC5883L_GetReadyStatus()
{
    return HMC5883L_ReadBit(HMC5883L_RA_STATUS, HMC5883L_STATUS_READY_BIT) == 0x01;
}

void HMC5883L_WriteBits(uint8_t reg, uint8_t start, uint8_t length, uint8_t data)
{
    uint8_t tmp  = HMC5883L_ReadReg(reg);
    uint8_t mask = ((1 << length) - 1) << (start - length + 1);
    data <<= (start - length + 1);  // shift data into correct position
    data &= mask;                   // zero all non-important bits in data
    tmp &= ~(mask);                 // zero all important bits in existing byte
    tmp |= data;                    // combine data with existing byte
    HMC5883L_WriteReg(reg, tmp);
}

void HMC5883L_WriteBit(uint8_t reg, uint8_t index, uint8_t data)
{
    uint8_t tmp = HMC5883L_ReadReg(reg);
    HMC5883L_WriteReg(reg, (data != 0) ? (tmp | (1 << index)) : (tmp & ~(1 << index)));
}

uint8_t HMC5883L_ReadBits(uint8_t reg, uint8_t start, uint8_t length)
{
    uint8_t tmp  = HMC5883L_ReadReg(reg);
    uint8_t mask = ((1 << length) - 1) << (start - length + 1);
    tmp &= mask;
    tmp >>= (start - length + 1);
    return tmp;
}

uint8_t HMC5883L_ReadBit(uint8_t reg, uint8_t index)
{
    return (HMC5883L_ReadReg(reg) >> index) & 0x01;
}

void HMC5883L_WriteReg(uint8_t reg, uint8_t buff)
{
    HAL_I2C_Mem_Write(&HMC5883L_I2C, HMC5883L_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, HAL_MAX_DELAY);
}

uint8_t HMC5883L_ReadReg(uint8_t reg)
{
    uint8_t buff;
    HAL_I2C_Mem_Read(&HMC5883L_I2C, HMC5883L_DEV, reg, I2C_MEMADD_SIZE_8BIT, &buff, 1, HAL_MAX_DELAY);
    return buff;
}

void HMC5883L_ReadBytes(uint8_t reg, uint8_t* buff, uint16_t size)
{
    HAL_I2C_Mem_Read(&HMC5883L_I2C, HMC5883L_DEV, reg, I2C_MEMADD_SIZE_8BIT, buff, size, HAL_MAX_DELAY);
}
