from machine import Pin, I2C
from lis3dh import LIS3DH_I2C
from time import sleep

i2c = I2C(0, sda=Pin(2), scl=Pin(15), freq=400000)

lis = LIS3DH_I2C(i2c, address=0x19)

while True:
    sleep(0.1)
    print(lis.acceleration)
