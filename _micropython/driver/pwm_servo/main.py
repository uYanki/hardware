from machine import Pin,PWM
from time import sleep

# print(dir(PWM))

# 周期为 20ms / 50Hz 的 PWM 波

#  脉冲高电平持续时间              角度     占空比
#  0.5ms--------------------------0度；    2.5%
#  1.0ms-------------------------45度；     5.0%
#  1.5ms-------------------------90度；     7.5%
#  2.0ms------------------------135度；    10.0%
#  2.5ms------------------------180度；    12.5%


class servo():
    
    def __init__(self,pin):
        self.pwm = PWM(Pin(pin),freq=50)
        
    def set_angle(self,angle):
        duty = ((angle+90)/180*2+0.5)/20  
        self.pwm.duty_u16(int(duty*65535))

obj = servo(9)

for i in range(-4,4):
    obj.set_angle(22.5*i)
    sleep(0.5)

