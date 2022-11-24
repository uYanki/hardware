from machine import Pin, SPI
from st7735 import ST7735
from time import sleep
import st34dt05a as pdm

spi = SPI(0, baudrate=20000000, polarity=0, phase=0, sck=Pin(18), mosi=Pin(19))
lcd = ST7735(160, 128, spi, dc=Pin(14), cs=Pin(17), rst=Pin(16), rot=3, bgr=0)
lcd.fill(0xFFFF00)
lcd.show()

pcm_rate = 8_000  # Hz - default is 12kHz i.e. 3.072MHz bit-sample rate
pdm.bit_sample_freq = pcm_rate * 256
pdm.buf_len = 128

pdm_clk = Pin(3)
pdm_data = Pin(2)


def buffer_handler(inactive_buf):
    pdmdata = pdm.get_buffer(inactive_buf)
    i = 0
    global lcd
    lcd.fill(0xFFFF00)
    for b in range(len(pdmdata)-1):
        lcd.line(i, int(pdmdata[b]/2), i+1, int(pdmdata[b+1]/2), 0xFFFF)
        i += 1
    lcd.show()


pdm.init(pdm_clk, pdm_data, handler=buffer_handler)
pdm.start()

while True:
    print("running")
    sleep(1)
