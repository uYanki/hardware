from machine import Pin
import machine
import utime
from ssd1306 import SSD1306_I2C

sleepSpan=1.5

sda=Pin(25)
scl=Pin(26)

i2c=machine.I2C(0,sda=sda,scl=scl,freq=400000)
oled=SSD1306_I2C(128,64,i2c)

while True:
    #在指定位置显示文字 str x y
    oled.text("Hello Pico!",0,20)
    oled.show()
    utime.sleep(sleepSpan)
    #奇数时反相显示，偶数时正常显示
    oled.invert(1)
    utime.sleep(sleepSpan)
    oled.invert(0)
    utime.sleep(sleepSpan)
    #填满屏幕
    oled.fill(1)
    oled.show()
    utime.sleep(sleepSpan)
    #调整亮度。0最暗，255最亮
    oled.contrast(32)
    utime.sleep(sleepSpan)
    oled.contrast(255)
    #清空屏幕
    oled.fill(0)
    #在指定位置绘制点
    oled.pixel(100, 32, 1)
    oled.line(5,5,123,5,1)
    oled.rect(15,15,80,40,1)
    oled.fill_rect(25,25,40,20,1)
    oled.show()
    utime.sleep(sleepSpan)
    #清空屏幕
    oled.fill(0)
    oled.show()
    utime.sleep(sleepSpan)


