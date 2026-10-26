# -*- coding: utf-8 -*
'''!
@file  get_data.py
@brief This example reads all data from the SEN0501/SEN0500 sensor via UART or I2C.
@n     It works with every hardware version (V1.0 / V2.0 / V3.0). Set SENSOR_VERSION below
@n     to the version printed on the front silkscreen of your board, e.g.
@n     "Multifunctional Environmental Sensor V3.0".
@n
@n     Version differences: temperature, humidity, light, pressure and altitude are identical
@n     on all versions. Only the UV sensor and its output differ:
@n       V1.0  ML8511      -> UV index (0 to 15, unitless)
@n       V2.0  LTR390      -> equivalent UVA irradiance, mW/cm2
@n       V3.0  GUVA-S12SD  -> equivalent UVA irradiance, mW/cm2
@n     A wrong SENSOR_VERSION does not report an error, it only gives a wrong UV value.
@copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT License (MIT)
@author      [thdyyl](yuanlong.yu@dfrobot.com)
@version     V1.4.0
@date        2026-10-26
@url         https://github.com/DFRobot/DFRobot_EnvironmentalSensor
'''

from __future__ import print_function
import sys
import os

sys.path.append("../")
import time
import RPi.GPIO as GPIO

from DFRobot_Environmental_Sensor import *

'''
  Hardware version of your board, see the silkscreen on the front side.
    SENSOR_V1_0  ->  "... Sensor V1.0"
    SENSOR_V2_0  ->  "... Sensor V2.0"
    SENSOR_V3_0  ->  "... Sensor V3.0"
'''
SENSOR_VERSION = SENSOR_V3_0

'''
  Select communication mode
  ctype=1：UART
  ctype=0：IIC
'''
ctype = 0

ADDRESS = 0x22
I2C_1 = 0x01
SEN050X: DFRobot_Environmental_Sensor
if ctype == 0:
  SEN050X = DFRobot_Environmental_Sensor_I2C(I2C_1, ADDRESS)
else:
  SEN050X = DFRobot_Environmental_Sensor_UART(9600, ADDRESS)

'''
  Atmospheric pressure unit select
'''
HPA = 0x01
KPA = 0x02

'''
  Temperature unit select
'''
TEMP_C = 0x03
TEMP_F = 0x04


def setup():
  while SEN050X.begin() == False:
    print("Sensor initialize failed!!")
    time.sleep(1)
  print("Sensor initialize success!!")


def loop():
  ##Obtain sensor data
  print("-----------------------\r\n")
  print("Temp: " + str(SEN050X.get_temperature(TEMP_C)) + " 'C\r\n")
  print("Temp: " + str(SEN050X.get_temperature(TEMP_F)) + " 'F\r\n")
  print("Humidity: " + str(SEN050X.get_humidity()) + " %\r\n")
  ##V1.0 returns a UV index, V2.0 and V3.0 return irradiance in mW/cm2
  if SENSOR_VERSION == SENSOR_V1_0:
    print("UV index: " + str(SEN050X.get_ultraviolet_intensity(SENSOR_VERSION)) + "\r\n")
  else:
    print("Ultraviolet intensity: " + str(SEN050X.get_ultraviolet_intensity(SENSOR_VERSION)) + " mW/cm2\r\n")
  print("LuminousIntensity: " + str(SEN050X.get_luminousintensity()) + " lx\r\n")
  print("Atmospheric pressure: " + str(SEN050X.get_atmosphere_pressure(HPA)) + " hpa\r\n")
  print("Elevation: " + str(SEN050X.get_elevation()) + " m\r\n")
  print("-----------------------\r\n")
  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
