import utime
from neopixel import NeoPixel
from machine import Pin

# 级联的灯珠总数量
num_leds = 1
# 创建像素序列对象
np = NeoPixel(Pin(21), num_leds)

# np[i]=(r,g,b) # 取值0~255, 值越大越亮，255刺眼

while True:
    for i in range(num_leds):
        np[i] = (8, 2, 2)  # rgb, 每位取值0~255, 值越大越亮，
    np.write()
    utime.sleep(1.0)
    for i in range(num_leds):
        np[i] = (0, 0, 0)
    np.write()
    utime.sleep(1.0)
