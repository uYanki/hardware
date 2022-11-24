/************************************************************************************************
   基于ESPDuino开发板，应用于iot.doit.am物联网平台，集上传传感器数据到远端与远端反向控制终端的例程源码
   本例采用TCP协议，通过WiFi透传模式进行远端控制与传输
   用到的传感器：12三色RGB(DIP封装)LED灯 *7水银开关 *28模拟温度 *21线性霍尔///*24迷你磁簧//*23大磁簧
   功能说明：远端控制3色灯显示蓝绿红颜色，水银开关、模拟温度、霍尔/磁簧 3个传感器读取模块所处环境的倾斜度、温度和磁场值，
             再把数据发送到远端平台上面显示出来；
   硬件连接：3色灯--D4、D5、D12，水银--D13，模拟温度--A0  霍尔--D14
   时间：2016.09.10；
 ***********************************************************************************************/
#include <ESP8266WiFi.h>
#include <math.h>
#define u8 unsigned char

//3色LED_BGR
#define ledB 4
#define ledG 5
#define ledR 12
//水银开关(检测倾度)
#define buttonpin 13 //定义水银开关传感器接口
//模拟温度
#define AoTemp A0
//迷你霍尔
#define hall 14

int vala;//定义数字变量val
int valb;//迷你霍尔变量值
int ato;//模拟温度变量

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

unsigned long lastTick = 0;
char ser[64];

//各种传感器的初始化
void sensor_init()
{
  //3色LED灯
  pinMode(ledB, OUTPUT);
  digitalWrite(ledB, LOW);
  pinMode(ledG, OUTPUT);
  digitalWrite(ledG, LOW);
  pinMode(ledR, OUTPUT);
  digitalWrite(ledR, LOW);
  //水银开关
  pinMode(buttonpin, INPUT); //定义倾斜开关传感器为输出接口
  //模拟温度
  //迷你线性霍尔(检测是否存在磁场)
  pinMode(hall, INPUT);
}
//模拟温度的转换函数
double Thermister(int RawADC)
{
  double Temp;
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
  Temp = Temp - 273.15;
  // 转换温度值;
  return Temp;
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
  else
  {
    Serial.println("connection ok");
  }
  lastTick = millis();
}

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
  //5s读一次
  if (millis() - lastTick > 1000)
  {
    lastTick = millis();
    static bool first_flag = true;
    if (first_flag)
    {
      Serial.println("subscribe");
      first_flag = false;
      sprintf(str, "cmd=subscribe&topic=lin\r\n");//注册用户名，以让后面的反向控制得以是实现
      client.print(str);
      return;
    }
    //读传感器并发送
    vala = digitalRead(buttonpin);//水银传感器返回值: 0：无倾斜，1：有倾斜
    valb = digitalRead(hall);//霍尔传感器返回值 0：有磁场，1：无磁场
    ato = Thermister(analogRead(AoTemp));
    sprintf(ser, "mercury: %d AoTemp: %d MagField: %d", vala, valb, ato);
    Serial.println(ser);
    sprintf(str, "cmd=upload&device_name=arduino1&data=%d&device_name=arduino2&data=%d&device_name=arduino3&data=%d&uid=lin&key=%s\r\n", vala, valb, ato, privateKey);
    client.print(str);
  }

  if (client.available())
  {
    //读并处理
    // R读取服务器的应答的所有行，并把它们打印到串口
    String recDataStr = client.readStringUntil('\n');
    Serial.println(recDataStr);
    //方向控制：注意LED灯是在设备arduino那里控制，蜂鸣器是在设备arduino1那里控制
    if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledBon\r") == 0)
    {
      digitalWrite(ledB, HIGH);
      Serial.println("Blue dot !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledBoff\r") == 0)
    {
      digitalWrite(ledB, LOW);
      Serial.println("Off Blue !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledGon\r") == 0)
    {
      digitalWrite(ledG, HIGH);
      Serial.println("Green dot !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledGoff\r") == 0)
    {
      digitalWrite(ledG, LOW);
      Serial.println("Off Green !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledRon\r") == 0)
    {
      digitalWrite(ledR, HIGH);
      Serial.println("Red dot !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=ledRoff\r") == 0)
    {
      digitalWrite(ledR, LOW);
      Serial.println("Off Red !");
    }
  }
}



