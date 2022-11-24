
# 呼吸灯的实现
from machine import Pin, PWM
import utime

# PWM方波频率(超过100KHz可能频率就不准了)
pl = 1000
pwmRed = PWM(Pin(25))
pwmGreen = PWM(Pin(26))
# 设置pwm频率
pwmRed.freq(pl)
pwmGreen.freq(pl)

# 每一步的休眠时间
SLEEP = 0.02
# 亮度变化的总步数
TOTAL_STEPS = 100
# 比例关系幂次
MI = 3
# 实际折算比
ratio = TOTAL_STEPS**MI

while True:
    # 变亮周期
    for i in range(TOTAL_STEPS+1):
        # 设置脉冲宽度
        pwmRed.duty_u16(int(65535*(i**MI)/ratio))
        pwmGreen.duty_u16(int(65535*((100-i)**MI)/ratio))
        utime.sleep(SLEEP)
    # 变暗周期
    for i in range(TOTAL_STEPS+1):
        # 设置脉冲宽度
        pwmRed.duty_u16(int(65535*((100-i)**MI)/ratio))
        pwmGreen.duty_u16(int(65535*(i**MI)/ratio))
        utime.sleep(SLEEP)
