/*
  文件名称：TickerParameter.ino
  功能：ESP8266 多个定时器对象控制led
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <Ticker.h>
#define LEDPin 16
Ticker tickerSetHigh;
Ticker tickerSetLow;

void setPin(int state) {
  digitalWrite(LEDPin, state);
}

void setup() {
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);
  
  // 每隔25毫秒调用一次 setPin(0) 
  tickerSetLow.attach_ms(25, setPin, 0);
  
  // 每隔26毫秒调用一次 setPin(1) 
  tickerSetHigh.attach_ms(26, setPin, 1);
}

void loop() {
}
