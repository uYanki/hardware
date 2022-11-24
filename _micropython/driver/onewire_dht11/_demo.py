import dht
import machine
from machine import Pin
import utime

# 基于指定的GPIO口创建DHT11对象
sensor = dht.DHT11(Pin(27))

while True:
    # 执行测量
    sensor.measure()
    # 在OLED屏幕上显示信息
    print("Temp:"+str(sensor.temperature())+"C")
    print("Humidity:"+str(sensor.humidity())+"%")
    utime.sleep(0.1)
