import tm1637
from machine import Pin
import utime

clk = Pin(25,Pin.OUT)
dio = Pin(26,Pin.OUT)

tm=tm1637.TM1637(clk=clk,dio=dio)

while True:
    #显示“时:分”格式数据
    tm.numbers(12,58)
    utime.sleep(2)
    #显示数字
    tm.number(-127)
    utime.sleep(2)
    tm.number(1279)
    utime.sleep(2)
    #显示16进制数字
    tm.hex(0xABCD)
    utime.sleep(2) 
    #打开所有LED
    tm.write([127,255,127,127])
    utime.sleep(2) 
    #关闭所有LED
    tm.write([0,0,0,0])
    utime.sleep(2) 
