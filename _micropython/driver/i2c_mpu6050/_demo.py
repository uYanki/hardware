from machine import Pin, I2C, SoftI2C
from mpu6050 import MPU6050
from time import sleep
import math

# rp2040
# i2c = I2C(0)  # 默认sda=20,scl=21
# i2c = I2C(0,sda=Pin(8,Pin.OUT), scl=Pin(9,Pin.OUT))

i2c = SoftI2C(sda=Pin(21, Pin.OUT), scl=Pin(22, Pin.OUT), freq=400000)

print(i2c.scan())

mpu = MPU6050(i2c)

while True:
    vals = mpu.get_values()
    # print(vals)
    print({
        "picth": math.atan2(vals["AcY"], vals["AcZ"]) * 57.3,
        "roll": math.atan2(vals["AcX"], vals["AcZ"]) * 57.3
    })
    sleep(0.1)
