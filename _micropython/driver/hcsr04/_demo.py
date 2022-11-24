import machine
from ssd1306 import SSD1306_I2C
from machine import Pin
import utime

def getDistance(trigger,echo):
    #将触发引脚拉低，然后暂停2微秒,以确保其未激活
    trigger.value(0)
    utime.sleep_us(2)
    #在将触发器引脚拉低之前，将触发器引脚拉高5微秒
    #这将从超声波传感器发送一个短脉冲，然后关闭该脉冲
    trigger.value(1)
    utime.sleep_us(5)
    trigger.value(0)
    #创建一个while循环以检查回波引脚。
    #如果没有收到回波脉冲，则更新变量
    #使其包含以微秒为单位的时间戳。
    while echo.value() == 0:
        signaloff = utime.ticks_us()
    #若收到了回波则开始另一个while循环
    #不断将当前时间戳（以微秒为单位）
    #存储到signalon变量中直至回波结束
    while echo.value() == 1:
        signalon = utime.ticks_us()
    #计算从超声波脉冲发出到返回的总时间
    timepassed = signalon - signaloff
    #将总时间乘以声速(343.2m/s，即0.0343cm/μs)，
    #该方程的乘积除以2，得到目标距离(厘米)。
    distance = (timepassed * 0.0343)/2
    return distance


#初始化I2C0，服务于OLED屏幕
sda=Pin(25)
scl=Pin(26)
i2c=machine.I2C(0,sda=sda,scl=scl,freq=400000)
oled=SSD1306_I2C(128,64,i2c)

#创建触发信号管脚对象
trigger = Pin(14, Pin.OUT)
#创建返回信号管脚对象
echo = Pin(27, Pin.IN)
#循环不断测量距离
while True:
    dis=getDistance(trigger,echo)
    #在OLED屏幕上显示信息
    oled.fill(0)
    oled.text("Distance:",0,0)
    oled.text(str(dis)+"CM",0,15)
    oled.show()
    utime.sleep(0.8)


