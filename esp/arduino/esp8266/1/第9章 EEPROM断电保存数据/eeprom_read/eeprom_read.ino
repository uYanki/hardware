/*
  文件名称：eeprom_read.ino
  功能：ESP8266 读取EEPROM保存的数据
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <EEPROM.h>
int address = 0;
byte value;
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
}
void loop()
{
  //读取当前地址里的数据
  value = EEPROM.read(address);
  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  // 地址+1读取下一个数据
  address = address + 1;
  if (address == 512)
    address = 0;
  delay(500);
}
