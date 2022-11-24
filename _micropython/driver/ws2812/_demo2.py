import utime
from neopixel import NeoPixel
from machine import Pin
from ws2812Util import WS2812DrawUtil
from WS2812ImgData import *

#WS2812点阵屏幕宽度
SCREEN_WIDTH=8
#WS2812点阵屏幕高度
SCREEN_HEIGHT=8

#级联的灯珠总数量
num_leds = SCREEN_WIDTH*SCREEN_HEIGHT
#创建像素序列对象
np = NeoPixel(Pin(21), num_leds)


du=WS2812DrawUtil(SCREEN_WIDTH,SCREEN_HEIGHT)
while True:
    du.drawImg(img8x8A,0,0)
    du.show(np,0.2)
    utime.sleep(2)
    du.drawImg(img8x8B,0,0)
    du.show(np,0.2)
    utime.sleep(2)
    du.drawImg(img8x8Black,0,0)
    du.show(np)
    utime.sleep(2)

        
