/******************************************************************************************
   基于ESPDuino开发板，应用于iot.doit.am物联网平台，集上传传感器数据到远端与远端反向控制终端的例程源码
   本例采用TCP协议，通过WiFi透传模式进行远端控制与传输.
   时间：2016.09.12；
   传感器：  *15金属触摸  *20光折断  11双色LED灯 *18红外避障
   功能说明：远端平台反向控制双色LED两米亮灭，光遮挡、红外避障、金属触摸等传感器时刻把状态值读取后
             发送到远端平台上面显示出来。
   硬件连接：双色LED--D4  金属触摸--D5  光遮挡--D12  红外避障--D13
*******************************************************************************************/
#include <ESP8266WiFi.h>

#define u8 unsigned char

//双色LED
#define led2 4
//金属触摸
#define metTouch 5
//光遮挡
#define lightBloc 12
//红外避障
#define IRavoi 13

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

char ser[64];//串口接收缓冲
char str[512];//网页接收数据缓冲区
WiFiClient client;// 使用WiFi客户端类创建TCP连接
//反向控制：
unsigned long MS_TIMER = 0;
unsigned long lastMSTimer = 0;
String comdata = "";
char flag = false;
int mt, liB, IRa;


//各种传感器的初始化
void sensor_init()
{
  pinMode(led2, OUTPUT);
  pinMode(metTouch, INPUT);
  pinMode(lightBloc, INPUT);
  pinMode(IRavoi, INPUT);
  digitalWrite(led2, LOW);
}

void setup()
{
  Serial.begin(115200);
  sensor_init();
  MS_TIMER = millis();

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
    mt=digitalRead(metTouch);
    liB=digitalRead(lightBloc);
    IRa=digitalRead(IRavoi);
    sprintf(ser, "MetTouch: %d; LightBloc: %d; IRavoi: %d.", mt, liB, IRa);
    Serial.println(ser);
    sprintf(str, "cmd=upload&device_name=arduino1&data=%d&device_name=arduino2&data=%d&device_name=arduino3&data=%d&uid=lin&key=%s\r\n", mt, liB, IRa, privateKey);
    client.print(str);
  }

  if (client.available())
  {
    //读并处理
    // R读取服务器的应答的所有行，并把它们打印到串口
    String recDataStr = client.readStringUntil('\n');
    Serial.println(recDataStr);
    //方向控制：注意LED灯是在设备arduino那里控制，蜂鸣器是在设备arduino1那里控制
    if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledon\r") == 0)
    {
      digitalWrite(led2, HIGH);
      Serial.println("2LED open!");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledoff\r") == 0)
    {
      digitalWrite(led2, LOW);
      Serial.println("2LED closed !");
    }
  }
}



