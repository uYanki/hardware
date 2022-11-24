/*
  文件名称：eeprom_write.ino
  功能：ESP8266 EEPROM保存数据
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <EEPROM.h>

// 当前地址，下一个数据写入下一个地址
int addr = 0;

void setup()
{
  EEPROM.begin(512);
}

void loop()
{
  //anlogRead读取的数据范围为0-1023，eeprom 一字节范围为0-254所以除以4
  int val = analogRead(A0) / 4;
  //保存数据到对应的地址
  EEPROM.write(addr, val);

  //地址+1下个数据写入下一个地址
  addr = addr + 1;
  //当数据满512字节时吧这些数据存入flash
  if (addr == 512)
  {
    addr = 0;
    EEPROM.commit();
  }
  delay(100);
}
