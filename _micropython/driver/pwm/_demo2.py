from machine import Pin,PWM
import utime

#PWM方波频率(超过100KHz可能频率就不准了)
pl=100000
#PWM方波占空比
zkb=1.0
#占空比变化值绝对值
zkbSpan=0.01
#占空比步进
zkbStep=-zkbSpan
#PWM脉冲宽度（取值范围0-65535）
#0-代表占空比0%，65535-代表占空比100%
PWM_PulseWidth=int(65535*zkb)

#红色和绿色LED的GPIO管脚
pwmRed=PWM(Pin(25))
pwmGreen=PWM(Pin(26))
#设置pwm频率
pwmRed.freq(pl)
pwmGreen.freq(pl)

while True:
    #设置脉冲宽度
    pwmRed.duty_u16(PWM_PulseWidth)
    pwmGreen.duty_u16(65535-PWM_PulseWidth)
    zkb=zkb+zkbStep
    PWM_PulseWidth=int(65535*zkb)
    if zkb<=zkbSpan/10: zkbStep=zkbSpan
    if zkb>=1: zkbStep=-zkbSpan
    utime.sleep(0.02)

    
    




