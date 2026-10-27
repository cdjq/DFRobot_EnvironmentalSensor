/*!
 * @file  DFRobot_EnvironmentalSensor.cpp
 * @brief Basic structure of DFRobot_EnvironmentalSensor class, the implementation of basic method
 *
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license   The MIT License (MIT)
 * @author    [thdyyl](yuanlong.yu@dfrobot.com)
 * @version   V1.3.0
 * @date      2026-10-26
 * @url       https://github.com/DFRobot/DFRobot_EnvironmentalSensor
 */

#include "DFRobot_EnvironmentalSensor.h"

#ifdef __AVR__
typedef uint16_t platformBitWidth_t;
#else
typedef uint32_t platformBitWidth_t;
#endif

static const float ML8511_ADC_REFERENCE_VOLTAGE = 3.0f;
static const float ML8511_ADC_FULL_SCALE        = 1024.0f;
static const float ML8511_OUTPUT_MIN_VOLTAGE    = 0.99f;
static const float ML8511_OUTPUT_MAX_VOLTAGE    = 2.90f;
static const float ML8511_UVI_MAX               = 15.0f;

// V2.0 firmware configures LTR390 for 20-bit resolution and 6x gain.
// The datasheet lists 2300 counts/UVI at 20-bit, 18x gain, so 6x gain is 2300/3.
static const float LTR390_COUNTS_PER_UVI = 2300.0f / 3.0f;
static const float LTR390_WINDOW_FACTOR  = 1.0f;
// 0.23 mW/cm2 per UVI is the GUVA sunlight scale. LTR390's 2300-based UVI
// reads about 2x high versus GUVA (3.35 vs 1.58 mW/cm2), so scale 0.23 by 1.58/3.35.
static const float LTR390_MW_CM2_PER_UVI = 0.23f * 1.58f / 3.35f;

static const float S12SD_ADC_REFERENCE_VOLTAGE = 3.0f;
static const float S12SD_ADC_FULL_SCALE        = 1023.0f;
static const float S12SD_TRANSIMPEDANCE_OHM    = 4303300.0f;
static const float S12SD_CURRENT_NA_PER_MW_CM2 = 113.0f;

DFRobot_EnvironmentalSensor::DFRobot_EnvironmentalSensor(uint8_t addr, TwoWire *pWire)
{
  _pWire = pWire;
  _addr  = addr;
};

DFRobot_EnvironmentalSensor::DFRobot_EnvironmentalSensor(uint8_t addr, Stream *s) : DFRobot_RTU(s)
{
  _s    = s;
  _addr = addr;
}

int8_t DFRobot_EnvironmentalSensor::begin(void)
{
  delay(500);
  setTimeoutTimeMs(200);
  if (_pWire) {
    _pWire->begin();
  }
  if (_addr > 0xF7) {
    DBG("Invaild Device addr.");
  }
  if (_addr != 0) {
    if (!detectDeviceAddress(_addr)) {
      DBG("Device addr Error.");
      return -1;
    }
  } else {
    return -1;
  }
  return 0;
}

