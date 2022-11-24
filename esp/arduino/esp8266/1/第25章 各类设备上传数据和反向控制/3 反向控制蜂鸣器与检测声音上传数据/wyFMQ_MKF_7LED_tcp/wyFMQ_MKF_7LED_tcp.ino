/************************************************************************************************************
   基于ESPDuino开发板，应用于iot.doit.am物联网平台，集上传传感器数据到远端与远端反向控制终端的例程源码
   本例采用TCP协议，通过WiFi透传模式进行远端控制与传输
   时间：2016.09.01；
   传感器：  2无源蜂鸣器  *26麦克风  14七彩自动闪烁LED灯
   功能说明：蜂鸣器对着麦克风等，在远端控制平台通过反向控制触发 无源蜂鸣器 响一小段时间，
             麦克风等传感器会把接收到的声音信号的模拟信号转化为数字信号发送到远端物联网数据平台；
             在无源蜂鸣器响期间七彩自动闪烁LED灯会一直闪烁；
   硬件连接：无源蜂鸣器--D4  麦克风--D5  七彩自动闪烁LED灯--D12
*************************************************************************************************************/
#include <ESP8266WiFi.h>
#define u8 unsigned char

//无源蜂鸣器
#define p_buzzer 4
//麦克风
#define mike A0
//7彩自闪烁LED
#define led7 5
int sensorValue = 0; //声音值变量

//所连接的WiFi热点账密
const char* ssid     = "Doit";
const char* password = "doit3305";

//数据接收的网络平台
const char* host = "iot.doit.am";//物联网平台
//const char* host = "diot.doit.am";
const int httpPort = 8810;
//登录平台的 用户名 与 秘钥Key （随机数）
const char* streamId   = "lin";
const char* privateKey = "dee5d879314b54dca7813364ff94b79a";

char str[512];//接收数据缓冲区
WiFiClient client;// 使用WiFi客户端类创建TCP连接
//反向控制：
unsigned long MS_TIMER = 0;
unsigned long lastMSTimer = 0;
String comdata = "";
char flag = false;

//各种传感器的初始化
void sensor_init()
{
  //无源蜂鸣器   p_buzzer
  pinMode(p_buzzer, OUTPUT);
  digitalWrite(p_buzzer, LOW);
  //麦克风   mike
  //7彩自闪烁LED
  pinMode(led7, OUTPUT);
  digitalWrite(led7, LOW);
}

void setup()
{
  Serial.begin(115200);
  sensor_init();
  MS_TIMER = millis();
  ESP.wdtEnable(6000);

  //  wifi_station_set_auto_connect(0);//关闭自动连接
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
  //  WiFiClient client;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
}
unsigned long lastTick = 0;
void loop()
{
  //  sensorValue = analogRead(mike);//读声音传感器的值
  if (flag == false)
  {
    if (millis() - MS_TIMER > 5000)
    {
      MS_TIMER = millis();
    }
  }

  //读传感器并发送
  //1s读一次
  if (millis() - lastTick > 1000)
  {
    lastTick = millis();
    static bool first_flag = true;
    if (first_flag)
    {
      first_flag = false;
      sprintf(str, "cmd=subscribe&topic=lin\r\n");//注册用户名，以让后面的反向控制得以是实现
      client.print(str);
      return;
    }
    //读传感器并发送
    sensorValue = analogRead(mike);//读声音传感器的值
    Serial.println(sensorValue, DEC); //以10进制的形式输出声音值
    sprintf(str, "cmd=upload&device_name=arduino1&data=%d&uid=lin&key=%s\r\n", sensorValue, privateKey);
    client.print(str);
  }

  if (client.available())
  {
    //读并处理
    // R读取服务器的应答的所有行，并把它们打印到串口
    String recDataStr = client.readStringUntil('\n');
    Serial.println(recDataStr);
    //反向控制：注意LED灯是在设备arduino那里控制，蜂鸣器是在设备arduino1那里控制
    if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=beepon\r") == 0)
    {
      digitalWrite(led7, HIGH);
      Serial.println("Beep is being open !");
      unsigned char i;
      for (i = 0; i < 100; i++) //输出一个频率的声音
      {
        digitalWrite(p_buzzer, HIGH); //发声音
        delayMicroseconds(40); // 延时40微秒
        digitalWrite(p_buzzer, LOW); //不发声音
        delayMicroseconds(40); //延时40微秒
      }
      digitalWrite(led7, LOW);
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=beepoff\r") == 0)
    {
      Serial.println("Beep has been closed !");//关闭蜂鸣器
      digitalWrite(p_buzzer, LOW);
      digitalWrite(led7, LOW);
    }
  }
}



