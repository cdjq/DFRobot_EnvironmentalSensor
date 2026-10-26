DFRobot_EnvironmentalSensor
===========================

- [中文版](./README_CN.md)

The multifunctional environmental sensor(SEN0500/SEN0501) integrates the UV, illumination, atmospheric pressure and temperature and humidity detection functions into one. It features Gravity and Breakout interfaces, and supports UART and I2C data communication, which can be selected by the onboard switch.
This Arduino Library developed by DFRobot is specially designed for DFRobot Environmental sensor. Users can use the library to drive their sensor to obtain relevant environmental data conbining with an Arduino mainboard.

## V1.0

![Product image](../../resources/images/SEN0501.png)
![Product image](../../resources/images/SEN0500.png)

## V3.0

![Product image](../../resources/images/V3.0/SEN0501.jpg)
![Product image](../../resources/images/V3.0/SEN0500.jpg)

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

This multifunctional environmental sensor library can help obtain information like temperature, humidity, pressure, UV intensity, natural sunlight intensity and altitude.
The module also offers Gravity and breakout version for easy use.

### Hardware versions

The hardware version is printed on the front silkscreen of the board, e.g. "Multifunctional Environmental Sensor V3.0". Pass it to `get_ultraviolet_intensity()`; the example `get_data.py` exposes it as `SENSOR_VERSION`.

Temperature, humidity, light, pressure and altitude are read the same way on every version. Only the UV sensor and the meaning of its reading differ:

Hardware (silkscreen) | API option | UV sensor | Raw register | `get_ultraviolet_intensity()` returns |
--------------------- | ---------- | --------- | ------------ | ------------------------------------- |
V1.0 (`V1.0`) | `SENSOR_V1_0` | ML8511, analog | 10-bit ADC of sensor voltage | UV index, 0 to 15, unitless |
V2.0 (`V2.0`)   | `SENSOR_V2_0` | LTR390, digital | Sensor count (20-bit, 6x gain) | Equivalent UVA irradiance, mW/cm² |
V3.0 (`V3.0`)   | `SENSOR_V3_0` | GUVA-S12SD, analog | 10-bit ADC of amplifier output | Equivalent 352 nm UVA irradiance, mW/cm² |

Selecting the wrong version does not produce an error, only a wrong UV value. The legacy chip-named constants `ML8511`, `LTR390UV` and `S12SD` are still accepted and map to `SENSOR_V1_0`, `SENSOR_V2_0` and `SENSOR_V3_0`.

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
    @return Return init status
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
    @brief Get SEN0500/SEN0501 UV data
    @param version Hardware version printed on the board silkscreen
    @n SENSOR_V1_0: V1.0 board (ML8511), returns UV index, 0 to 15, unitless
    @n SENSOR_V2_0: V2.0 board (LTR390), returns equivalent UVA irradiance, mW/cm2
    @n SENSOR_V3_0: V3.0 board (GUVA-S12SD), returns equivalent UVA irradiance, mW/cm2
    @return Return the ultraviolet measurement for the selected hardware version
  '''
  def get_ultraviolet_intensity(self, version=SENSOR_V1_0):

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
    @brief Get SEN0500/SEN0501 altitude data
    @return Return the obtained altitude data
  '''
  def get_elevation(self):
```

## Compatibility

* RaspberryPi Version

| Board         | Work Well | Work Wrong | Untested | Remarks |
| ------------- | :-------: | :--------: | :------: | ------- |
| Raspberry Pi2 |          |            |    √    |         |
| Raspberry Pi3 |          |            |    √    |         |
| Raspberry Pi4 |    √    |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |          |            |    √    |         |
| Python3 |    √    |            |          |         |

## History

- 2026-10-26 - Version 1.4.0 selects the UV conversion by hardware version (`SENSOR_V1_0` / `SENSOR_V2_0` / `SENSOR_V3_0`) instead of UV chip name, merges the three examples into `get_data.py`, and documents the V1.0 vs V2.0/V3.0 differences.
- 2026-10-22 - Version 1.3.0 reports V2.0 LTR390 and V3.0 GUVA-S12SD ultraviolet data in mW/cm².
- 2021-08-31 - Version 1.0.0 released.

## Credits

Written by thdyyl(yuanlong.yu@dfrobot.com), 2026. (Welcome to our [website](https://www.dfrobot.com/))
