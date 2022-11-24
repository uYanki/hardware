/*
  文件名称：weboled.ino
  功能：oled显示web发送的内容
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "OLED.h"

ESP8266WebServer server(80);   
//接入的路由器参数
const char* ssid = "Doit";
const char* password = "doit3305";

//GPIO4(SDA) GPIO5(SCL) 
OLED display(4, 5);

//web页面
String form =
  "<p>"
  "<center>"
  "<h1>ESP8266 OLED显示屏</h1>"
  "<img src='http://bbs.doit.am/static/image/common/logo_new.png'>"
  "<form action='msg'><p>内容：<input type='text' name='msg' size=50 autofocus> <input type='submit' value='提交'></form>"
  "</center>";

void setup() {
  //初始化oled
  display.begin();
  Serial.begin(9600);
  //开机画面
  display.print("ESP8266",2,4);
  display.print("WIFI Display",4,2);
  delay(3*1000);
  display.clear();

  display.print("Start WiFi...");

  //初始化wifi
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  //显示连接的热点
  display.print("Start WiFi...");
  display.print("Connecting to ",2,0); 
  char *buf = new char[strlen(ssid)+1];
  strcpy(buf, ssid);
  display.print(buf,4,1);
  int r=6,c=1;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(c>15){
      c=1;
      for(int i=1; i<16; i++){display.print(" ",r,i);}
    }
    display.print(".",r,c++);
  }
  //清除内容
  display.clear();

  //整理转化wifi信息
  char sip[16];
  char smac[16];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  IPAddress ip = WiFi.localIP();
  sprintf(sip, "%i.%i.%i.%i",ip[0],ip[1],ip[2],ip[3]);
  sprintf(smac, "%02X%02X%02X%02X%02X%02X",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  //显示wifi信息
  display.print("WIFI Message:");
  display.print("IP:",2,0);
  display.print(sip,3,1);
  display.print("MAC:",5,0);
  display.print(smac,6,1);
  //响应浏览器访问
  server.on("/",[]() {server.send(200, "text/html", form);});
  server.on("/msg", handle_msg);
  server.begin();   
}

void loop() {
  //监听客户端请求
  server.handleClient();
}
//发送web页面，显示收到的数据
void handle_msg() {
  display.clear();
  server.send(200, "text/html", form);
  char msg[50];
  strcpy(msg,server.arg("msg").c_str());
  display.print("Receive:");
  display.print(msg,2,0);
}
