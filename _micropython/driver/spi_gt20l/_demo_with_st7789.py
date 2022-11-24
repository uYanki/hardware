import random
from time import sleep
from machine import Pin, SPI, PWM
import st7789
import gt20l

# 背光 （PWM调节亮度）
bl = PWM(Pin(4, Pin.OUT))
bl.freq(100)
bl.duty_u16(16000)
# 屏幕
spi1 = SPI(1, baudrate=40000000)  # mosi=Pin(13),sck=Pin(14)
lcd = st7789.ST7789(
    spi1, 135, 240,
    cs=Pin(15, Pin.OUT),
    reset=Pin(2, Pin.OUT),
    dc=Pin(12, Pin.OUT)
)
lcd.init()

#################################################

# 字库
spi2 = SPI(2, baudrate=40000000)  # mosi=Pin(23), miso=Pin(19),sck=Pin(18)
font = gt20l.gt20l(spi2, Pin(10, Pin.OUT))

# 字符显示


def showchar_gt20l(x, y, w, h, points, color):
    # points 字符点阵，w 点阵宽，h 点阵高
    for yoffset in range(0, h/8):
        for xoffset in range(w):
            byte = int(points[yoffset*w+xoffset], 16)  # base=16
            for i in range(8):  # one byte
                if (0x1 << i) & byte:
                    lcd.pixel(x + xoffset, y + i, color)
        y += 8  # one byte size


def showchar_816ascll(x, y, ch, color):
    global lcd, font
    c = font.get_816ascll(ch)
    showchar_gt20l(x, y, 8, 16, c, color)


def showstr_816ascll(x, y, s, color):
    for c in s:
        showchar_816ascll(x, y, c, color)
        x += 8  # 字符宽

# GB2312简体中文编码表
# http://tools.jb51.net/table/gb2312
# https://blog.csdn.net/anyuliuxing/article/details/84326207 (推荐)


def showchar_1516gb2312(x, y, ch, color):
    global lcd, font
    c = font.get_gb2312_font(ch)
    showchar_gt20l(x, y, 16, 16, c, color)


def showstr_1516gb2312(x, y, s, color):
    for c in s:
        showchar_1516gb2312(x, y, c, color)
        x += 15  # 字符宽


def gb2312_convert_to_two_byte(hans):  # [低4位,高4位]
    arr = []
    for han in hans:
        arr.append([han >> 8, han & 0xff])
    return arr


str_zh = gb2312_convert_to_two_byte(
    [0xB9FE, 0xE0B6, 0xA3AC, 0xCED2, 0xB2D9, 0xC4E3, 0xC2E8])  # 哈喽，我操你妈

#################################################

lcd.fill(st7789.BLACK)
showstr_816ascll(30, 80, "hello wrold", st7789.WHITE)
showstr_1516gb2312(10, 120, str_zh, st7789.WHITE)
