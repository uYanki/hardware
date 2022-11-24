from bmp180 import BMP180
import machine
from machine import Pin
import utime


# 初始化I2C1，服务于BMP180
sda = Pin(27)
scl = Pin(14)
i2c1 = machine.I2C(1, sda=sda, scl=scl, freq=100000)
bmp180 = BMP180(i2c1)
bmp180.oversample_sett = 2
# 设置大气压基值
bmp180.baseline = 101325

while True:
    # 获取温度数据
    temp = bmp180.temperature
    # 获取气压数据并换算为标准大气压
    p = bmp180.pressure/101325
    # 获取海拔数据
    altitude = bmp180.altitude
    # 在OLED屏幕上显示信息
    print("Temp:"+str(temp))
    print("Press:"+str(p))
    print("altitude:"+str(altitude))
    utime.sleep(0.2)
