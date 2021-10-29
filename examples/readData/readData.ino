/*!
 *@file  read_data.ino
 *@brief 这个demo演示获取SEN050X传感器上的数据，通过IIC或串口连接传感器
 *@n 将SEN050X返回的数据打印在串口监视器上
 *
 * @n connected table
 * ---------------------------------------------------------------------------------------------------------------
 *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |
 *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |
 *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |
 *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |
 *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |
 * ---------------------------------------------------------------------------------------------------------------
 * 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author      [TangJie](jie.tang@dfrobot.com)
 * @version     V1.0
 * @date        2021-08-31
 * @url         https://github.com/DFRobot/DFRobot_EnvironmentalSensor
 */
#include "DFRobot_EnvironmentalSensor.h"
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
#include <SoftwareSerial.h>
#endif

#define MODESWITCH        /*UART:*/1 /*I2C: 0*/

#if MODESWITCH
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  SoftwareSerial mySerial(/*rx =*/4, /*tx =*/5);
  DFRobot_EnvironmentalSensor environment(/*addr =*/SEN050X_DEFAULT_DEVICE_ADDRESS, /*s =*/&mySerial);
#else
  DFRobot_EnvironmentalSensor environment(/*addr =*/SEN050X_DEFAULT_DEVICE_ADDRESS, /*s =*/&Serial1);
#endif
#else
DFRobot_EnvironmentalSensor environment(/*addr = */SEN050X_DEFAULT_DEVICE_ADDRESS, /*pWire = */&Wire);
#endif
void setup()
{
#if MODESWITCH
  //初始化MCU通信串口
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  mySerial.begin(9600);
#elif defined(ESP32)
  Serial1.begin(9600, SERIAL_8N1, /*rx =*/D3, /*tx =*/D2);
#else
  Serial1.begin(9600);
#endif
#endif
  Serial.begin(115200);
  while(environment.begin() != 0){
    Serial.println(" Sensor initialize failed!!");
    delay(1000);
  }
  Serial.println(" Sensor  initialize success!!");
}

void loop()
{
  //打印从传感器后获取的数据
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
  Serial.print("Ultraviolet intensity: ");
  Serial.print(environment.getUltravioletIntensity());
  Serial.println(" mw/cm2");
  Serial.print("LuminousIntensity: ");
  Serial.print(environment.getLuminousIntensity());
  Serial.println(" lx");
  Serial.print("Atmospheric pressure: ");
  Serial.print(environment.getAtmospherePressure(HPA));
  Serial.println(" hpa");
  Serial.print("Elevation: ");
  Serial.print(environment.getElevation());
  Serial.println(" m");
  Serial.println("-------------------------------");
  delay(500);
}