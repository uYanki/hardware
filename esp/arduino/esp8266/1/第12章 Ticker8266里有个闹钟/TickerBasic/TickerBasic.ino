/*
  文件名称：TickerBasic.ino
  功能：ESP8266 定时器控制led
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <Ticker.h>
#define LEDPin 16
Ticker timer;

int count = 0;

void flip()
{
  int state = digitalRead(LEDPin); 
  digitalWrite(LEDPin, !state);
  
  ++count;
  //当翻转次数达到20次时，定时器每隔0.1秒翻转一次led
  if (count == 20)
  {
    timer.attach(0.1, flip);
  }
  //当翻转次数达到120次时关闭定时器
  else if (count == 120)
  {
    timer.detach();
  }
}

void setup() {
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
  
  //每隔0.3s led状态翻转一次
  timer.attach(0.3, flip);
}

void loop() {
}