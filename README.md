DFRobot_EnvironmentalSensor
===========================

* [中文版](./README_CN.md)

SEN0500/SEN0501 是一个多气象传感器，模块个上集成了，紫外线传感器、光照传感器、大气压强传感器、温湿度传感器，模块有Gravity接口和Breakout两种接口方式，通过串口或I2C进行数据传输，通过模块上的切换开关进行通信方式选择。
DFRobot_Environmental_Sensor库是DFRobot专门为该传感器写的一个Arduino驱动库，用户可以用此驱动库来驱动Ardunio主控连接的传感器获取传感器数据。

![产品效果图片](../../resources/images/SEN0500/SEN0501.png)
  
## Product Link (https://www.dfrobot.com)
    SKU: SEN0500/SEN0501

## Table of Contents

  * [Summary](#summary)
  * [Installation](#installation)
  * [Methods](#methods)
  * [Compatibility](#compatibility)
  * [History](#history)
  * [Credits](#credits)

## Summary

这是一个多气象传感器得库，使用该库能够很便捷得获取温度、湿度、压强、紫外线强度、自然光线强度和海拔等信息，
模块还提供了Gravity版本和breakout版本方便使用。


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @fn begin
   * @brief 初始化SEN0500/SEN0501传感器
   * @return 返回值初始化状态
   * @retval 0  成功
   * @retval -1 失败
   */
  int8_t begin(void);

  /**
   * @fn getTemperature
   * @brief 获取SEN0500/SEN0501温度数据
   * @param units 温度数据单位选择
   * @n     TEMP_C 摄氏度
   * @n     TEMP_F 华氏度 
   * @return 返回获取的温度数据
   */
  float getTemperature(uint8_t unist);

  /**
   * @fn getHumidity
   * @brief 获取SEN0500/SEN0501湿度数据 
   * @return 返回获取的湿度数据
   */
  float getHumidity(void);

  /**
   * @fn getUltravioletIntensity
   * @brief 获取SEN0500/SEN0501紫外线强度指数数据 
   * @return 返回获取的紫外线强度指数数据
   */
  float getUltravioletIntensity(void);

  /**
   * @fn getLuminousIntensity
   * @brief 获取SEN0500/SEN0501光线强度数据 
   * @return 返回获取的光线强度数据
   */
  float getLuminousIntensity(void);

  /**
   * @fn getAtmospherePressure
   * @brief 获取SEN0500/SEN0501大气压强数据 
   * @param units 大气压强数据单位选择
   * @n            HPA:百帕
   * @n            KPA:千帕
   * @return 返回获取的大气压强数据
   */
  uint16_t getAtmospherePressure(uint8_t units);

  /**
   * @fn getElevation
   * @brief 获取SEN0500/SEN0501海拔数据 
   * @return 返回获取的海拔数据
   */
  float getElevation(void);
```

## Compatibility

MCU                | SoftwareSerial | HardwareSerial |      IIC      |
------------------ | :----------: | :----------: | :----------: | 
Arduino Uno        |      √       |      X       |      √       |
Mega2560           |      √       |      √       |      √       |
Leonardo           |      √       |      √       |      √       |
ESP32              |      X       |      √       |      √       |
ESP8266            |      √       |      X       |      √       |
micro:bit          |      X       |      X       |      √       |
FireBeetle M0      |      X       |      √       |      √       |
raspberry          |      X       |      √       |      √       |

## History

- 2021-08-31 - Version 1.0.0 released.

## Credits

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))