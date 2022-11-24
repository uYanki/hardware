/*
  文件名称：CaptivePortal.ino
  功能：ESP8266 portal强制认证
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
//portal认证监听53端口
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
//portal认证页面
String responseHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
  "be redirected here.</p></body></html>";

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("CaptivePortal");

  // 吧所有的dns请求都转到apip
  dnsServer.start(DNS_PORT, "*", apIP);

  // 给所有请求都回复认证页面
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();
}

void loop() {
  //监听客户端请求
  dnsServer.processNextRequest();
  webServer.handleClient();
}
