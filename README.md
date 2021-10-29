DFRobot_EnvironmentalSensor
===========================

* [中文版](./README_CN.md)

SEN050X 是一个气象传感器，模块个上集成了，紫外线传感器、光照传感器、大气压强传感器、温湿度传感器，模块有Gravity接口和Breakout两种接口方式，通过串口或I2C进行数据传输，通过模块上的切换开关进行通信方式选择。
DFRobot_Environmental_Sensor库是DFRobot专门为该传感器写的一个Arduino驱动库，用户可以用此驱动库来驱动Ardunio主控连接的传感器获取传感器数据。

![正反面svg效果图](https://www.dfrobot.co)

## Product Link（链接到英文商城）
  在运行此库的demo之前，你需要下载关联库: https://github.com/DFRobot/DFRobot_RTU
  
## Table of Contents

- [DFRobot_EnvironmentalSensor](#dfrobot_environmentalsensor)
  - [Product Link（链接到英文商城）](#product-link链接到英文商城)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
  - [Methods](#methods)
  - [Compatibility](#compatibility)
  - [History](#history)
  - [Credits](#credits)

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
/**
   * @fn DFRobot_EnvironmentalSensor
   * @brief DFRobot_Multifunctional_Environmental_Sensor构造函数。
   * @param pWire 指向TowWire流的I2C指针,此种传递方式需要在demo中调用begin初始化ArduinoI2C配置。
   * @param addr  SEN0501设备I2C通信的I2C地址。
   */
  DFRobot_EnvironmentalSensor(uint8_t addr, TwoWire *pWire = &Wire);

  /**
   * @fnDFRobot_EnvironmentalSensor
   * @brief DFRobot_Multifunctional_Environmental_Sensor构造函数。
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
   * @n            HPA:百帕
   * @n            KPA:千帕
   * @return 返回获取的大气压强数据
   */
  uint16_t getAtmospherePressure(uint8_t units);

  /**
   * @fn getElevation
   * @brief 获取SEN050X海拔数据 
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

- 2021-08-31 - 1.0.0 版本

## Credits

Written by(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))