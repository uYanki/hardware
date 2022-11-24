/*
  文件名称：sta.ino
  功能：sta连接路由器
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/

#include <ESP8266WiFi.h>

const char* ssid     = "Doit";
const char* password = "doit3305";

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}

