# -*- coding: utf-8 -*
'''!
@file        dfrobot_environmental_sensor.py
@brief       DFRobot_EnvironmentalSensor library is developed for DFRobot environmental sensor.
@copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT License (MIT)
@author      [thdyyl](yuanlong.yu@dfrobot.com)
@version     V1.3.0
@date        2026-10-26
@url         https://github.com/DFRobot/DFRobot_EnvironmentalSensor
'''

import serial
import time
import smbus
import os
import math
import RPi.GPIO as GPIO
import math

import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu

I2C_MODE = 0x01
UART_MODE = 0x02
DEV_ADDRESS = 0x22

DEVICE_VID = 0x3343
DEVICE_ADDRESS = 0x22

HPA = 0x01
KPA = 0x02
TEMP_C = 0x03
TEMP_F = 0x04

# Hardware version of the SEN0500/SEN0501 board, printed on the front silkscreen,
# e.g. "Multifunctional Environmental Sensor V3.0".
# Temperature, humidity, light, pressure and altitude are identical on all versions.
# Only the UV channel differs:
#   V1.0: ML8511 analog UV sensor, 10-bit ADC, get_ultraviolet_intensity() returns a UV index (0-15, unitless)
#   V2.0: LTR390 digital UV sensor, raw count, get_ultraviolet_intensity() returns UVA irradiance in mW/cm2
#   V3.0: GUVA-S12SD analog UV sensor, 10-bit ADC, get_ultraviolet_intensity() returns UVA irradiance in mW/cm2
# Passing the wrong version does not fail; it only produces a wrong UV value.
SENSOR_V1_0 = 0x00
SENSOR_V3_0 = 0x01
SENSOR_V2_0 = 0x02

# Legacy names (UV sensor chip), kept for backward compatibility. Prefer SENSOR_Vx_x.
ML8511 = SENSOR_V1_0
S12SD = SENSOR_V3_0
LTR390UV = SENSOR_V2_0
# Alias for a misspelled constant name.
S12DS = S12SD

ML8511_ADC_REFERENCE_VOLTAGE = 3.0
ML8511_ADC_FULL_SCALE = 1024.0
ML8511_OUTPUT_MIN_VOLTAGE = 0.99
ML8511_OUTPUT_MAX_VOLTAGE = 2.90
ML8511_UVI_MAX = 15.0

# V2.0 firmware uses LTR390 at 20-bit resolution and 6x gain.
LTR390_COUNTS_PER_UVI = 2300.0 / 3.0
LTR390_WINDOW_FACTOR = 1.0
# GUVA sunlight scale is 0.23 mW/cm2 per UVI. LTR390 UVI is about 2x high
# versus GUVA (3.35 vs 1.58 mW/cm2), so scale 0.23 by 1.58/3.35.
LTR390_MW_CM2_PER_UVI = 0.23 * 1.58 / 3.35

S12SD_ADC_REFERENCE_VOLTAGE = 3.0
S12SD_ADC_FULL_SCALE = 1023.0
S12SD_TRANSIMPEDANCE_OHM = 4303300.0
S12SD_CURRENT_NA_PER_MW_CM2 = 113.0


