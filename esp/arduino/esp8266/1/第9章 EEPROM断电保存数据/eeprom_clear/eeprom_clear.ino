/*
  文件名称：eeprom_clear.ino
  功能：ESP8266 清除EEPROM保存的数据
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <EEPROM.h>
void setup()
{
  EEPROM.begin(512);
  //把eeprom0-511写0
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
  // 写完led打开led
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  EEPROM.end();
}
void loop(){}
