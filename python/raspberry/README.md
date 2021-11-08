DFRobot_EnvironmentalSensor
===========================

- [中文版](./README_CN.md)

数据手册上抄一下芯片的描述

这里写模块介绍，做到读完这段，就能对模块有初步了解，让客户懂的用这个模块能干什么（数据手册通常比较官方，这里你可以举例子，更场景化）<br>
这个模块的优点，告诉用户为什么要购买这个模块。一些关键术语，我们要在readme中有解释

这里需要显示拍照图片，可以一张图片，可以多张图片（不要用SVG图）

![产品效果图](../../resources/images/SEN0001.png)


## Product Link (https://www.dfrobot.com)

    SKU：产品名称

## Table of Contents

  * [summary](#summary)
  * [installation](#installation)
  * [methods](#methods)
  * [compatibility](#compatibility)
  * [history](#history)
  * [credits](#credits)

## Summary

这里填写当前Python软件库完成了基础功能，特色功能

## Installation

要使用这个库，首先将库下载到Raspberry Pi，然后打开例程文件夹。要执行一个例程demox.py，请在命令行中输入python demox.py。例如，要执行control_led.py例程，你需要输入:

```python
python control_led.py
```

## Methods

```python
  '''!
    @brief 初始化SEN050X传感器
    @brief 初始化SEN050X传感器
    @return 返回值初始化状态
    @retval 0  成功
    @retval -1 失败
  '''
  def begin(self):

  '''!
    @brief 获取SEN050X温度数据
    @param units 温度数据单位选择
    @n     TEMP_C 摄氏度
    @n     TEMP_F 华氏度 
    @return 返回获取的温度数据
  '''
  def get_temperature(self,unist):

  '''!
    @brief 获取SEN050X湿度数据 
    @return 返回获取的湿度数据
  '''
  def get_humidity(self):

  '''!
    @brief 获取SEN050X紫外线强度指数数据 
    @return 返回获取的紫外线强度指数数据
  '''
  def get_ultraviolet_intensity(self):

  '''!
    @brief 获取SEN050X光线强度数据 
    @return 返回获取的光线强度数据
  '''
  def get_luminousintensity(self):

  '''!
    @brief 获取SEN050X大气压强数据 
    @param units 大气压强数据单位选择
    @n            HPA 百帕
    @n            KPA 千帕
    @return 返回获取的大气压强数据
  '''
  def get_atmosphere_pressure(self, units):

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
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |       √   |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## History

- 2021-08-31 - Version 1.0.0 released.

## Credits

Written by TangJie(jie.tang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))