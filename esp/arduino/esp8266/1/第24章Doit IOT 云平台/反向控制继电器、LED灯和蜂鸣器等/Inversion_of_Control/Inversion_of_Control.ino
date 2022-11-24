/*********************************************************************************************************************
   基于ESPDuino开发板，应用于iot.doit.am物联网平台，集上传传感器数据到远端与远端反向控制终端的例程源码
   本例采用TCP协议，通过WiFi透传模式进行数据的远程传输
   用到的传感器：继电器1和继电器2， LED灯， 有源蜂鸣器；
   功能说明：上电后，在物联网数据控制平台上通过反向控制继电器1和继电器2的开闭、LED灯的亮灭、蜂鸣器的鸣叫。
   硬件连接：继电器1--D4， 继电器2--D5， LED灯--D12， 蜂鸣器--D13；
   时间：2016.09.10；
*********************************************************************************************************************/
//超声波与继电器
#include <ESP8266WiFi.h>
#include <Ticker.h>
#define Relays1 4
#define Relays2 5
#define led 12
#define beep 13
#define u8 unsigned char

Ticker timer;
const char* ssid     = "Doit";
const char* password = "doit3305";

const char* host = "iot.doit.am";//物联网平台
const int httpPort = 8810;

const char* streamId   = "lin";
const char* privateKey = "dee5d879314b54dca7813364ff94b79a";
char str[512];
WiFiClient client;// 使用WiFi客户端类创建TCP连接
//反向控制：
unsigned long MS_TIMER = 0;
unsigned long lastMSTimer = 0;
String comdata = "";
char flag = false;
void sensor_init()
{
  pinMode(Relays1, OUTPUT);
  digitalWrite(Relays1, HIGH);
  pinMode(Relays2, OUTPUT);
  digitalWrite(Relays2, HIGH);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(beep, OUTPUT);
  digitalWrite(beep, LOW);
}

void setup()
{
  Serial.begin(115200);
  MS_TIMER = millis();
  sensor_init();
  delay(10);
  WiFi.disconnect();/////////////////
  // 首先，我们连接到WiFi网络
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(50);
  Serial.print("connecting to ");
  Serial.println(host);

  // 使用WiFi客户端类创建TCP连接
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
}
unsigned long lastTick = 0;
void loop()
{
  if (millis() - lastTick > 1000)
  {
    lastTick = millis();
    static bool first_flag = true;
    if (first_flag)
    {
      first_flag = false;
      sprintf(str, "cmd=subscribe&topic=lin\r\n");
      client.print(str);
      return;
    }
  }
  if (client.available())
  {
    //读并处理
    // R读取服务器的应答的所有行，并把它们打印到串口
    String recDataStr = client.readStringUntil('\n');
    Serial.println(recDataStr);
    if (recDataStr.compareTo("cmd=publish&device_name=arduino1&device_cmd=lbopen\r") == 0)
    {
      digitalWrite(led, HIGH);
      digitalWrite(beep, HIGH);//蜂鸣器响
      Serial.println("Sound and light ON");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino1&device_cmd=lbclose\r") == 0)
    {
      digitalWrite(led, LOW);
      digitalWrite(beep, LOW);
      Serial.println("Sound and light OFF");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=j1\r") == 0)
    {
      digitalWrite(Relays1, LOW);
      digitalWrite(beep, HIGH);//蜂鸣器响
      Serial.println("Relays1 open");
      delay(30);
      digitalWrite(Relays1, HIGH);
      digitalWrite(beep, LOW);
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=j2\r") == 0)
    {
      digitalWrite(Relays2, LOW);
      digitalWrite(beep, HIGH);//蜂鸣器响
      Serial.println("Relays2 open");
      delay(30);
      digitalWrite(Relays2, HIGH);
      digitalWrite(beep, LOW);
    }
  }
}


