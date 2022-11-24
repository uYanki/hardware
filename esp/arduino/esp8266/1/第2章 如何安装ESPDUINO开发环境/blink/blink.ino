/*
  文件名称：blink.ino
  功能：ESP8266闪灯示例
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#define PIN_LED 16
#define KEY_FLASH 0

void setup() {
	//设置PINLED为输出模式
  	pinMode(PIN_LED, OUTPUT);    
  	//初始关闭LED灯
  	digitalWrite(PIN_LED,HIGH);
}
void loop() {
  //开启LED
  digitalWrite(PIN_LED, LOW);           
  delay(1000);//等待1s
  //关闭led
  digitalWrite(PIN_LED, HIGH);
  delay(1000);//等待1s
}
