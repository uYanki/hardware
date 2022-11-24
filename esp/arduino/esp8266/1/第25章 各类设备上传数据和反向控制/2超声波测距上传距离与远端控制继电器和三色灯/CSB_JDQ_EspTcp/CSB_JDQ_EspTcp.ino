/************************************************************************************************
   基于ESPDuino开发板，应用于iot.doit.am物联网平台，集上传传感器数据到远端与远端反向控制终端的例程源码
   本例采用TCP协议，通过WiFi透传模式进行远端控制与传输
   采用读取HC-SR04超声波传感器获取距离再发送到远端(平台)、远端控制继电器和三色LED灯的亮灭演示反向控制控制
   时间：2016.09.01；
   传感器：  超声波传感器  双继电器   3色RGB_LED灯
   功能说明：上电后，超声波传感器不断扫描距离前方障碍物的距离并把数据发送到物联网数据平台，在平台上通过
             反向控制的方式控制继电器1和继电器2的开闭、控制3色LED灯的亮灭，可在串口看到相关响应；
   硬件连接：超声波(发送端)--D4  超声波(接收端)--D5  继电器1--D12  继电器2--D13  3色RGB_LED灯--D14/D15/D16
 ***********************************************************************************************/
//超声波与继电器
#include <ESP8266WiFi.h>

#define u8 unsigned char
//超声波收发
#define trigpin 4
#define echopin 5

//继电器1和2
#define JDQ1 12
#define JDQ2 13

//3色LED灯 RGB-LED
#define ledB 14
#define ledG 15
#define ledR 16

//所连接的WiFi热点账密
const char* ssid     = "Doit";
const char* password = "doit3305";

//数据接收的网络平台
const char* host = "diot.doit.am";//物联网平台
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
  //超声波
  pinMode(echopin, INPUT);
  pinMode(trigpin, OUTPUT);
  //继电器1和继电器2
  pinMode(JDQ1, OUTPUT);
  pinMode(JDQ2, OUTPUT);
  digitalWrite(JDQ1, HIGH);
  digitalWrite(JDQ2, HIGH);
  //3色LED灯  共阴极
  pinMode(ledB, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  digitalWrite(ledB, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledR, LOW);
}
/*******************************超声波传感器控制****************************************/
//时序-->读数
float Ultrasonic(void)
{
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);//发一个10ms的高脉冲去触发TrigPin
  float dist = pulseIn(echopin, HIGH); //接收高电平时间
  dist = dist / 58.0; //计算距离
  return dist;
}
/***************************************************************************************/
void setup()
{
  Serial.begin(115200);
  sensor_init();
  MS_TIMER = millis();

  delay(10);
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
  int distance = Ultrasonic();
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
    if (1 < distance && distance < 450)
    {
      sprintf(str, "cmd=upload&device_name=arduino&data=%d&uid=lin&key=%s\r\n", distance, privateKey);
      client.print(str);
      Serial.print("  distance: ");
      Serial.print(distance);//输出距离
      Serial.println("cm");//输出单位
    }
  }
  if (client.available())
  {
    //读并处理
    // R读取服务器的应答的所有行，并把它们打印到串口
    String recDataStr = client.readStringUntil('\n');
    Serial.println(recDataStr);
    //方向控制：注意继电器1和继电器2是在设备arduino1那里控制，三色灯是在设备arduino2那里控制
    if (recDataStr.compareTo("cmd=publish&device_name=arduino1&device_cmd=j1on\r") == 0)
    {
      digitalWrite(JDQ1, LOW);
      Serial.println("Relay_1 is being turned on !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino1&device_cmd=j1off\r") == 0)
    {
      digitalWrite(JDQ1, HIGH);
      Serial.println("Relay_1 has been closed !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino1&device_cmd=j2on\r") == 0)
    {
      digitalWrite(JDQ2, LOW);
      Serial.println("Relay_2 is being turned on !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino1&device_cmd=j2off\r") == 0)
    {
      digitalWrite(JDQ2, HIGH);
      Serial.println("Relay_2 has been closed !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=lbon\r") == 0)
    {
      digitalWrite(ledB, HIGH);
      Serial.println("You are opening the Blue light !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=lboff\r") == 0)
    {
      digitalWrite(ledB, LOW);
      Serial.println("You have closed the Blue light !");

    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=lgon\r") == 0)
    {
      digitalWrite(ledG, HIGH);
      Serial.println("You are opening the Green light !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=lgoff\r") == 0)
    {
      digitalWrite(ledG, LOW);
      Serial.println("You have closed the Green light !");

    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=lron\r") == 0)
    {
      digitalWrite(ledR, HIGH);
      Serial.println("You are opening the Red light !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino2&device_cmd=lroff\r") == 0)
    {
      digitalWrite(ledR, LOW);
      Serial.println("You have closed the Red light !");
    }
  }
}


