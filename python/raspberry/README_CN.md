DFRobot_EnvironmentalSensor
===========================

* [English Version](./README.md)

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

使用库，首先下载库文件，将其粘贴到指定的目录中，然后打开Examples文件夹并在该文件夹中运行演示。

## Methods

```python
'''!
    @brief 初始化SEN050X传感器
    @brief 初始化SEN050X传感器
    @return 返回值初始化状态
    @retval 0  成功
    @retval -1 失败
  '''
  def begin(self)

  '''!
    @brief 获取SEN050X温度数据
    @param units 温度数据单位选择
    @n     TEMP_C 摄氏度
    @n     TEMP_F 华氏度 
    @return 返回获取的温度数据
  '''
  def get_temperature(self,unist)

  '''!
    @brief 获取SEN050X湿度数据 
    @return 返回获取的湿度数据
  '''
  def get_humidity(self)

  '''!
    @brief 获取SEN050X紫外线强度指数数据 
    @return 返回获取的紫外线强度指数数据
  '''
  def get_ultraviolet_intensity(self)

  '''!
    @brief 获取SEN050X光线强度数据 
    @return 返回获取的光线强度数据
  '''
  def get_luminousintensity(self)

  '''!
    @brief 获取SEN050X大气压强数据 
    @param units 大气压强数据单位选择
    @n            HPA 百帕
    @n            KPA 千帕
    @return 返回获取的大气压强数据
  '''
  def get_atmosphere_pressure(self, units)

  '''!
    @brief 获取SEN050X海拔数据 
    @return 返回获取的海拔数据
  '''
  def get_elevation(self):
```

## Compatibility

* RaspberryPi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |     √     |            |          |         |
| RaspberryPi4 |           |            |     √    |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## History

- 2021-08-31 - 1.0.0 版本

## Credits

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))