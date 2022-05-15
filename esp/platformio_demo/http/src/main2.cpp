#include <Arduino.h>
#include <ArduinoJson.h>
#include <BluetoothSerial.h>
#include <HTTPClient.h>
#include <WiFi.h>

// 注:为让程序不撑爆flash(wifi和bluetooth共存所致),需在platformio.ini中配置分区表 board_build.partitions = huge_app.csv

// 本代码使用了以下模块: WiFi Bluetooth SerialPort ADC Http.POST Json
/**
 * @brief ①开启 django 的局域网访问 python manage.py runserver 0.0.0.0:8000
 *        ②通过蓝牙或串口发送 settings.json 配置 wifi和服务器地址
 *        ③再浏览器内访问 localhost:8000/chart 即可 (其他同一局域网下的设备访问 192.168.3.20:8000/chart )
 */

BluetoothSerial     SerialBT;
HTTPClient          http;
DynamicJsonDocument doc(1024);

#define PIN_ADC1 34
#define PIN_ADC2 35

void setup() {
    /* 初始化串口 */
    Serial.begin(115200);
    /* 初始化蓝牙 */
    SerialBT.begin("ESP32");  //设置蓝牙名
    SerialBT.setPin("1234");  //设置配对码
    /* 初始化引脚 */
    pinMode(PIN_ADC1, INPUT);
    pinMode(PIN_ADC2, INPUT);
}

inline void print_msg(const char* str) {
    Serial.print(str);
    SerialBT.print(str);
}

inline void println_msg(const char* str) {
    Serial.println(str);
    SerialBT.println(str);
}

template <typename T>
inline void print_data(const char* str, T val) {
    Serial.print(str);
    Serial.print(val);
    SerialBT.print(str);
    SerialBT.println(val);
}

String json;
int    interval;  //采样时间间隔 ms

void loop() {
    /* 串口接收消息(非阻塞式) */
    while (Serial.available())
        json += (char)Serial.read();
    /* 蓝牙接收消息(非阻塞式) */
    while (SerialBT.available())
        json += (char)SerialBT.read();
    /* 解析捕获的json信息 */
    if (json.length()) {
        deserializeJson(doc, json);
        String ssid   = doc["WiFi"]["ssid"];   // wifi 名称
        String pwd    = doc["WiFi"]["pwd"];    // wifi 密码
        String server = doc["Server"]["url"];  // 服务器地址
        interval      = doc["interval"];
        print_data("[ssid]", ssid);
        print_data("[pwd]", pwd);
        print_data("[server]", server);
        print_data("[interval]", interval);
        /* 连接 wifi */
        WiFi.begin(ssid.c_str(), pwd.c_str());
        // 输出连接状态
        print_msg("WiFi connecting");
    WiFiStatus:
        switch (WiFi.status()) {
            case WL_CONNECTED:  // 连接成功
                println_msg("WL_CONNECTED");
                break;
            case WL_CONNECT_FAILED:  // 连接失败
                println_msg("WL_CONNECT_FAILED");
                break;
            default:  // 正在连接
                print_msg(".");
                delay(300);
                goto WiFiStatus;
                break;
        }
        /* 配置服务器 */
        http.begin(server);
        /* 清空json */
        json.clear();
    }

    /* 上传采集到的adc值 */
    if (WiFi.status() == WL_CONNECTED) {
        // 获取ADC的值
        int adc1 = analogRead(PIN_ADC1);
        int adc2 = analogRead(PIN_ADC2);
        // 配置消息头,注:缺失该语句将导致数据无法上传
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // 设置超时
        http.setTimeout(3000);
        // 上传至服务器
        int httpCode = http.POST((String) "adc1=" + adc1 + (String) "&adc2=" + adc2);
        if (httpCode == 200) {
            print_data("[adc1]", adc1);
            print_data("[adc2]", adc2);
        } else
            println_msg("err!!!");
        // 延时指定时间(采样时间间隔)
        delay(interval);
    } else
        delay(1);
}
