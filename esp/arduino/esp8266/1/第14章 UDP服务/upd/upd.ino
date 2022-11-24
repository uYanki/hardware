/*
  文件名称：udp.ino
  功能：ESP8266 建立udp服务监听udp端口
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//udp包最大字节数
#define MAX_PACKETSIZE 512
WiFiUDP udp;
const char* ssid = "Doit";
const char* password = "doit3305";
//udp包缓冲区
char buffUDP[MAX_PACKETSIZE];

void startUDPServer(int port)
{
  Serial.print("\r\nStartUDPServer at port:");
  Serial.println(port);
  udp.begin(port);
}
void sendUDP(char *p)
{
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(p);udp.endPacket();
}
void doUdpServerTick()
{
  int packetSize = udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = udp.remoteIP();
    for (int i = 0; i < 4; i++) {//允许做多4个UDP客户端连接
      Serial.print(remote[i], DEC);
      if (i < 3) Serial.print(".");
    }
    Serial.print(", port ");
    Serial.println(udp.remotePort());
    memset(buffUDP,0x00,sizeof(buffUDP));
    udp.read(buffUDP, MAX_PACKETSIZE-1);
    udp.flush();
    Serial.println("Recieve:");
    Serial.println(buffUDP);
    sendUDP(buffUDP);//send back
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Started");
  WiFi.begin(ssid, password);
  //WiFi.softAP("ESP8266_WIFI");
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  Serial.println(WiFi.localIP());
  startUDPServer(8000);
}

void loop() {
  doUdpServerTick();
  delay(1);
}

