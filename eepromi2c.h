#include <Arduino.h>
#include <I2C_DMAC.h>
#define DEVICE 0x50 //this is the device ID from the datasheet of the 24LC256

//in the normal write anything the eeaddress is incrimented after the writing of each byte. The Wire library does this behind the scenes.

//template <class T> int eeWrite(int ee, const T& value)
//{
//  const byte* p = (const byte*)(const void*)&value;
//  unsigned int i;
//  
//  Wire.beginTransmission(DEVICE);
//  Wire.write((int)(ee >> 8)); // MSB
//  Wire.write((int)(ee & 0xFF)); // LSB
//  for (i = 0; i < sizeof(value); i++)
//    Wire.write(*p++);
//  Wire.endTransmission();
//  return i;
//}

template <class T> int eeRead(int ee, T& value)
{
  uint8_t* p = (uint8_t*)(void*)&value;
  
  I2C.initWriteRegAddr(DEVICE, ee);        // Set-up DMAC to write to MPU6050 register pointer
  I2C.write();
  I2C.initReadBytes(DEVICE,p,sizeof(value));
  I2C.read();
  return sizeof(value);
}
