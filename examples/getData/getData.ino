/*!
 *@file  getData.ino
 *@brief This example reads all data from the SEN0501/SEN0500 sensor via UART or I2C.
 *@n     It works with every hardware version (V1.0 / V2.0 / V3.0). Set SENSOR_VERSION below
 *@n     to the version printed on the front silkscreen of your board, e.g.
 *@n     "Multifunctional Environmental Sensor V3.0".
 *@n
 *@n     Version differences: temperature, humidity, light, pressure and altitude are identical
 *@n     on all versions. Only the UV sensor and its output differ:
 *@n       V1.0  ML8511      -> UV index (0 to 15, unitless)
 *@n       V2.0  LTR390      -> equivalent UVA irradiance, mW/cm2
 *@n       V3.0  GUVA-S12SD  -> equivalent UVA irradiance, mW/cm2
 *@n     A wrong SENSOR_VERSION does not report an error, it only gives a wrong UV value.
 * ---------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  | 25/D2 |     X      |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  | 26/D3 |     X      |
 * ---------------------------------------------------------------------------------------------------------------
 *
 * @copyright   Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      [thdyyl](yuanlong.yu@dfrobot.com)
 * @version     V1.3.0
 * @date        2026-10-26
 * @url         https://github.com/DFRobot/DFRobot_EnvironmentalSensor
 */
#include "DFRobot_EnvironmentalSensor.h"
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include <SoftwareSerial.h>
#endif

/**
 * Hardware version of your board, see the silkscreen on the front side.
 *   eSensorV1_0  ->  "... Sensor V1.0"
 *   eSensorV2_0  ->  "... Sensor V2.0"
 *   eSensorV3_0  ->  "... Sensor V3.0"
 */
#define SENSOR_VERSION eSensorV3_0

#define MODESWITCH /*UART:*/ 1 /*I2C: 0*/

#if MODESWITCH
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
SoftwareSerial              mySerial(/*rx =*/4, /*tx =*/5);
DFRobot_EnvironmentalSensor environment(/*addr =*/SEN050X_DEFAULT_DEVICE_ADDRESS, /*s =*/&mySerial);
#else
DFRobot_EnvironmentalSensor environment(/*addr =*/SEN050X_DEFAULT_DEVICE_ADDRESS, /*s =*/&Serial1);
#endif
#else
DFRobot_EnvironmentalSensor environment(/*addr = */ SEN050X_DEFAULT_DEVICE_ADDRESS, /*pWire = */ &Wire);
#endif

void setup()
{
#if MODESWITCH
  //Init MCU communication serial port
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  mySerial.begin(9600);
#elif defined(ESP32)
  Serial1.begin(9600, SERIAL_8N1, /*rx =*/26 /*D3*/, /*tx =*/25 /*D2*/);
#else
  Serial1.begin(9600);
#endif
#endif
  Serial.begin(115200);
  while (environment.begin() != 0) {
    Serial.println(" Sensor initialize failed!!");
    delay(1000);
  }
  Serial.println(" Sensor initialize success!!");
}

void loop()
{
  //Print the data obtained from sensor
  Serial.println("-------------------------------");
  Serial.print("Temp: ");
  Serial.print(environment.getTemperature(TEMP_C));
  Serial.println(" ℃");
  Serial.print("Temp: ");
  Serial.print(environment.getTemperature(TEMP_F));
  Serial.println(" ℉");
  Serial.print("Humidity: ");
  Serial.print(environment.getHumidity());
  Serial.println(" %");
  //V1.0 returns a UV index, V2.0 and V3.0 return irradiance in mW/cm2
  if (SENSOR_VERSION == eSensorV1_0) {
    Serial.print("UV index: ");
    Serial.println(environment.getUltravioletIntensity(SENSOR_VERSION));
  } else {
    Serial.print("Ultraviolet intensity: ");
    Serial.print(environment.getUltravioletIntensity(SENSOR_VERSION));
    Serial.println(" mW/cm2");
  }
  Serial.print("LuminousIntensity: ");
  Serial.print(environment.getLuminousIntensity());
  Serial.println(" lx");
  Serial.print("Atmospheric pressure: ");
  Serial.print(environment.getAtmospherePressure(HPA));
  Serial.println(" hpa");
  Serial.print("Altitude: ");
  Serial.print(environment.getElevation());
  Serial.println(" m");
  Serial.println("-------------------------------");
  delay(500);
}