class DFRobot_Environmental_Sensor:
  '''!
  @brief Define DFRobot_Environmental_Sensor basic class
  @details For driving the environmental sensor
  '''

  def __init__(self, bus, baud=9600):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self._uart_i2c = I2C_MODE

    else:
      self.master = modbus_rtu.RtuMaster(serial.Serial(port="/dev/ttyAMA0", baudrate=baud, bytesize=8, parity='N', stopbits=1))
      self.master.set_timeout(1.0)
      self._uart_i2c = UART_MODE

  def _detect_device_address(self):
    '''!
    @brief Get sensor address
    @return  Return sensor address
    '''
    return self._read_value(0x04, 2)

  def _read_value(self, reg_addr, length):
    """Read and decode a register, returning None when the read fails."""
    rbuf = self._read_reg(reg_addr, length)
    if rbuf == -1 or rbuf is None:
      return None

    expected_length = 2 if self._uart_i2c == I2C_MODE else 1
    if not isinstance(rbuf, (list, tuple)) or len(rbuf) < expected_length:
      return None

    if self._uart_i2c == I2C_MODE:
      return rbuf[0] << 8 | rbuf[1]
    return rbuf[0]

  def begin(self):
    '''!
    @brief Init SEN0500/SEN0501 sensor
    @return Return init status
    @retval 0  Succeed
    @retval -1 Failed
    '''
    if self._detect_device_address() != DEV_ADDRESS:
      return False
    return True

  def get_temperature(self, unist):
    '''!
    @brief Get SEN0500/SEN0501 temperature data
    @param units Temperature data unit select
    @n     TEMP_C ℃
    @n     TEMP_F ℉
    @return Return the obtained temperature data
    '''
    data = self._read_value(0x14, 2)
    if data is None:
      return -1
    temp = (-45) + ((data * 175.00) / 1024.00 / 64.00)
    if unist == TEMP_F:
      temp = temp * 1.8 + 32
    return round(temp, 2)

  def get_humidity(self):
    '''!
    @brief Get SEN0500/SEN0501 humidity data
    @return Return the obtained humidity data
    '''
    humidity = self._read_value(0x16, 2)
    if humidity is None:
      return -1
    humidity = (humidity / 1024) * 100 / 64
    return round(humidity, 2)

  def get_ultraviolet_intensity(self, version=SENSOR_V1_0):
    '''!
    @brief Get SEN0500/SEN0501 UV data
    @param version Hardware version printed on the board silkscreen
    @n SENSOR_V1_0: V1.0 board (ML8511), returns UV index, 0 to 15, unitless
    @n SENSOR_V2_0: V2.0 board (LTR390), returns equivalent UVA irradiance, mW/cm2
    @n SENSOR_V3_0: V3.0 board (GUVA-S12SD), returns equivalent UVA irradiance, mW/cm2
    @n Default is SENSOR_V1_0 to keep the behaviour of earlier library releases.
    @return Return the ultraviolet measurement for the selected hardware version
    '''
    data = self._read_value(0x10, 2)
    if data is None:
      return -1
    ultraviolet = 0.0
    # The register holds different raw data on each hardware version.
    if version == SENSOR_V1_0:
      outputVoltage = ML8511_ADC_REFERENCE_VOLTAGE * data / ML8511_ADC_FULL_SCALE
      if outputVoltage <= ML8511_OUTPUT_MIN_VOLTAGE:
        ultraviolet = 0.0
      elif outputVoltage >= ML8511_OUTPUT_MAX_VOLTAGE:
        ultraviolet = ML8511_UVI_MAX
      else:
        ultraviolet = (outputVoltage - ML8511_OUTPUT_MIN_VOLTAGE) * ML8511_UVI_MAX / (ML8511_OUTPUT_MAX_VOLTAGE - ML8511_OUTPUT_MIN_VOLTAGE)
    elif version == SENSOR_V2_0:
      uvi = data / LTR390_COUNTS_PER_UVI * LTR390_WINDOW_FACTOR
      ultraviolet = uvi * LTR390_MW_CM2_PER_UVI
    elif version == SENSOR_V3_0:
      data = data if data > 1 else 0
      outputVoltage = S12SD_ADC_REFERENCE_VOLTAGE * data / S12SD_ADC_FULL_SCALE
      currentNa = (outputVoltage * 1000000000.0) / S12SD_TRANSIMPEDANCE_OHM
      ultraviolet = currentNa / S12SD_CURRENT_NA_PER_MW_CM2
    return round(ultraviolet, 2)

  def get_luminousintensity(self):
    '''!
    @brief Get SEN0500/SEN0501 luminous intensity data
    @return Return the obtained luminous intensity data
    '''
    data = self._read_value(0x12, 2)
    if data is None:
      return -1
    luminous = data * (1.0023 + data * (8.1488e-5 + data * (-9.3924e-9 + data * 6.0135e-13)))
    return round(luminous, 2)

  def get_atmosphere_pressure(self, units):
    '''!
    @brief Get SEN0500/SEN0501 atmosphere pressure data
    @param units Atmosphere pressure data unit select
    @n            HPA Hectopascal
    @n            KPA Kilopascal
    @return Return the obtained atmosphere pressure data
    '''
    atmosphere = self._read_value(0x18, 2)
    if atmosphere is None:
      return -1
    if units == KPA:
      atmosphere /= 10
    return atmosphere

  def get_elevation(self):
    '''!
    @brief Get SEN0500/SEN0501 elevation data
    @return Return the obtained elevation data
    '''
    elevation = self._read_value(0x18, 2)
    if elevation is None:
      return -1
    elevation = 44330 * (1.0 - pow(elevation / 1015.0, 0.1903))
    return round(elevation, 2)


class DFRobot_Environmental_Sensor_I2C(DFRobot_Environmental_Sensor):
  '''!
  @brief An example of an i2c interface module
  '''

  def __init__(self, bus, addr):
    self._addr = addr
    DFRobot_Environmental_Sensor.__init__(self, bus, 0)

  def _read_reg(self, reg_addr, length):
    '''!
    @brief read the data from the register
    @param reg register address
    @param value read data
    '''
    try:
      rslt = self.i2cbus.read_i2c_block_data(self._addr, reg_addr, length)
    except Exception:
      rslt = -1
    return rslt


class DFRobot_Environmental_Sensor_UART(DFRobot_Environmental_Sensor):
  '''!
  @brief An example of an UART interface module
  '''

  def __init__(self, baud, addr):
    self._baud = baud
    self._addr = addr
    try:
      DFRobot_Environmental_Sensor.__init__(self, 0, self._baud)
    except:
      print("plese get root!")

  def _read_reg(self, reg_addr, length):
    '''!
    @brief Read data from the sensor
    '''
    try:
      return list(self.master.execute(self._addr, cst.READ_INPUT_REGISTERS, reg_addr // 2, length // 2))
    except Exception:
      return -1
