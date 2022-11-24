
from machine import Pin, SoftI2C
from sht30 import SHT30
from time import sleep

i2c = SoftI2C(sda=Pin(20), scl=Pin(21), freq=400000)

sht = SHT30(i2c, i2c_address=0x44)

while True:
    print(sht.measure())
    sleep(0.3)

"".find
