/*
  文件名称：Simple_intranet_discovery.ino
  功能：ESP8266 建设一个自定义的udp发现服务
  作者：www.doit.am
  日期：2017-2
  版本：1.0
*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//最大客户端连接数
#define MAX_SRV_CLIENTS 1
//包大小
#define MAX_PACKETSIZE 512

//接入的路由器参数
const char* ssid = "Doit";
const char* password = "doit3305";
int udpPort=1234;//设置监听端口0-65535

//udp
WiFiUDP udp;
char buffUDP[MAX_PACKETSIZE];

/*
  *开始UDP服务
 */
void startUDPServer(int port)
{
  Serial.print("StartUDPServer at port:");
  Serial.println(port);
  udp.begin(port);
}

/*
  *监听读取udp包
 */
void doUdpServerTick()
{
  if (udp.parsePacket())
  {
    memset(buffUDP, 0x00, sizeof(buffUDP));
    udp.read(buffUDP, MAX_PACKETSIZE - 1);
    udp.flush();
    parseUDPPackage(buffUDP);
  }
}

/*
  *发送udp包
 */
void sendUDP(char *p)
{
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(p); udp.endPacket();
}

/*
  UDP接收包处理
*/
void parseUDPPackage(char *p)
{
  Serial.print("[UDP parseData:]");
  Serial.println(p);
  //ping命令
  if (strstr(p, "cmd=ping") != NULL) {
    char str[64];
    uint8_t mac[6];
    WiFi.macAddress(mac);
    IPAddress ip = WiFi.localIP();
    sprintf(str, "cmd=pong&host_ip=%d.%d.%d.%d&device_id=%02x%02x%02x%02x",ip[0],ip[1],ip[2],ip[3],
      mac[2], mac[3], mac[4], mac[5]);
    sendUDP(str);
  }
}

void setup() {
  Serial.begin(115200);//使用串口0透传数据
  Serial.println();
  Serial.println("Start WiFi...");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConnecting to "); 
  Serial.println(ssid);
  Serial.print("loadIP:");
  Serial.println(WiFi.localIP());

  startUDPServer(udpPort);
}

void loop() { 
  doUdpServerTick();
}
