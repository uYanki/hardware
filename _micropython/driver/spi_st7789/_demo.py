import random
from time import sleep
from machine import Pin, SPI, PWM
import st7789

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

while True:
    lcd.fill(
        st7789.color565(
            random.getrandbits(8),
            random.getrandbits(8),
            random.getrandbits(8),
        ),
    )
    sleep(2)
