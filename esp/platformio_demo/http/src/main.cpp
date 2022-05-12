#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define PIN_ADC 34

HTTPClient http;

void setup() {
  // 初始化串口
  Serial.begin(9600);
  // 连接WiFi
  WiFi.begin("HUAWEI-10GA2K", "13631520360");
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
  }
  Serial.println("WiFi connected");
  // 配置目标服务器
  http.begin("http://192.168.3.20:8000/add/");
  // 配置引脚
  pinMode(PIN_ADC, INPUT);
}

void loop() {
  delay(1e3);
  // 获取ADC的值
  int adc = analogRead(PIN_ADC);
  // 发送至服务器
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  // 没有这条代码将会导致参数无法上传
  int httpCode = http.POST((String) "sound=" + adc /*&light=adc*/);
  if (httpCode == 200) {
    Serial.println(adc);
  } else
    Serial.println("err");
}