from machine import Pin
import utime

# 方波频率
pl = 50
# 方波占空比
zkb = 0.05
# 方波每周期高电平时间
highPer = (1.0/pl)*zkb
# 方波每周期低电平时间
lowPer = (1.0/pl)*(1.0-zkb)

print(lowPer)
print(highPer)

# 模拟PWM

ledRed = Pin(25, Pin.OUT)
while True:
    ledRed.value(0)
    utime.sleep(lowPer)
    ledRed.value(1)
    utime.sleep(highPer)
