from machine import Pin, I2C
from mpu6050 import MPU6050
from time import sleep

i2c = I2C(sda=Pin(2), scl=Pin(3), freq=400000)
mpu = MPU6050(i2c,0x69)

while True:
    print(mpu.get_values())
    sleep(0.1)
