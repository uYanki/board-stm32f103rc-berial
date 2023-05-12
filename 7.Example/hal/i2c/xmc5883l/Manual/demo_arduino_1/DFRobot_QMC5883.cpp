/*!
 * @file DFRobot_QMC5883.cpp
 * @brief Compatible with QMC5883 and QMC5883
 * @n 3-Axis Digital Compass IC
 *
 * @copyright	[DFRobot](http://www.dfrobot.com), 2017
 * @copyright	GNU Lesser General Public License
 *
 * @author [dexian.huang](952838602@qq.com)
 * @version  V1.0
 * @date  2017-7-3
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "DFRobot_QMC5883.h"
#include <Wire.h>

bool DFRobot_QMC5883::begin()
{
  bool ret = false;
  if (ICType == IC_NONE) {
    for (uint8_t i = 0; i < 5; i++) {
      Wire.begin();
      Wire.beginTransmission(HMC5883L_ADDRESS);
      if (Wire.endTransmission() == 0) {
        ICType       = IC_HMC5883L;
        this->ICAddr = HMC5883L_ADDRESS;
        Serial.println("ICType = IC_HMC5883L");
        break;
      }
    }
  }
  if (ICType == IC_NONE) {
    for (uint8_t i = 0; i < 5; i++) {
      Wire.begin();
      Wire.beginTransmission(QMC5883_ADDRESS);
      if (Wire.endTransmission() == 0) {
        ICType       = IC_QMC5883;
        this->ICAddr = QMC5883_ADDRESS;
        break;
      }
    }
  }
  if (ICType == IC_NONE) {
    for (uint8_t i = 0; i < 5; i++) {
      Wire.begin();
      Wire.beginTransmission(VCM5883L_ADDRESS);
      if (Wire.endTransmission() == 0) {
        ICType       = IC_VCM5883L;
        this->ICAddr = VCM5883L_ADDRESS;
        break;
      }
    }
  }
  switch (ICType) {
    case IC_NONE: ret = false; break;
    case IC_HMC5883L:
      if ((fastRegister8(HMC5883L_REG_IDENT_A) != 0x48) ||
          (fastRegister8(HMC5883L_REG_IDENT_B) != 0x34) ||
          (fastRegister8(HMC5883L_REG_IDENT_C) != 0x33)) {
        return false;
      }
      setRange(HMC5883L_RANGE_1_3GA);
      setMeasurementMode(HMC5883L_CONTINOUS);
      setDataRate(HMC5883L_DATARATE_15HZ);
      setSamples(HMC5883L_SAMPLES_1);
      mgPerDigit = 0.92f;
      ret        = true;
      Serial.println("IC_HMC5883L");
      break;
    case IC_QMC5883:
      writeRegister8(QMC5883_REG_IDENT_B, 0X01);
      writeRegister8(QMC5883_REG_IDENT_C, 0X40);
      writeRegister8(QMC5883_REG_IDENT_D, 0X01);
      writeRegister8(QMC5883_REG_CONFIG_1, 0X1D);
      if ((fastRegister8(QMC5883_REG_IDENT_B) != 0x01) ||
          (fastRegister8(QMC5883_REG_IDENT_C) != 0x40) ||
          (fastRegister8(QMC5883_REG_IDENT_D) != 0x01)) {
        return false;
      }
      setRange(QMC5883_RANGE_8GA);
      setMeasurementMode(QMC5883_CONTINOUS);
      setDataRate(QMC5883_DATARATE_50HZ);
      setSamples(QMC5883_SAMPLES_8);
      mgPerDigit = 4.35f;
      ret        = true;
      Serial.println("IC_QMC5883");
      break;
    case IC_VCM5883L:
      writeRegister8(VCM5883L_CTR_REG1, 0X00);
      writeRegister8(VCM5883L_CTR_REG2, 0X4D);
      ret = true;
      Serial.println("IC_VCM5883L");
      break;
    default: ret = false; break;
  }
  return ret;
}

Vector DFRobot_QMC5883::readRaw(void)
{
  if (ICType == IC_HMC5883L) {
    v.XAxis = readRegister16(HMC5883L_REG_OUT_X_M);
    v.YAxis = readRegister16(HMC5883L_REG_OUT_Y_M);
    v.ZAxis = readRegister16(HMC5883L_REG_OUT_Z_M);
  }
  else if (ICType == IC_QMC5883) {
    v.XAxis = readRegister16(QMC5883_REG_OUT_X_L);
    v.YAxis = readRegister16(QMC5883_REG_OUT_Y_L);
    v.ZAxis = readRegister16(QMC5883_REG_OUT_Z_L);
  }
  else if (ICType == IC_VCM5883L) {
    v.XAxis = -readRegister16(VCM5883L_REG_OUT_X_L);
    v.YAxis = -readRegister16(VCM5883L_REG_OUT_Y_L);
    v.ZAxis = -readRegister16(VCM5883L_REG_OUT_Z_L);
  }
  v.AngleXY =
      (atan2((double)v.YAxis, (double)v.XAxis) * (180 / 3.14159265) + 180);
  v.AngleXZ =
      (atan2((double)v.ZAxis, (double)v.XAxis) * (180 / 3.14159265) + 180);
  v.AngleYZ =
      (atan2((double)v.ZAxis, (double)v.YAxis) * (180 / 3.14159265) + 180);
  return v;
}
void DFRobot_QMC5883::calibrate()
{
  if (v.XAxis < minX)
    minX = v.XAxis;
  if (v.XAxis > maxX)
    maxX = v.XAxis;
  if (v.YAxis < minY)
    minY = v.YAxis;
  if (v.YAxis > maxY)
    maxY = v.YAxis;
  if (v.ZAxis < minZ)
    minZ = v.ZAxis;
  if (v.ZAxis > maxZ)
    maxZ = v.ZAxis;
}
void DFRobot_QMC5883::initMinMax()
{
  minX = v.XAxis;
  maxX = v.XAxis;
  minY = v.YAxis;
  maxY = v.YAxis;
  minZ = v.ZAxis;
  maxZ = v.ZAxis;
}

void DFRobot_QMC5883::setRange(QMC5883_range_t range)
{
  if (ICType == IC_HMC5883L) {
    switch (range) {
      case HMC5883L_RANGE_0_88GA: Gauss_LSB_XY = 1370.0; break;
      case HMC5883L_RANGE_1_3GA: Gauss_LSB_XY = 1090.0; break;
      case HMC5883L_RANGE_1_9GA: Gauss_LSB_XY = 820.0; break;
      case HMC5883L_RANGE_2_5GA: Gauss_LSB_XY = 660.0; break;
      case HMC5883L_RANGE_4GA: Gauss_LSB_XY = 440.0; break;
      case HMC5883L_RANGE_4_7GA: Gauss_LSB_XY = 390.0; break;
      case HMC5883L_RANGE_5_6GA: Gauss_LSB_XY = 330.0; break;
      case HMC5883L_RANGE_8_1GA: Gauss_LSB_XY = 230.0; break;
      default: break;
    }

    writeRegister8(HMC5883L_REG_CONFIG_B, range << 5);
  }
  else if (ICType == IC_QMC5883) {
    switch (range) {
      case QMC5883_RANGE_2GA: mgPerDigit = 1.22f; break;
      case QMC5883_RANGE_8GA: mgPerDigit = 4.35f; break;
      default: break;
    }
    writeRegister8(QMC5883_REG_CONFIG_2, range << 4);
  }
  else if (ICType == IC_VCM5883L) {
    // default 8G
  }
}

QMC5883_range_t DFRobot_QMC5883::getRange(void)
{
  QMC5883_range_t ret;
  switch (ICType) {
    case IC_HMC5883L:
      ret =
          (QMC5883_range_t)((readRegister8(HMC5883L_REG_CONFIG_B) >> 5));
      break;
    case IC_QMC5883:
      ret = (QMC5883_range_t)((readRegister8(QMC5883_REG_CONFIG_2) >> 4));
      break;
    case IC_VCM5883L: ret = QMC5883_RANGE_8GA; break;
    default: ret = QMC5883_RANGE_8GA; break;
  }
  return ret;
}

void DFRobot_QMC5883::setMeasurementMode(QMC5883_mode_t mode)
{
  uint8_t value;
  switch (ICType) {
    case IC_HMC5883L:
      value = readRegister8(HMC5883L_REG_MODE);
      value &= 0b11111100;
      value |= mode;
      writeRegister8(HMC5883L_REG_MODE, value);
      break;
    case IC_QMC5883:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0xfc;
      value |= mode;
      writeRegister8(QMC5883_REG_CONFIG_1, value);
      break;
    case IC_VCM5883L:
      value = readRegister8(VCM5883L_CTR_REG2);
      value &= 0xFE;
      value |= mode;
      writeRegister8(VCM5883L_CTR_REG2, value);
      break;
    default: break;
  }
}

QMC5883_mode_t DFRobot_QMC5883::getMeasurementMode(void)
{
  uint8_t value = 0;
  switch (ICType) {
    case IC_HMC5883L:
      value = readRegister8(HMC5883L_REG_MODE);
      value &= 0b00000011;
      break;
    case IC_QMC5883:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0b00000011;
      break;
    case IC_VCM5883L:
      value = readRegister8(VCM5883L_CTR_REG2);
      value &= 0b00000001;
      break;
    default: break;
  }
  return (QMC5883_mode_t)value;
}

void DFRobot_QMC5883::setDataRate(QMC5883_dataRate_t dataRate)
{
  uint8_t value;
  switch (ICType) {
    case IC_HMC5883L:
      value = readRegister8(HMC5883L_REG_CONFIG_A);
      value &= 0b11100011;
      value |= (dataRate << 2);
      writeRegister8(HMC5883L_REG_CONFIG_A, value);
      break;
    case IC_QMC5883:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0xf3;
      value |= (dataRate << 2);
      writeRegister8(QMC5883_REG_CONFIG_1, value);
      break;
    case IC_VCM5883L:
      value = readRegister8(VCM5883L_CTR_REG2);
      value &= 0xf3;
      value |= (dataRate << 2);
      writeRegister8(VCM5883L_CTR_REG2, value);
      break;
    default: break;
  }
}

QMC5883_dataRate_t DFRobot_QMC5883::getDataRate(void)
{
  uint8_t value = 0;
  switch (ICType) {
    case IC_HMC5883L:
      value = readRegister8(HMC5883L_REG_CONFIG_A);
      value &= 0b00011100;
      value >>= 2;
      break;
    case IC_QMC5883:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0b00001100;
      value >>= 2;
      break;
    case IC_VCM5883L:
      value = readRegister8(VCM5883L_CTR_REG2);
      value &= 0b00001100;
      value >>= 2;
      break;
    default: break;
  }
  return (QMC5883_dataRate_t)value;
}

void DFRobot_QMC5883::setSamples(QMC5883_samples_t samples)
{
  uint8_t value;
  switch (ICType) {
    case IC_HMC5883L:
      value = readRegister8(HMC5883L_REG_CONFIG_A);
      value &= 0b10011111;
      value |= (samples << 5);
      writeRegister8(HMC5883L_REG_CONFIG_A, value);
      break;
    case IC_QMC5883:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0x3f;
      value |= (samples << 6);
      writeRegister8(QMC5883_REG_CONFIG_1, value);
      break;
    case IC_VCM5883L:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0x3f;
      value |= (samples << 6);
      writeRegister8(QMC5883_REG_CONFIG_1, value);
      break;
    default: break;
  }
}

QMC5883_samples_t DFRobot_QMC5883::getSamples(void)
{
  uint8_t value = 0;
  switch (ICType) {
    case IC_HMC5883L:
      value = readRegister8(HMC5883L_REG_CONFIG_A);
      value &= 0b01100000;
      value >>= 5;
      break;
    case IC_QMC5883:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0x3f;
      value >>= 6;
      break;
    case IC_VCM5883L:
      value = readRegister8(QMC5883_REG_CONFIG_1);
      value &= 0x3f;
      value >>= 6;
      break;
    default: break;
  }
  return (QMC5883_samples_t)value;
}

void DFRobot_QMC5883::setDeclinationAngle(float declinationAngle)
{
  this->ICdeclinationAngle = declinationAngle;
}

void DFRobot_QMC5883::getHeadingDegrees()
{
  // double XuT,YuT,ZuT;
  // readRaw();
  // XuT = v.XAxis / Gauss_LSB_XY * 100;
  // YuT = v.YAxis / Gauss_LSB_XY * 100;
  // ZuT = v.ZAxis / Gauss_LSB_XY * 100;
  float heading = atan2(v.YAxis, v.XAxis);
  heading += this->ICdeclinationAngle;
  if (heading < 0)
    heading += 2 * PI;
  if (heading > 2 * PI)
    heading -= 2 * PI;
  v.HeadingDegress = heading * 180 / PI;
}
// Write byte to register
void DFRobot_QMC5883::writeRegister8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(this->ICAddr);
#if ARDUINO >= 100
  Wire.write(reg);
  Wire.write(value);
#else
  Wire.send(reg);
  Wire.send(value);
#endif
  Wire.endTransmission();
}
// Read byte to register
uint8_t DFRobot_QMC5883::fastRegister8(uint8_t reg)
{
  uint8_t value = 0;
  Wire.beginTransmission(this->ICAddr);
#if ARDUINO >= 100
  Wire.write(reg);
#else
  Wire.send(reg);
#endif
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)this->ICAddr, (uint8_t)1);
#if ARDUINO >= 100
  value = Wire.read();
#else
  value = Wire.receive();
#endif
  Wire.endTransmission();
  return value;
}

// Read byte from register
uint8_t DFRobot_QMC5883::readRegister8(uint8_t reg)
{
  uint8_t value = 0;
  Wire.beginTransmission(this->ICAddr);
#if ARDUINO >= 100
  Wire.write(reg);
#else
  Wire.send(reg);
#endif
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)this->ICAddr, (uint8_t)1);
  while (!Wire.available()) {};
#if ARDUINO >= 100
  value = Wire.read();
#else
  value = Wire.receive();
#endif
  return value;
}
// Read word from register
int16_t DFRobot_QMC5883::readRegister16(uint8_t reg)
{
  int16_t value = 0;
  uint8_t vha, vla;
  Wire.beginTransmission(this->ICAddr);
#if ARDUINO >= 100
  Wire.write(reg);
#else
  Wire.send(reg);
#endif
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)this->ICAddr, (uint8_t)2);
  while (!Wire.available()) {};
  if (ICType == IC_HMC5883L) {
#if ARDUINO >= 100
    vha = Wire.read();
    vla = Wire.read();
#else
    vha = Wire.receive();
    vla = Wire.receive();
#endif
  }
  else {
#if ARDUINO >= 100
    vla = Wire.read();
    vha = Wire.read();
#else
    vla = Wire.receive();
    vha = Wire.receive();
#endif
  }
  value = vha << 8 | vla;
  return value;
}

int DFRobot_QMC5883::getICType(void)
{
  return ICType;
}
