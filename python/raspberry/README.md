DFRobot_EnvironmentalSensor
===========================

- [中文版](./README_CN.md)

This multifunctional environmental sensor, whose SKU number is SEN0500/SEN0501, integrates UV, illumination, atmospheric pressure and temperature and humidity sensor. It features Gravity and Breakout interfaces. Data can be transmitted through serial port or I2C and communication method can be selected by controlling the diverter switch on the module.
The DFRobot_Environmental_Sensor library is an Arduino-drive one specially designed for the sensor by DFRobot. Users can use it to drive the sensor that Ardunio main controller connects and obtain the sensor data.

![产品效果图](../../resources/images/SEN0500/SEN0501.png)

## Product Link (https://www.dfrobot.com)

    SKU：SEN0500/SEN0501

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

Download this library to Raspberry Pi before use, then open the routine folder. Type python demox.py on the command line to execute a routine demox.py. For example, to execute the control_led.py routine, you need to enter:

```python
python control_led.py
```

## Methods

```python
  '''!
    @brief Init SEN0500/SEN0501 sensor
    @brief Init SEN0500/SEN0501 sensor
    @return Return value init status
    @retval 0  Succeed
    @retval -1 Failed
  '''
  def begin(self):

  '''!
    @brief Get SEN0500/SEN0501 temperature data
    @param units Temperature data unit select
    @n     TEMP_C ℃
    @n     TEMP_F ℉ 
    @return Return the obtained temperature data
  '''
  def get_temperature(self,unist):

  '''!
    @brief Get SEN0500/SEN0501 humidity data 
    @return Return the obtained humidity data
  '''
  def get_humidity(self):

  '''!
    @brief Get SEN0500/SEN0501 UV intensity index data 
    @return Return the obtained UV intensity index data
  '''
  def get_ultraviolet_intensity(self):

  '''!
    @brief Get SEN0500/SEN0501 luminous intensity data 
    @return Return the obtained luminous intensity data
  '''
  def get_luminousintensity(self):

  '''!
    @brief Get SEN0500/SEN0501 atmosphere pressure data 
    @param units Atmosphere pressure data unit select
    @n            HPA Hectopascal
    @n            KPA Kilopascal
    @return Return the obtained atmosphere pressure data
  '''
  def get_atmosphere_pressure(self, units):

  '''!
    @brief Get SEN0500/SEN0501 elevation data 
    @return Return the obtained elevation data
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
