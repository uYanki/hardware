from machine import PWM, Pin

# 单舵机控制，控制舵机的PWM的参数：频率50Hz，占空比 1638~8192 （相对于u16来说）
# 将0~180之间的度数转换为舵机控制用占空比


def fromDegreeToZKB(deg):
    return int(1638+(8192-1638)*deg/180)


ctrl = PWM(Pin(25))
ctrl.freq(50)
ctrl.duty_u16(fromDegreeToZKB(145))
