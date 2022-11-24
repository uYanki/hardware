/*
  文件名称：mdns.ino
  功能：ESP8266 使用mdns协议访问设备页面
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//路由器信息
const char* ssid = "Doit";
const char* password = "doit3305";
//实例化一个webserver
ESP8266WebServer webServer(80);

//响应的html
const char mainPageString[] PROGMEM="<!DOCTYPE html>\r\n\
<html lang='en'>\r\n\
<head>\r\n\
  <meta charset='UTF-8'>\r\n\
  <title>Hello World </title>\r\n\
</head>\r\n\
<body>\r\n\
  <H1>Hello ESP8266</H1>\r\n\
  form mDNS example.\r\n\
</body>\r\n\
</html>";

//初始化wifi设置
void initWifi(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//回应访问主页"esp8266.local/"
void handleRoot(){
  webServer.send(200, "text/html", mainPageString);
  webServer.client().stop();
}
//回应未找到的请求
void handleNotFound(){
  webServer.send(404, "text/html", "404 File Not Found!");
  webServer.client().stop();
}
void setup(void)
{  
  Serial.begin(115200);
  initWifi();
  //初始化MDNS，设置域名为esp8266
  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
    while(1) { 
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  //设置mdns服务
  MDNS.addService("http", "tcp", 80);
  //响应web请求
  webServer.on("/", HTTP_GET, handleRoot);
  webServer.onNotFound(handleNotFound);
  webServer.begin();
}

void loop(void)
{
  //监听客户端的web请求
  webServer.handleClient();
}
