# -*- coding: utf-8 -*
'''
  @copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence     The MIT License (MIT)
  @author      TangJie(jie.tang@dfrobot.com)
  @version     V1.0
  @date        2021-08-31
  @get         from https://www.dfrobot.com
  @url         https://github.com/cdjq/DFRobot_EnvironmentalSensor
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

I2C_MODE                  = 0x01
UART_MODE                 = 0x02
DEV_ADDRESS               = 0x22

DEVICE_VID                = 0x3343
DEVICE_ADDRESS            = 0x22

#大气压强单位选择
HPA                       = 0x01
KPA                       = 0X02

#温度单位选择
TEMP_C                    = 0X03
TEMP_F                    = 0X04

class DFRobot_Environmental_Sensor():
    
  def __init__(self ,bus ,baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self._uart_i2c = I2C_MODE
      
    else:
      self.master = modbus_rtu.RtuMaster(serial.Serial(port="/dev/ttyAMA0",baudrate=9600, bytesize=8, parity='N', stopbits=1))
      self.master.set_timeout(1.0)
      self._uart_i2c = UART_MODE
       
  
  '''
    @brief 判断地址是否正确
    @n
    @return  true:地址匹配成功 false:地址匹配失败
  '''
  def _detect_device_address(self):
    rbuf = self._read_reg(0x04,2)
    if self._uart_i2c == I2C_MODE:
      data = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      data = rbuf[0]
    return data

  '''
    @brief 初始化SEN050X传感�?
  
    @param pid 初始化传感器的PID, 用于判断通行是否正确
    @return 返回值；
            True:成功
            False:失败
  '''
  def begin(self):
    if self._detect_device_address() != DEV_ADDRESS:
      return False
    return True

  '''
    @brief 获取SEN050X温度数据
    @param unist 返回数据单位选择
    @n     TEMP_C:摄氏�?
    @n     TEMP_F:华氏�?
    @return 返回获取的温度数�?
  '''
  def get_temperature(self,unist):
    rbuf = self._read_reg(0x14, 2)
    if self._uart_i2c == I2C_MODE:
      data = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      data = rbuf[0]
    temp = (-45) +((data * 175.00) / 1024.00 / 64.00)
    if(unist == TEMP_F):
      temp = temp * 1.8 + 32 
    return round(temp,2)
    
  '''
    @brief 获取SEN050X湿度数据
    @param NULL
    @return 返回获取的湿度数�?
  '''
  def get_humidity(self):
    rbuf = self._read_reg(0x16, 2)
    if self._uart_i2c == I2C_MODE:
      humidity = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      humidity = rbuf[0]
    humidity = (humidity / 1024) * 100 / 64
    return humidity
  
  '''
    @brief 获取SEN050X紫外线强度指数数�?
    @param NULL
    @return 返回获取的紫外线强度指数数据
  '''
  def get_ultraviolet_intensity(self):
    rbuf = self._read_reg(0x10, 2)
    if self._uart_i2c == I2C_MODE:
      data = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      data = rbuf[0]
    outputVoltage = 3.0 * data/1024
    ultraviolet = (outputVoltage - 0.99) * (15.0 - 0.0) / (2.9 - 0.99) + 0.0 
    return round(ultraviolet,2)
      
  '''
    @brief 获取SEN050X光线强度数据
    @paran NULL 
    @return 返回获取的光线强度数�?
  '''
  def get_luminousintensity(self):
    rbuf = self._read_reg(0x12 ,2)
    if self._uart_i2c == I2C_MODE:
      data = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      data = rbuf[0]
    factor1 = 0.5
    factor2 = 0.0576
    result = data * factor1 * factor2
    return round(result,2)

  '''
    @brief 获取SEN050X大气压强数据
    @paran units 返回数据单位选择
    @n     HPA:百帕
    @n     KPA:千帕
    @return 返回获取的大气压强数�?
  '''
  def get_atmosphere_pressure(self, units):
    rbuf = self._read_reg(0x18, 2)
    if self._uart_i2c == I2C_MODE:
      atmosphere = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      atmosphere = rbuf[0]
    if units == KPA:
      atmosphere /= 10
    return atmosphere

  '''
    @brief 获取SEN050X海拔数据
    @paran NULL
    @return 返回获取的海拔数�?
  '''
  def get_elevation(self):
    rbuf = self._read_reg(0x18, 2)
    if self._uart_i2c == I2C_MODE:
      elevation = rbuf[0] << 8 | rbuf[1]
    elif self._uart_i2c == UART_MODE:
      elevation = rbuf[0]
    elevation = 44330 * (1.0 - pow(elevation / 1015.0, 0.1903));
    return round(elevation,2)

  
        
'''
  @brief An example of an i2c interface module
'''
class DFRobot_Environmental_Sensor_I2C(DFRobot_Environmental_Sensor):
  def __init__(self ,bus ,addr):
    self._addr = addr
    DFRobot_Environmental_Sensor.__init__(self,bus,0)   
    
  '''
    @brief read the data from the register
    @param reg register address
    @param value read data
  '''
  def _read_reg(self, reg_addr ,length):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self._addr ,reg_addr , length)
    except:
      rslt = -1
    return rslt    

'''
  @brief An example of an UART interface module
'''
class DFRobot_Environmental_Sensor_UART(DFRobot_Environmental_Sensor):
  
  def __init__(self ,baud, addr):
    self._baud = baud
    self._addr = addr
    try:
      DFRobot_Environmental_Sensor.__init__(self,0,self._baud)
    except:
      print ("plese get root!")
 
  
   
  '''
    @brief 从传感器读出数据
  '''
  def _read_reg(self, reg_addr ,length):
    return list(self.master.execute(self._addr, cst.READ_INPUT_REGISTERS, reg_addr/2, length/2))
    