uint16_t DFRobot_EnvironmentalSensor::getDevicePID(void)
{
  uint8_t buf[2];
  if (_pWire) {
    readReg(REG_PID, buf, 2);
  } else {
    readReg(REG_PID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);
}

uint16_t DFRobot_EnvironmentalSensor::getDeviceVID(void)
{
  uint8_t buf[2];
  if (_pWire) {
    readReg(REG_VID, buf, 2);
  } else {
    readReg(REG_VID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);
}

float DFRobot_EnvironmentalSensor::getTemperature(uint8_t unist)
{
  uint8_t  buffer[2];
  uint16_t data;
  float    temp;
  readReg(REG_TEMP, buffer, 2);
  data = buffer[0] << 8 | buffer[1];
  temp = (-45) + ((data * 175.00) / 1024.00 / 64.00);
  if (unist == TEMP_F) {
    temp = temp * 1.8 + 32;
  }
  return temp;
}

float DFRobot_EnvironmentalSensor::getHumidity(void)
{
  uint8_t  buffer[2];
  float    humidity;
  uint16_t data;
  readReg(REG_HUMIDITY, buffer, 2);
  data     = ((buffer[0] << 8 | buffer[1]));
  humidity = (float)data * 100 / 65536;
  return humidity;
}

float DFRobot_EnvironmentalSensor::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float DFRobot_EnvironmentalSensor::getUltravioletIntensity(eUVSOC version)
{
  uint16_t uvLevel;
  uint8_t  buffer[2];
  float    ultraviolet = 0.0f;
  readReg(REG_ULTRAVIOLET_INTENSITY, buffer, 2);
  uvLevel = buffer[0] << 8 | buffer[1];
  DBG(uvLevel);

  // The register holds different raw data on each hardware version, so the
  // conversion must match the board. See eUVSOC in the header for details.
  if (version == eSensorV1_0) {
    // V1.0 (ML8511): register value is a 10-bit ADC reading. Map 0.99 V to 2.90 V onto UVI 0 to 15.
    float outputVoltage = ML8511_ADC_REFERENCE_VOLTAGE * uvLevel / ML8511_ADC_FULL_SCALE;
    if (outputVoltage <= ML8511_OUTPUT_MIN_VOLTAGE) {
      ultraviolet = 0.0f;
    } else if (outputVoltage >= ML8511_OUTPUT_MAX_VOLTAGE) {
      ultraviolet = ML8511_UVI_MAX;
    } else {
      ultraviolet = mapfloat(outputVoltage, ML8511_OUTPUT_MIN_VOLTAGE, ML8511_OUTPUT_MAX_VOLTAGE, 0.0f, ML8511_UVI_MAX);
    }
  } else if (version == eSensorV2_0) {
    // V2.0 (LTR390): register value is a digital count. Convert with 2300 counts/UVI, then to GUVA-equivalent mW/cm2.
    float uvi   = ((float)uvLevel / LTR390_COUNTS_PER_UVI) * LTR390_WINDOW_FACTOR;
    ultraviolet = uvi * LTR390_MW_CM2_PER_UVI;
  } else if (version == eSensorV3_0) {
    // V3.0 (GUVA-S12SD): register value is a 10-bit ADC reading from the amplifier output.
    // Equivalent irradiance under a 352 nm UVA lamp.
    uvLevel             = uvLevel > 1 ? uvLevel : 0;
    float outputVoltage = S12SD_ADC_REFERENCE_VOLTAGE * uvLevel / S12SD_ADC_FULL_SCALE;
    float currentNa     = (outputVoltage * 1000000000.0f) / S12SD_TRANSIMPEDANCE_OHM;
    ultraviolet         = currentNa / S12SD_CURRENT_NA_PER_MW_CM2;
  }

  return ultraviolet;
}

float DFRobot_EnvironmentalSensor::getLuminousIntensity(void)
{
  uint16_t data;
  uint8_t  buffer[2];
  readReg(REG_LUMINOUS_INTENSITY, buffer, 2);

  data           = buffer[0] << 8 | buffer[1];
  float luminous = data;
  luminous       = luminous * (1.0023f + luminous * (8.1488e-5f + luminous * (-9.3924e-9f + luminous * 6.0135e-13f)));
  return luminous;
}

uint16_t DFRobot_EnvironmentalSensor::getAtmospherePressure(uint8_t units)
{
  uint16_t atmosphere;
  uint8_t  buffer[2];
  readReg(REG_ATMOSPHERIC_PRESSURE, buffer, 2);
  atmosphere = buffer[0] << 8 | buffer[1];
  if (units == KPA) {
    atmosphere /= 10;
  }
  return atmosphere;
}

float DFRobot_EnvironmentalSensor::getElevation(void)
{
  uint16_t atmosphere;
  uint8_t  buffer[2];
  readReg(REG_ATMOSPHERIC_PRESSURE, buffer, 2);
  atmosphere      = buffer[0] << 8 | buffer[1];
  float elevation = 44330 * (1.0 - pow(atmosphere / 1015.0f, 0.1903));
  return elevation;
}

uint8_t DFRobot_EnvironmentalSensor::readReg(uint16_t reg, void *pBuf, uint8_t size)
{
  uint8_t *pData = (uint8_t *)pBuf;

  if (pBuf == NULL) {
    DBG("data error");
    return 0;
  }
  if (_pWire) {
    uint8_t regAddr = reg * 2;
    _pWire->beginTransmission(_addr);
    _pWire->write(regAddr);
    _pWire->endTransmission();
    _pWire->requestFrom(_addr, size);
    for (uint8_t i = 0; i < size; i++) pData[i] = _pWire->read();

    return size;
  } else {
    return readInputRegister(_addr, reg, pData, size);
  }
}

bool DFRobot_EnvironmentalSensor::detectDeviceAddress(uint8_t addr)
{
  if (_pWire) {
    uint8_t buf[2];
    readReg(REG_DEVICE_ADDR, buf, 2);
    if (addr == ((buf[0] << 8 | buf[1]) & 0xFF))
      return true;

  } else {
    uint16_t ret = readInputRegister(addr, REG_DEVICE_ADDR);
    DBG(ret);
    if ((ret & 0xFF) == addr)
      return true;
  }
  return false;
}
