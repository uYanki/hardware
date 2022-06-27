from machine import Pin, SoftI2C, Timer
import time
i2c = SoftI2C(scl=Pin(5), sda=Pin(4), freq=100000)
str = i2c.scan()
print('%x' % str[0])

BH1750_CMD_POWERDOWN = 0x0
BH1750_CMD_POWERON = 0x1
BH1750_CMD_RESET = 0x7
BH1750_CMD_H_RESOLUTION = 0x10
BH1750_CMD_H_RESOLUTION2 = 0x11
BH1750_CMD_L_RESOLUTION = 0x13
BH1750_CMD_ONETIME_H = 0x20
BH1750_CMD_ONETIME_H2 = 0x21
BH1750_CMD_ONETIME_L = 0x23

BH1750_I2C_ADD = 0x5c  # 0x23

buf = bytearray(1)
buf[0] = BH1750_CMD_H_RESOLUTION

i2c.writeto(BH1750_I2C_ADD, buf)
time.sleep_ms(200)

while True:
    buf = i2c.readfrom(BH1750_I2C_ADD, 0x2)
    data = buf[0] * 256 + buf[1]
    print(data)
    time.sleep_ms(200)
