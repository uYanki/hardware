
# 功能：简易示波器

from machine import Pin
from machine import ADC
import machine
import utime
from ssd1306 import SSD1306_I2C


# 当前波形数据
# 每一条数据的取值范围-32~+31
waveData = []
# 初始化波形数据
for i in range(33):
    waveData.append(0)

# 推进一帧波形数据的方法


def pseuodata(newData):
    waveData.pop(0)
    waveData.append(newData)

# 绘制一帧波形数据的方法


def drawWave(oled, wd):
    # 清空屏幕
    oled.fill(0)
    # 绘制横坐标轴
    oled.line(0, 32, 127, 32, 1)
    # 绘制纵坐标轴
    oled.line(64, 0, 64, 63, 1)
    # 起始x坐标
    xStart = 0
    # 每一条数据之间的x距离
    xSpan = 4
    # 绘制每一条数据
    for i in range(len(wd)-1):
        oled.line(xStart+i*xSpan, 32-wd[i], xStart+(i+1)*xSpan, 32-wd[i+1], 1)
    oled.show()


adcIn = ADC(Pin(36))
# ADC.ATTN_0DB — 1.2V
# ADC.ATTN_2_5DB — 1.5V
# ADC.ATTN_6DB — 2.0V
# ADC.ATTN_11DB —  3.3V
adcIn.atten(ADC.ATTN_11DB)

sda = Pin(25)
scl = Pin(26)
i2c = machine.I2C(0, sda=sda, scl=scl, freq=400000)
oled = SSD1306_I2C(128, 64, i2c)

while True:
    drawWave(oled, waveData)
    u16Data = adcIn.read_u16()
    pseuodata(int(u16Data*64/65535-32))
    utime.sleep(0.05)
