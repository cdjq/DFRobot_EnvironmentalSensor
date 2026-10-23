# -*- coding: utf-8 -*
'''!
@file  V1_0.py
@brief This example can obtain data from the SEN0501/SEN0500 V1.0 sensor via UART and I2C.
@copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
@license     The MIT License (MIT)
@author      [thdyyl](yuanlong.yu@dfrobot.com)
@version     V1.0
@date        2026-09-01
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
  print("Sensor  initialize success!!")


def loop():
  ##Obtain sensor data
  print("-----------------------\r\n")
  print("Temp: " + str(SEN050X.get_temperature(TEMP_C)) + " 'C\r\n")
  print("Temp: " + str(SEN050X.get_temperature(TEMP_F)) + " 'F\r\n")
  print("Humidity: " + str(SEN050X.get_humidity()) + " %\r\n")
  print("UV index: " + str(SEN050X.get_ultraviolet_intensity(ML8511)) + "\r\n")
  print("LuminousIntensity: " + str(SEN050X.get_luminousintensity()) + " lx\r\n")
  print("Atmospheric pressure: " + str(SEN050X.get_atmosphere_pressure(HPA)) + " hpa\r\n")
  print("Elevation: " + str(SEN050X.get_elevation()) + " m\r\n")
  print("-----------------------\r\n")
  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
