import machine
from machine import Pin
from machine import ADC
from ssd1306 import SSD1306_I2C
import utime
import Keyboard as bnkd

adcIn = machine.ADC(Pin(36))
adcIn.atten(ADC.ATTN_11DB)
sda = Pin(25)
scl = Pin(26)
i2c = machine.I2C(0, sda=sda, scl=scl, freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

# 屏幕尺寸
SCREEN_WIDTH = 128
SCREEN_HEIGHT = 64

# 可控制方块的边长
span = 20
# 每次方块移动的步进
step = 2
# 方块的绘制模式
mode = True

# 可控制方块的位置
x = 54
y = 22

while True:
    kc = bnkd.keyCode(adcIn)
    oled.fill(0)
    if mode:
        oled.fill_rect(x, y, span, span, 1)
    else:
        oled.rect(x, y, span, span, 1)
        oled.fill_rect(x+5, y+5, span-10, span-10, 1)
    oled.show()
    if kc == 1:  # Left
        x = x-step
    elif kc == 0:  # Up
        y = y-step
    elif kc == 2:  # Down
        y = y+step
    elif kc == 3:  # Right
        x = x+step
    elif kc == 4:  # A
        mode = True
    elif kc == 5:  # B
        mode = False
    # 检查更新后的方块位置
    # 不允许超出屏幕
    if x < 0:
        x = 0
    elif x > SCREEN_WIDTH-span:
        x = SCREEN_WIDTH-span
    if y < 0:
        y = 0
    elif y > SCREEN_HEIGHT-span:
        y = SCREEN_HEIGHT-span
    utime.sleep(0.005)
