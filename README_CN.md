DFRobot_EnvironmentalSensor
===========================

* [English Version](./README.md)

SEN050X 是一个气象传感器，模块个上集成了，紫外线传感器、光照传感器、大气压强传感器、温湿度传感器，模块有Gravity接口和Breakout两种接口方式，通过串口或I2C进行数据传输，通过模块上的切换开关进行通信方式选择。
DFRobot_Environmental_Sensor库是DFRobot专门为该传感器写的一个Arduino驱动库，用户可以用此驱动库来驱动Ardunio主控连接的传感器获取传感器数据。

![正反面svg效果图](https://www.dfrobot.co)

## Product Link（链接到英文商城）
  在运行此库的demo之前，你需要下载关联库: https://github.com/DFRobot/DFRobot_RTU
  
## Table of Contents

* [Installation](#installation)
* [Calibration](#calibration)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
/**
 * @brief DFRobot_Multifunctional_Environmental_Sensor构造函数。
 * 
 * @param pWire 指向TowWire流的I2C指针,此种传递方式需要在demo中调用begin初始化ArduinoI2C配置。
 * @param addr  SEN0501设备I2C通信的I2C地址。
 */
  DFRobot_Multifunctional_Environmental_Sensor(TwoWire *pWire = &Wire, uint8_t addr = SEN050X_DEFAULT_DEVICE_ADDRESS);
/**
 * @brief DFRobot_Multifunctional_Environmental_Sensor构造函数。
 * @param addr: 主机要和SEN0501从机设备通信的设备地址
 * @n     SEN0501_DEFAULT_DEVICE_ADDRESS or 32（0x20）: SEN0501设备出厂默认设备地址，如果用户没有修改设备的地址，那么SEN0501的设备地址为32。
 * @param s   : 指向Stream流的串口指针，此种传递方式需要在demo中调用begin初始化Arduino主控的通信串口配置，需和SEN0501设备从机的串口配置一致
 * * @n SEN0501串口配置为：9600波特率，8位数据位，无校验位，1位停止位，参数无法修改。
 */
  DFRobot_Multifunctional_Environmental_Sensor(uint8_t addr, Stream *s);
  ~DFRobot_Multifunctional_Environmental_Sensor(){};
/**
 * @brief 初始化SEN050X传感器
 * 
 * @param pid 初始化传感器的PID
 * @n     DEVICE_PID_GRAVITY 初始化SEN0501传感器
 * @n     DEVICE_PID_BREAKOUT 初始化SEN0500传感器
 * @return 返回值;
 * @n      0：成功
 * @n      -1:失败
 */
uint8_t begin(uint16_t pid);
/**
 * @brief 获取SEN0501温度数据
 * 
 * @return 返回获取的温度数据
 */
float getTemperature(uint8_t unist);
/**
 * @brief 获取SEN0501湿度数据
 * 
 * @return 返回获取的湿度数据
 */
uint16_t getHumidity(void);
/**
 * @brief 获取SEN0501紫外线强度指数数据
 * 
 * @return 返回获取的紫外线强度指数数据
 */
float getUltravioletIntensity(void);
/**
 * @brief 获取SEN0501光线强度数据
 * 
 * @return 返回获取的光线强度数据
 */
uint16_t getLuminousIntensity(void);
/**
 * @brief 获取SEN0501大气压强数据
 * 
 * @param units 返回数据单位选择
 * @n            HPA 百帕
 * @n            KPA 千帕
 * @return 返回获取的大气压强数据
 */
uint16_t getAtmospherePressure(uint8_t units);
/**
 * @brief 获取SEN0501海拔数据
 * 
 * @return 返回获取的海拔数据
 */
uint16_t getElevation(void);
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
FireBeetle M0      |      X       |      √       |      X        |
raspberry          |      X       |      √       |      √       |

## History

- Data 2021-08-31
- Version V1.0

## Credits

Written by(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))