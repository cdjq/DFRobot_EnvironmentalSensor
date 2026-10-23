DFRobot_EnvironmentalSensor
===========================

* [中文版](./README_CN.md)

The multifunctional environmental sensor(SEN0500/SEN0501) integrates the UV, illumination, atmospheric pressure and temperature and humidity detection functions into one. It features Gravity and Breakout interfaces, and supports UART and I2C data communication, which can be selected by the onboard switch. <br/>
This Arduino Library developed by DFRobot is specially designed for DFRobot Environmental sensor. Users can use the library to drive their sensor to obtain relevant environmental data conbining with an Arduino mainboard.

## V1.0
![Product image](./resources/images/SEN0501.png)
![Product image](./resources/images/SEN0500.png)

## V3.0
![Product image](./resources/images/V3.0/SEN0501.jpg)
![Product image](./resources/images/V3.0/SEN0500.jpg)

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

This multifunctional environmental sensor library can help obtain information like temperature, humidity, pressure, UV intensity, natural sunlight intensity and altitude.
The module also offers Gravity and breakout version for easy use.

Different hardware revisions use different UV sensors and conversions:

Hardware | UV sensor | API option | Return value |
-------- | --------- | ---------- | ------------ |
V1.0 | ML8511 | `eML8511` | Legacy analog-voltage UV index |
V2.0 | LTR390UV | `eLTR390UV` | Equivalent UVA irradiance in mW/cm² |
V3.0 | GUVA-S12SD | `eS12SD` | Equivalent 352nm UVA irradiance in mW/cm² |


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /**
   * @fn begin
   * @brief Init SEN0500/SEN0501 sensor
   * @return Return init status
   * @retval 0  Succeed
   * @retval -1 failed
   */
  int8_t begin(void);

  /**
   * @fn getTemperature
   * @brief Get SEN0500/SEN0501 temperature data
   * @param units Temperature data unit select
   * @n     TEMP_C ℃
   * @n     TEMP_F ℉
   * @return Return the obtained temperature data
   */
  float getTemperature(uint8_t unist);

  /**
   * @fn getHumidity
   * @brief Get SEN0500/SEN0501 humidity data
   * @return Return the obtained humidity data
   */
  float getHumidity(void);

  /**
   * @fn getUltravioletIntensity
   * @brief Get SEN0500/SEN0501 UV data
   * @param soc UV sensor and output type
   * @n eML8511: V1.0 ML8511 legacy UV index
   * @n eLTR390UV: V2.0 LTR390 equivalent UVA irradiance, mW/cm2
   * @n eS12SD: V3.0 GUVA-S12SD equivalent UVA irradiance, mW/cm2
   * @return Return the selected ultraviolet measurement
   */
  float getUltravioletIntensity(eUVSOC soc = eML8511);

  /**
   * @fn getLuminousIntensity
   * @brief Get SEN0500/SEN0501 luminous intensity data
   * @return Return the obtained luminous intensity data
   */
  float getLuminousIntensity(void);

  /**
   * @fn getAtmospherePressure
   * @brief Get SEN0500/SEN0501 atmosphere pressure data
   * @param units Atmosphere pressure data unit select
   * @n            HPA: Hectopascal
   * @n            KPA: Kilopascal
   * @return Return the obtained atmosphere pressure data
   */
  uint16_t getAtmospherePressure(uint8_t units);

  /**
   * @fn getElevation
   * @brief Get SEN0500/SEN0501 altitude data
   * @return Return the obtained altitude data
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
Raspberry Pi       |      X       |      √       |      √       |

## History

- 2026-10-22 - Version 1.3.0 reports V2.0 LTR390 and V3.0 GUVA-S12SD ultraviolet data in mW/cm².
- 2021-08-31 - Version 1.0.0 released.
- 2021-01-18 - Version 1.0.1 released.

## Credits

Written by thdyyl(yuanlong.yu@dfrobot.com), 2026. (Welcome to our [website](https://www.dfrobot.com/))
