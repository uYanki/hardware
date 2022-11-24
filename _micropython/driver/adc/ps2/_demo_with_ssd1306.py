import machine
from machine import Pin
import utime
from ssd1306 import SSD1306_I2C

#屏幕尺寸
SCREEN_WIDTH=128
SCREEN_HEIGHT=64

adc0=machine.ADC(Pin(36))
adc0.atten(ADC.ATTN_11DB)
adc1=machine.ADC(Pin(39))
adc1.atten(ADC.ATTN_11DB)
sda=Pin(25)
scl=Pin(26)
i2c=machine.I2C(0,sda=sda,scl=scl,freq=400000)
oled=SSD1306_I2C(SCREEN_WIDTH,SCREEN_HEIGHT,i2c)

#可控制方块的边长
span=20

#根据模数转换模块读数返回X坐标的方法
def xGen(adc):
    value=adc.read_u16()
    #0~65535 -> 0~SCREEN_WIDTH-span
    return int(value*(SCREEN_WIDTH-span)/65535)

#根据模数转换模块读数返回Y坐标的方法
def yGen(adc):
    value=adc.read_u16()
    #0~65535 -> 0~SCREEN_HEIGHT-span
    return int(value*(SCREEN_HEIGHT-span)/65535)

#可控制方块的位置
x=xGen(adc0)
y=yGen(adc1)

while True:
    oled.fill(0)
    oled.fill_rect(x,y,span,span,1)
    oled.show()
    x=xGen(adc0)
    y=yGen(adc1)
    utime.sleep(0.08)


