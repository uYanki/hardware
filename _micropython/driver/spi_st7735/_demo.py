from machine import Pin, SPI
from st7735 import ST7735
from time import sleep


spi = SPI(0, baudrate=20000000, polarity=0, phase=0, sck=Pin(18), mosi=Pin(19))
lcd = ST7735(160, 128, spi, dc=Pin(14), cs=Pin(17), rst=Pin(16), rot=3, bgr=0)
lcd.fill(0xFFFF00)
lcd.text("H", 20, 40, 0xFFFF)
lcd.text("W", 60, 80, 0xFFFF)
lcd.line(20, 40, 60, 80, 0xFFFF)
lcd.show()
