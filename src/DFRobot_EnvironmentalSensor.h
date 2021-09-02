/*
 MIT License

 Copyright (C) <2019> <@DFRobot Frank>

��Permission is hereby granted, free of charge, to any person obtaining a copy of this
��software and associated documentation files (the "Software"), to deal in the Software
��without restriction, including without limitation the rights to use, copy, modify,
��merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
��permit persons to whom the Software is furnished to do so.

��The above copyright notice and this permission notice shall be included in all copies or
��substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DFROBOT_MULTIFUNCTIONAL_ENVIRONMENTAL_SENSOR_H
#define DFROBOT_MULTIFUNCTIONAL_ENVIRONMENTAL_SENSOR_H

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_RTU.h"
#include "String.h"
#include "DFRobot_RTU.h"

#if (!defined ARDUINO_ESP32_DEV) && (!defined __SAMD21G18A__)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

#define FACILITY_ID          0X20  //设备ID

#if 0
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#ifndef RTU_BROADCAST_ADDRESS
#define RTU_BROADCAST_ADDRESS                      0x00 /**<modbus协议的广播地址为0x00*/
#endif
#define SEN050X_DEFAULT_DEVICE_ADDRESS             0x22 /**<SEN0501传感器的默认设备地址为0x20*/

class DFRobot_EnvironmentalSensor: public DFRobot_RTU{


public:
#define DEVICE_PID_GRAVITY   0x01F5
#define DEVICE_PID_BREAKOUT  0x01F4
#define DEVICE_VID           0x3343

//大气压强单位选择
#define HPA                  0x01
#define KPA                  0X02

//温度单位选择
#define TEMP_C                    0X03
#define TEMP_F                    0X04



/**
 * @brief DFRobot_Multifunctional_Environmental_Sensor构造函数。
 * 
 * @param pWire 指向TowWire流的I2C指针,此种传递方式需要在demo中调用begin初始化ArduinoI2C配置。
 * @param addr  SEN0501设备I2C通信的I2C地址。
 */
  DFRobot_EnvironmentalSensor(uint8_t addr, TwoWire *pWire = &Wire);
/**
 * @brief DFRobot_Multifunctional_Environmental_Sensor构造函数。
 * @param addr: 主机要和SEN0501从机设备通信的设备地址
 * @n     SEN0501_DEFAULT_DEVICE_ADDRESS or 32（0x20）: SEN0501设备出厂默认设备地址，如果用户没有修改设备的地址，那么SEN0501的设备地址为32。
 * @param s   : 指向Stream流的串口指针，此种传递方式需要在demo中调用begin初始化Arduino主控的通信串口配置，需和SEN0501设备从机的串口配置一致
 * * @n SEN0501串口配置为：9600波特率，8位数据位，无校验位，1位停止位，参数无法修改。
 */
  DFRobot_EnvironmentalSensor(uint8_t addr, Stream *s);
  ~DFRobot_EnvironmentalSensor(){};
/**
 * @brief 初始化SEN050X传感器
 * 
 * @param pid 初始化传感器的PID，用于判断通信是否正确
 * @n     DEVICE_PID_GRAVITY  SEN0501传感器
 * @n     DEVICE_PID_BREAKOUT SEN0500传感器
 * @return 返回值;
 * @n      0:成功
 * @n      -1:失败
 */
uint8_t begin(uint16_t pid);
/**
 * @brief 获取设备PID(产品标识ID),注意此功能只有地址不为广播地址的类对象可以使用，
 * @n     广播地址类对象调用该方法无效。
 * @return PID:
 * @n      0:获取失败
 * @n      设备PID
 */
uint16_t getDevicePID();
/**
 * @brief 获取设备VID（厂商标识ID）,注意此功能只有地址不为广播地址的类对象可以使用，广播地址类对象调用该方法无效。
 * @return VID:
 * @n      0:获取失败
 * @n      DEVICE_VID or 0x3343：设备VID
 */
uint16_t getDeviceVID();
/**
 * @brief 获取SEN050X温度数据
 * 
 * @param units 返回数据单位选择
 * @n     TEMP_C:摄氏度
 * @n     TEMP_F:华氏度
 * 
 * @return 返回获取的温度数据
 */
float getTemperature(uint8_t unist);
/**
 * @brief 获取SEN050X湿度数据
 * 
 * @return 返回获取的湿度数据
 */
uint16_t getHumidity(void);
/**
 * @brief 获取SEN050X紫外线强度指数数据
 * 
 * @return 返回获取的紫外线强度指数数据
 */
float getUltravioletIntensity(void);
/**
 * @brief 获取SEN050X光线强度数据
 * 
 * @return 返回获取的光线强度数据
 */
uint16_t getLuminousIntensity(void);
/**
 * @brief 获取SEN050X大气压强数据
 * 
 * @param units 返回数据单位选择
 * @n            HPA:百帕
 * @n            KPA:千帕
 * @return 返回获取的大气压强数据
 */
uint16_t getAtmospherePressure(uint8_t units);
/**
 * @brief 获取SEN050X海拔数据
 * 
 * @return 返回获取的海拔数据
 */
uint16_t getElevation(void);


protected:
  uint8_t  readReg(uint16_t reg, void *pBuf, uint8_t size);
  bool detectDeviceAddress(uint8_t addr);
  
protected:
  TwoWire   *_pWire = NULL;
  Stream    *_s = NULL;
  uint8_t   _addr;

};

#endif
