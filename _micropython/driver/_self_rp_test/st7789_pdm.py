import random
from time import sleep
from machine import Pin, SPI, PWM
import st7789
import st34dt05a as pdm

# 背光 （PWM调节亮度）
bl = PWM(Pin(22, Pin.OUT))
bl.freq(100)
bl.duty_u16(16000)
# 屏幕
spi1 = SPI(0, mosi=Pin(19), miso=Pin(16), sck=Pin(18), baudrate=40000000)
lcd = st7789.ST7789(
    spi1, 135, 240,
    cs=Pin(25, Pin.OUT),
    reset=Pin(23, Pin.OUT),
    dc=Pin(24, Pin.OUT)
)
lcd.init()
lcd.fill(st7789.RED)

pcm_rate = 8_000  # Hz - default is 12kHz i.e. 3.072MHz bit-sample rate
pdm.bit_sample_freq = pcm_rate * 256
pdm.buf_len = 240

pdm_clk = Pin(11)
pdm_din = Pin(10)


def buffer_handler(inactive_buf):
    global lcd, pdm
    pdmdata = pdm.get_buffer(inactive_buf)
    lcd.fill(st7789.RED)
    i = 0
    for b in range(len(pdmdata)-1):
        lcd.line(int(pdmdata[b] / 2), i,
                 int(pdmdata[b+1] / 2),  i+1, st7789.GREEN)
#         lcd.line(i, int(pdmdata[b]), i+1, int(pdmdata[b+1]), st7789.GREEN)
        i += 1


pdm.init(pdm_clk, pdm_din, handler=buffer_handler)
pdm.start()
