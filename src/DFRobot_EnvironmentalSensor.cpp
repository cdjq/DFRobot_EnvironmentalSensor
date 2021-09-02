/*!
 * @file  DFRobot_EnvironmentalSensor.cpp
 * @brief DFRobot_EnvironmentalSensor 类的基础结构，基础方法的实现
 *
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence   The MIT License (MIT)
 * @author    [TangJie](jie.tang@dfrobot.com)
 * @version   V1.0
 * @date      2021-08-31
 * @url       https://github.com/cdjq/DFRobot_EnvironmentalSensor
 */

#include "DFRobot_EnvironmentalSensor.h"

#ifdef __AVR__
typedef uint16_t    platformBitWidth_t;
#else
typedef uint32_t    platformBitWidth_t;
#endif



DFRobot_EnvironmentalSensor::DFRobot_EnvironmentalSensor(uint8_t addr, TwoWire *pWire)
{
  _pWire = pWire;
  _addr = addr;
};

DFRobot_EnvironmentalSensor::DFRobot_EnvironmentalSensor(uint8_t addr, Stream *s):DFRobot_RTU(s)
{
  _s = s;
  _addr = addr;
}

int8_t DFRobot_EnvironmentalSensor::begin(uint16_t pid)
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

uint16_t DFRobot_EnvironmentalSensor::getDevicePID(void)
{
  uint8_t buf[2];
  if(_pWire){
    readReg(REG_PID, buf, 2);
  }else{
    readReg(REG_PID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);

}

uint16_t DFRobot_EnvironmentalSensor::getDeviceVID(void)
{
  uint8_t buf[2];
  if(_pWire){
    readReg(REG_VID, buf, 2);
  }else{
    readReg(REG_VID, buf, 2);
  }
  return (buf[0] << 8 | buf[1]);

}

float DFRobot_EnvironmentalSensor::getTemperature(uint8_t unist)
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

uint16_t DFRobot_EnvironmentalSensor::getHumidity(void)
{
  uint8_t buffer[2];
  int16_t humidity;

  readReg(REG_HUMIDITY,buffer, 2);
  humidity = buffer[0] << 8 | buffer[1];
  return humidity;
}

float DFRobot_EnvironmentalSensor::getUltravioletIntensity(void)
{
  uint16_t ultraviolet;
  uint8_t buffer[2];
  readReg(REG_ULTRAVIOLET_INTENSITY,buffer,2);
  ultraviolet = buffer[0] << 8 | buffer[1];
  return (ultraviolet / 100) + (ultraviolet % 100) * 0.01;
}

uint16_t DFRobot_EnvironmentalSensor::getLuminousIntensity(void)
{
  uint16_t luminous;
  uint8_t buffer[2];
  readReg(REG_LUMINOUS_INTENSITY, buffer, 2);
  luminous = buffer[0] << 8 | buffer[1];
  return luminous;
}

uint16_t DFRobot_EnvironmentalSensor::getAtmospherePressure(uint8_t units)
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

uint16_t DFRobot_EnvironmentalSensor::getElevation(void)
{
  uint16_t elevation;
  uint8_t buffer[2];
  readReg(REG_ELEVATION, buffer, 2);
  elevation = buffer[0] << 8 | buffer[1];
  return elevation;
}

uint8_t DFRobot_EnvironmentalSensor::readReg(uint16_t reg, void *pBuf, uint8_t size)
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
    return readHoldingRegister(_addr, reg, _pBuf, size);
  }
}

bool  DFRobot_EnvironmentalSensor::detectDeviceAddress(uint8_t addr)
{
   if(_pWire){
   uint8_t buf[2];
   readReg(REG_DEVICE_ADDR, buf, 2);
   if(addr == ((buf[0] << 8 | buf[1]) & 0xFF))
    return true;
    
  }else{
    uint16_t ret = readHoldingRegister(addr, REG_DEVICE_ADDR);
    if((ret & 0xFF) == addr)
      return true;
  }
  return false;
}