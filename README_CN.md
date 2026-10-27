DFRobot_EnvironmentalSensor
===========================

* [English Version](./README.md)

SEN0500/SEN0501 是一个多气象传感器，模块上集成了，紫外线传感器、光照传感器、大气压强传感器、温湿度传感器，模块有Gravity接口和Breakout两种接口方式，通过串口或I2C进行数据传输，通过模块上的切换开关进行通信方式选择。
DFRobot_Environmental_Sensor库是DFRobot专门为该传感器写的一个Arduino驱动库，用户可以用此驱动库来驱动Ardunio主控连接的传感器获取传感器数据。

## V1.0
![产品效果图片](./resources/images/SEN0501.png)
![产品效果图片](./resources/images/SEN0500.png)

## V3.0
![产品效果图片](./resources/images/V3.0/SEN0501.jpg)
![产品效果图片](./resources/images/V3.0/SEN0500.jpg)

## 产品链接（https://www.dfrobot.com）

    SKU：SEN0500/SEN0501

## 目录

  * [概述](#概述)
  * [库安装](#库安装)
  * [方法](#方法)
  * [兼容性](#兼容性)
  * [历史](#历史)
  * [创作者](#创作者)

## 概述

这是一个多气象传感器得库，使用该库能够很便捷得获取温度、湿度、压强、紫外线强度、自然光线强度和海拔等信息，
模块还提供了Gravity版本和breakout版本方便使用。

### 硬件版本

硬件版本印在模块正面丝印上，例如 "Multifunctional Environmental Sensor V3.0"。把它传给 `getUltravioletIntensity()` 即可；示例 `getData.ino` 中通过 `SENSOR_VERSION` 宏设置。

温度、湿度、光照、气压和海拔在所有版本上的读取方式完全一致，只有紫外线传感器及其读数含义不同：

硬件版本（丝印） | API参数 | 紫外线传感器 | 寄存器原始值 | `getUltravioletIntensity()` 返回值 |
---------------- | ------- | ------------ | ------------ | ---------------------------------- |
V1.0（`V1.0`） | `eSensorV1_0` | ML8511，模拟输出 | 传感器电压的10位ADC值 | UV指数，0~15，无单位 |
V2.0（`V2.0`）   | `eSensorV2_0` | LTR390，数字输出 | 传感器计数值（20位，6倍增益） | 等效UVA辐照度，mW/cm² |
V3.0（`V3.0`）   | `eSensorV3_0` | GUVA-S12SD，模拟输出 | 放大器输出的10位ADC值 | 352nm UVA灯条件下的等效辐照度，mW/cm² |

版本选错不会报错，只会得到错误的紫外线数值。旧的按芯片命名的参数 `eML8511`、`eLTR390UV`、`eS12SD` 仍然可用，分别等同于 `eSensorV1_0`、`eSensorV2_0`、`eSensorV3_0`。

## 库安装

使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## 方法

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
   * @brief 获取SEN0500/SEN0501紫外线数据
   * @param version 模块丝印上的硬件版本
   * @n eSensorV1_0：V1.0 模块（ML8511），返回UV指数，0~15，无单位
   * @n eSensorV2_0：V2.0 模块（LTR390），返回等效UVA辐照度，单位mW/cm2
   * @n eSensorV3_0：V3.0 模块（GUVA-S12SD），返回等效UVA辐照度，单位mW/cm2
   * @n 默认为 eSensorV1_0，与旧版本库行为保持一致
   * @return 返回所选硬件版本对应的紫外线测量结果
   */
  float getUltravioletIntensity(eUVSOC version = eSensorV1_0);

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


## 兼容性

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

## 历史
- 2026-10-26 - 1.3.0版本，V2.0 LTR390与V3.0 GUVA-S12SD紫外线数据统一为mW/cm²，紫外线换算改为按硬件版本选择（`eSensorV1_0` / `eSensorV2_0` / `eSensorV3_0`），示例合并为 `getData.ino`，补充 V1.0 与 V2.0/V3.0 的差异说明。
- 2021-08-31 - 1.0.0 版本
- 2022-01-18 - 1.0.1 版本

## 创作者

Written by thdyyl(yuanlong.yu@dfrobot.com), 2026. (Welcome to our [website](https://www.dfrobot.com/))
