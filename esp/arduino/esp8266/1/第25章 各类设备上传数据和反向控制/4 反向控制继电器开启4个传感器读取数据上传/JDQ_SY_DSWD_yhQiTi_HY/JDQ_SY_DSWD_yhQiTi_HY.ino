/********************************************************************************************************************
  基于ESPDuino开发板，应用于iot.doit.am物联网平台，集上传传感器数据到远端与远端反向控制终端的例程源码
  本例采用TCP协议，通过WiFi透传模式进行远端控制与传输
  时间：2016.09.10；
  传感器：  35继电器  *27高敏感度声音  *31 DS18B20数字温度传感器    (有害气体检测)  *16火焰
  功能说明：继电器控制后面4个输入端传感器，远端控制继电器，通过触发继电器开启4个输入传感器，4个传感器通过读取所处环境的各种状态值，
  然后每隔0.5秒把4个值发送到远端平台上面显示出来；
  硬件连接：继电器--D4  高敏感度声音--D5  DS温度--D12  有害气体--D13  火焰--D14
*********************************************************************************************************************/
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define u8 unsigned char

//继电器
#define relays 4
//高敏感声音
#define sound 5
//有害气体
#define gas_do 13
#define gas_ao A0
//火焰
#define flame 14
//DS温度
#define ONE_WIRE_BUS 12 // 数据线接数据口12
OneWire oneWire(ONE_WIRE_BUS); //实例化一个对象
DallasTemperature sensors(&oneWire); //实例化一个对象

int val_flame ;//定义数字变量
int GMGsound = 0; // 声音值变量
int val_gas = 1;
int mo = 0;
int dstemp = 0;
//所连接的WiFi热点账密
const char* ssid     = "Doit";
const char* password = "doit3305";

//数据接收的网络平台
const char* host = "iot.doit.am";//物联网平台
const int httpPort = 8810;
//登录平台的 用户名 与 秘钥Key （随机数）
const char* streamId   = "lin";
const char* privateKey = "dee5d879314b54dca7813364ff94b79a";
char ser[64];//串口接收数据缓冲区
char str[512];//网页接收数据缓冲区
WiFiClient client;// 使用WiFi客户端类创建TCP连接
//反向控制：
unsigned long MS_TIMER = 0;
unsigned long lastMSTimer = 0;
//String comdata = "";
char flag = false;

//各种传感器的初始化
void sensor_init()
{
  //继电器
  pinMode(relays, OUTPUT);
  digitalWrite(relays, LOW);
  //高敏感声音
  pinMode(sound, INPUT);
  //DS温度
  sensors.begin(); //调用该对象的方法，启动传感器初始化
  //有害气体
  pinMode(gas_do, INPUT);
  //火焰
  pinMode(flame, INPUT);

}
void setup()
{
  Serial.begin(115200);
  sensor_init();
  //  timer.attach(0.1, tick_sensor_control);
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
  //5s读一次
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
    GMGsound = digitalRead(sound); //读声音传感器的值  HIGH/LOW
    sensors.requestTemperatures(); // 发送命令去读取温度
    val_gas = digitalRead(gas_do); //LOW/HIGH  Y/N
    mo = (analogRead(gas_ao)) * 5;
    mo = map(mo, 0, 1023 * 5, 10, 1000);
    val_flame = digitalRead(flame); //将数字接口4的值读取赋给val   HIGH/LOW
    dstemp = sensors.getTempCByIndex(0);
    sprintf(ser, "SensorValue =%d HarmfulGas = %d flame = %d Temperature = %d", GMGsound, val_gas, val_flame, dstemp);
    Serial.println(ser);
    //    sprintf(str, "cmd=upload&device_name=GMGsound&data=%d&device_name=HarmfulGas&data=%d&device_name=Flame&data=%d&device_name=Temperature&data=%d&uid=lin&key=%s\r\n", GMGsound, val_gas, val_flame, dstemp);
    sprintf(str, "cmd=upload&device_name=arduino1&data=%d&device_name=arduino2&data=%d&device_name=arduino3&data=%d&device_name=arduino4&data=%d&uid=lin&key=%s\r\n", GMGsound, val_gas, val_flame, dstemp);
    client.print(str);
  }

  if (client.available())
  {
    //读并处理
    // R读取服务器的应答的所有行，并把它们打印到串口
    String recDataStr = client.readStringUntil('\n');
    Serial.println(recDataStr);
    //方向控制
    if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=start\r") == 0)
    {
      digitalWrite(relays, HIGH); //继电器导通；
      Serial.println("Start all sensors !");
    }
    else if (recDataStr.compareTo("cmd=publish&device_name=arduino&device_cmd=close\r") == 0)
    {
      digitalWrite(relays, LOW); //继电器断开；
      Serial.println("Close all sensors !");
    }

  }
}



