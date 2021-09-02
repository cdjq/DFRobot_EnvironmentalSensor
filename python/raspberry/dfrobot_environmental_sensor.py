# -*- coding: utf-8 -*
'''
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      PengKaixing(jie.tang@dfrobot.com)
  * @version     V1.0
  * @date        2021-08-31
  * @get         from https://www.dfrobot.com
  * @url         https://www.dfrobot.co
'''

import serial
import time
import smbus
import os
import math
import RPi.GPIO as GPIO
import math
from DFRobot_RTU import *
I2C_MODE                  = 0x01
UART_MODE                 = 0x02
DEV_ADDRESS               = 0x22

DEVICE_VID                = 0x3343
DEVICE_ADDRESS            = 0x22

'''大气压强单位选择'''
HPA                       = 0x01
KPA                       = 0X02

'''温度单位选择'''
TEMP_C                    = 0X03
TEMP_F                    = 0X04



class dfrobot_environmental_sensor(DFRobot_RTU):
    
  def __init__(self ,bus ,Baud):
    if bus != 0:
      self.i2cbus = smbus.SMBus(bus)
      self.__uart_i2c = I2C_MODE
    else:
      super(dfrobot_environmental_sensor, self).__init__(Baud, 8, 'N', 1)
      #self.ser = serial.Serial("/dev/ttyAMA0" ,baudrate=Baud,stopbits=1, timeout=0.5)
      self.__uart_i2c = UART_MODE
      if self.ser.isOpen == False:
        self.ser.open() 
  def test(self):
    rbuf = self.read_reg(0x00,2)
    print(rbuf)
    time.sleep(1) 

  '''
  *   @brief 获取设备PID(产品标识ID),注意此功能只有地址不为广播地址的类对象可以使用,
  *   @n     广播地址类对象调用该方法无效
  *   @return PID:
  *   @n     0:获取失败
  *   @n     设备PID
  '''
  def get_device_pid(self):
    rbuf = self.read_reg(0x00, 2)
    return rbuf[0] << 8 | rbuf[1]

  '''
  *   @brief 获取设备VID（厂商标识ID）,注意此功能只能有地址不为广播地址的类对象可以使用，
  *   @n     广播地址类对象调用该方法无效
  *   @return VID:
  *   @n     0:获取失败
  *   @n     DEVICE_VID or 0x3344:设备VID
  '''
  def get_device_vid(self):
    rbuf = self.read_reg(0x02, 2)
    return rbuf[0] << 8 | rbuf[1]
  
  '''
  *   @brief 判断地址是否正确
  *
  *   @return  true:地址匹配成功 false:地址匹配失败
  '''
  def detect_device_address(self):
    rbuf = self.read_reg(0x04,2)
    return (rbuf[0] << 8 | rbuf[1])

  '''
  *   @brief 初始化SEN050X传感器
  *
  *   @param pid 初始化传感器的PID, 用于判断通行是否正确
  *   @n     DEVICE_PID_GRAVITY    SEN0501传感器
  *   @n     DEVICE_PID_BREAKOUT   SEN0500传感器
  * 
  *   @return 返回值；
  *           0:成功
  *           -1:失败
  '''
  def begin(self,pid):
    if self.detect_device_address() != DEV_ADDRESS:
      return False
    if self.get_device_pid() != pid:
      return False
    if self.get_device_vid() != DEVICE_VID:
      return False
    return True

  '''
  *  @brief 获取SEN050X温度数据
  *  @param unist 返回数据单位选择
  *  @n     TEMP_C:摄氏度
  *  @n     TEMP_F:华氏度
  *  @return 返回获取的温度数据
  '''
  def get_temperature(self,unist):
    rbuf = self.read_reg(0x14, 2)
    data = rbuf[0] << 8 | rbuf[1]
    if data >= 4500:
      data -= 4500
      temp = data / 100 + (data % 100) * 0.01
    else:
      data -= 4500
      temp = data /100 + (data % 100) * 0.01
    if unist == TEMP_F:
        temp = (temp * 1.8) + 32
    return round(temp,2)
    
  '''
  *  @brief 获取SEN050X湿度数据
  *  @param NULL
  *  @return 返回获取的湿度数据
  '''
  def get_humidity(self):
    rbuf = self.read_reg(0x16, 2)
    humidity = rbuf[0] << 8 | rbuf[1]
    return humidity

  '''
  *   @brief 获取SEN050X紫外线强度指数数据
  *   @param NULL
  *   @return 返回获取的紫外线强度指数数据
  '''
  def get_ultraviolet_intensity(self):
    rbuf = self.read_reg(0x10, 2)
    data = rbuf[0] << 8 | rbuf[1]
    ultraviolet = (data / 100) + (data % 100) * 0.01
    return ultraviolet

  '''
  *   @brief 获取SEN050X光线强度数据
  *   @paran NULL
  *   @return 返回获取的光线强度数据
  '''
  def get_luminousintensity(self):
    rbuf = self.read_reg(0x12 ,2)
    luminous = rbuf[0] << 8 | rbuf[1]
    return luminous

  '''
  *   @brief 获取SEN050X大气压强数据
  *   @paran units 返回数据单位选择
  *   @n     HPA:百帕
  *   @n     KPA:千帕
  *   @return 返回获取的大气压强数据
  '''
  def get_atmosphere_pressure(self, units):
    rbuf = self.read_reg(0x18, 2)
    atmosphere = rbuf[0] << 8 | rbuf[1]
    if units == KPA:
      atmosphere /= 10
    return atmosphere

  '''
  *   @brief 获取SEN050X海拔数据
  *   @paran NULL
  *   @return 返回获取的海拔数据
  '''
  def get_elevation(self):
    rbuf = self.read_reg(0x1A, 2)
    elevation = rbuf[0] << 8 | rbuf[1]
    return elevation

        
'''
  @brief An example of an i2c interface module
'''
class dfrobot_environmental_sensor_i2c(dfrobot_environmental_sensor):
  def __init__(self ,bus ,addr):
    self.__addr = addr
    super(dfrobot_environmental_sensor_i2c, self).__init__(bus,0)     
    
  '''
    @brief read the data from the register
    @param reg register address
    @param value read data
  '''
  def read_reg(self, reg_addr ,length):
    try:
      rslt = self.i2cbus.read_i2c_block_data(self.__addr ,reg_addr , length)
    except:
      rslt = -1
    return rslt    

'''
  @brief An example of an UART interface module
'''
class dfrobot_environmental_sensor_uart(dfrobot_environmental_sensor):
  SERIAL_DATA_BUF_MAX_SIZE = 20
  RTU_READ_REG_CMD = 0x03
  RTU_WRITE_REG_CMD = 0x06
  RTU_WRITE_MULTIPLE_REG_CMD = 0x10
  def __init__(self ,Baud):
    self.__Baud = Baud
    try:
      super(dfrobot_environmental_sensor_uart, self).__init__(Baud, 8, 'N', 1)
      #super(dfrobot_blood_oxygen_s_uart, self).__init__(0,Baud)
    except:
      print ("plese get root!")
  
   
  '''
    从传感器读出数据
  '''
  def read_reg(self, reg_addr ,length):
    return self.read_input_registers(0x22,int(math.ceil(reg_addr/2)),int(math.ceil(length/2)))[1:]
       