/*!
 * @file  DFRobot_Sensor.h
 * @brief DFRobot_EnvironmentalSensor 类的基础结构
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license   The MIT License (MIT)
 * @author    [TangJie](jie.tang@dfrobot.com)
 * @version   V1.0
 * @date      2021-08-31
 * @url       https://github.com/DFRobot/DFRobot_EnvironmentalSensor
 */

#ifndef DFROBOT_MULTIFUNCTIONAL_ENVIRONMENTAL_SENSOR_H
#define DFROBOT_MULTIFUNCTIONAL_ENVIRONMENTAL_SENSOR_H

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_RTU.h"
#include "String.h"

#if (!defined ARDUINO_ESP32_DEV) && (!defined __SAMD21G18A__)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif


#if 1
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#ifndef RTU_BROADCAST_ADDRESS
#define RTU_BROADCAST_ADDRESS                      0x00 ///<modbus协议的广播地址为0x00
#endif
#define SEN050X_DEFAULT_DEVICE_ADDRESS             0x22 ///SEN050X传感器的默认设备地址为0x22

class DFRobot_EnvironmentalSensor: public DFRobot_RTU{

public:
#define DEVICE_PID_GRAVITY   0x01F5
#define DEVICE_PID_BREAKOUT  0x01F4
#define DEVICE_VID           0x3343

#define HPA                  0x01 ///<大气压强百帕
#define KPA                  0X02 ///<大气压强千帕

#define TEMP_C                    0X03  ///<摄氏度
#define TEMP_F                    0X04  ///<华氏度

#define REG_PID                   0x0000 ///<协议转换板的寄存器
#define REG_VID                   0x0001 ///<协议转换板的寄存器
#define REG_DEVICE_ADDR           0x0002 ///<协议转换板的寄存器
#define REG_UART_CTRL0            0x0003 ///<协议转换板的寄存器
#define EG_UART_CTRL1             0x0004 ///<协议转换板的寄存器
#define REG_VERSION               0x0005 ///<协议转换板的寄存器

#define REG_ULTRAVIOLET_INTENSITY 0x0008 ///<协议转换板的寄存器
#define REG_LUMINOUS_INTENSITY    0x0009 ///<协议转换板的寄存器
#define REG_TEMP                  0x000A ///<协议转换板的寄存器
#define REG_HUMIDITY              0x000B ///<协议转换板的寄存器
#define REG_ATMOSPHERIC_PRESSURE  0x000C ///<协议转换板的寄存器
#define REG_ELEVATION             0x000D ///<协议转换板的寄存器



  /**
   * @fn DFRobot_EnvironmentalSensor
   * @brief DFRobot_EnvironmentalSensor构造函数。
   * @param pWire 指向TowWire流的I2C指针,此种传递方式需要在demo中调用begin初始化ArduinoI2C配置。
   * @param addr  SEN0501设备I2C通信的I2C地址。
   */
  DFRobot_EnvironmentalSensor(uint8_t addr, TwoWire *pWire = &Wire);

  /**
   * @fn DFRobot_EnvironmentalSensor
   * @brief DFRobot_EnvironmentalSensor构造函数。
   * @param addr: 主机要和SEN0501从机设备通信的设备地址
   * @n     SEN0501_DEFAULT_DEVICE_ADDRESS or 32（0x20）: SEN0501设备出厂默认设备地址，如果用户没有修改设备的地址，那么SEN0501的设备地址为32。
   * @param s   : 指向Stream流的串口指针，此种传递方式需要在demo中调用begin初始化Arduino主控的通信串口配置，需和SEN0501设备从机的串口配置一致
   * @n SEN0501串口配置为：9600波特率，8位数据位，无校验位，1位停止位，参数无法修改。
   */
  DFRobot_EnvironmentalSensor(uint8_t addr, Stream *s);
  ~DFRobot_EnvironmentalSensor(){};

  /**
   * @fn begin
   * @brief 初始化SEN050X传感器
   * @return 返回值初始化状态
   * @retval 0  成功
   * @retval -1 失败
   */
  int8_t begin(void);

  /**
   * @fn getTemperature
   * @brief 获取SEN050X温度数据
   * @param units 温度数据单位选择
   * @n     TEMP_C 摄氏度
   * @n     TEMP_F 华氏度 
   * @return 返回获取的温度数据
   */
  float getTemperature(uint8_t unist);

  /**
   * @fn getHumidity
   * @brief 获取SEN050X湿度数据 
   * @return 返回获取的湿度数据
   */
  float getHumidity(void);

  /**
   * @fn getUltravioletIntensity
   * @brief 获取SEN050X紫外线强度指数数据 
   * @return 返回获取的紫外线强度指数数据
   */
  float getUltravioletIntensity(void);

  /**
   * @fn getLuminousIntensity
   * @brief 获取SEN050X光线强度数据 
   * @return 返回获取的光线强度数据
   */
  float getLuminousIntensity(void);

  /**
   * @fn getAtmospherePressure
   * @brief 获取SEN050X大气压强数据 
   * @param units 大气压强数据单位选择
   * @n            HPA 百帕
   * @n            KPA 千帕
   * @return 返回获取的大气压强数据
   */
  uint16_t getAtmospherePressure(uint8_t units);

  /**
   * @fn getElevation
   * @brief 获取SEN050X海拔数据 
   * @return 返回获取的海拔数据
   */
  float getElevation(void);


protected:
  uint16_t getDevicePID();
  float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
  uint16_t getDeviceVID();
  uint8_t  readReg(uint16_t reg, void *pBuf, uint8_t size);
  bool detectDeviceAddress(uint8_t addr);
  
protected:
  TwoWire   *_pWire = NULL;
  Stream    *_s = NULL;
  uint8_t   _addr;

};

#endif
