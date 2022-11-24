

from machine import Pin, SoftI2C
from bmp280 import BMP280
from time import sleep

i2c = SoftI2C(sda=Pin(2), scl=Pin(3), freq=400000)
bmp = BMP280(i2c)

while True:
    print(bmp.temperature, bmp.pressure)
    sleep(0.5)
