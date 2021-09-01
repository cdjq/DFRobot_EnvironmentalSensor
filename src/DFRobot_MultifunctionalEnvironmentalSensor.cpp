/*
 MIT License

 Copyright (C) <2019> <@DFRobot Frank>

��Permission is hereby granted, free of charge, to any person obtaining a copy of this
��software and associated documentation files (the "Software"), to deal in the Software
��without restriction, including without limitation the rights to use, copy, modify,
��merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
��permit persons to whom the Software is furnished to do so.

��The above copyright notice and this permission notice shall be included in all copies or
��substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "DFRobot_MultifunctionalEnvironmentalSensor.h"

#ifdef __AVR__
typedef uint16_t    platformBitWidth_t;
#else
typedef uint32_t    platformBitWidth_t;
#endif

#define REG_PID                   0x0000 //协议转换板的寄存器
#define REG_VID                   0x0001 //协议转换板的寄存器
#define REG_DEVICE_ADDR           0x0002 //协议转换板的寄存器
#define REG_UART_CTRL0            0x0003 //协议转换板的寄存器
#define EG_UART_CTRL1             0x0004 //协议转换板的寄存器
#define REG_VERSION               0x0005 //协议转换板的寄存器

#define REG_ULTRAVIOLET_INTENSITY 0x0008 //协议转换板的寄存器
#define REG_LUMINOUS_INTENSITY    0x0009 //协议转换板的寄存器
#define REG_TEMP                  0x000A //协议转换板的寄存器
#define REG_HUMIDITY              0x000B //协议转换板的寄存器
#define REG_ATMOSPHERIC_PRESSURE  0x000C //协议转换板的寄存器
#define REG_ELEVATION             0x000D //协议转换板的寄存器

DFRobot_Multifunctional_Environmental_Sensor::DFRobot_Multifunctional_Environmental_Sensor(uint8_t addr, TwoWire *pWire)
{
  _pWire = pWire;
  _addr = addr;
};

DFRobot_Multifunctional_Environmental_Sensor::DFRobot_Multifunctional_Environmental_Sensor(uint8_t addr, Stream *s):DFRobot_RTU(s)
{
  _s = s;
  _addr = addr;
}

uint8_t DFRobot_Multifunctional_Environmental_Sensor::begin(uint16_t pid)
{
  delay(500);
  setTimeoutTimeMs(200);
  if(_addr > 0xF7){
    DBG("Invaild Device addr.");
  }
  if(_addr != 0){
    if(!detectDeviceAddress(_addr)){
      DBG("Device addr Error.");
      return -1;
    }

    if(getDevicePID() != pid){
      DBG("PID Error");
      return -1;
    }
    
    if(getDeviceVID() != DEVICE_VID){
          DBG("VID Error");
          return -1;
      }

  }
  return 0;
}

uint16_t DFRobot_Multifunctional_Environmental_Sensor::getDevicePID(void)
{
  uint8_t buf[2];
  if(_pWire){
    readReg(REG_PID, buf, 2);
  }else{
    readReg(REG_PID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);

}

uint16_t DFRobot_Multifunctional_Environmental_Sensor::getDeviceVID(void)
{
  uint8_t buf[2];
  if(_pWire){
    readReg(REG_VID, buf, 2);
  }else{
    readReg(REG_VID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);

}

float DFRobot_Multifunctional_Environmental_Sensor::getTemperature(uint8_t unist)
{
  uint8_t buffer[2];
  int16_t data;
  float temp;
  
  readReg(REG_TEMP,buffer,2);

  data = buffer[0] << 8 | buffer[1];
  if(data >= 4500){
		data -= 4500;
		temp = data / 100 + ((data % 100) * 0.01);
	}else{
		data -= 4500;
		temp = data / 100 + (data % 100 ) * 0.01;
	}	
  if(unist == TEMP_F)
  {
    temp = temp * 1.8 + 32 ;
  }
  return temp;
}

uint16_t DFRobot_Multifunctional_Environmental_Sensor::getHumidity(void)
{
  uint8_t buffer[2];
  int16_t humidity;

  readReg(REG_HUMIDITY,buffer, 2);
  humidity = buffer[0] << 8 | buffer[1];
  return humidity;
}

float DFRobot_Multifunctional_Environmental_Sensor::getUltravioletIntensity(void)
{
  uint16_t ultraviolet;
  uint8_t buffer[2];
  readReg(REG_ULTRAVIOLET_INTENSITY,buffer,2);
  ultraviolet = buffer[0] << 8 | buffer[1];
  return (ultraviolet / 100) + (ultraviolet % 100) * 0.01;
}

uint16_t DFRobot_Multifunctional_Environmental_Sensor::getLuminousIntensity(void)
{
  uint16_t luminous;
  uint8_t buffer[2];
  readReg(REG_LUMINOUS_INTENSITY, buffer, 2);
  luminous = buffer[0] << 8 | buffer[1];
  return luminous;
}

uint16_t DFRobot_Multifunctional_Environmental_Sensor::getAtmospherePressure(uint8_t units)
{
  uint16_t atmosphere;
  uint8_t buffer[2];
  readReg(REG_ATMOSPHERIC_PRESSURE, buffer, 2); 
  atmosphere = buffer[0] << 8 | buffer[1];
  if(units == KPA){
    atmosphere /=10;
  }
  return atmosphere;
}

uint16_t DFRobot_Multifunctional_Environmental_Sensor::getElevation(void)
{
  uint16_t elevation;
  uint8_t buffer[2];
  readReg(REG_ELEVATION, buffer, 2);
  elevation = buffer[0] << 8 | buffer[1];
  return elevation;
}

uint8_t DFRobot_Multifunctional_Environmental_Sensor::readReg(uint16_t reg, void *pBuf, uint8_t size)
{
  uint8_t* _pBuf = (uint8_t*)pBuf;
  
    if(pBuf == NULL){
      DBG("data error");
      return 0;
    }
  if(_pWire){
    uint8_t _reg = reg * 2;
    _pWire->begin();
    _pWire->beginTransmission(_addr);
    _pWire->write(_reg);
    _pWire->endTransmission();
    _pWire->requestFrom(_addr, size);
    for(uint8_t i = 0; i < size; i++)
      _pBuf[i] = _pWire->read();
      
    return size;
  }else{
    return readInputRegister(_addr, reg, _pBuf, size);
  }
}

bool  DFRobot_Multifunctional_Environmental_Sensor::detectDeviceAddress(uint8_t addr)
{
   if(_pWire){
   uint8_t buf[2];
   readReg(REG_DEVICE_ADDR, buf, 2);
   if(addr == ((buf[0] << 8 | buf[1]) & 0xFF))
    return true;
    
  }else{
    uint16_t ret = readInputRegister(addr, REG_DEVICE_ADDR);
    if((ret & 0xFF) == addr)
      return true;
  }
  return false;
}