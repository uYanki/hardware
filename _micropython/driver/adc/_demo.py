import machine
from machine import Pin
from machine import ADC
import utime

adcIn = ADC(Pin(36))

# 量程设置
# ADC.ATTN_0DB — 1.2V
# ADC.ATTN_2_5DB — 1.5V
# ADC.ATTN_6DB — 2.0V
# ADC.ATTN_11DB —  3.3V
adcIn.atten(ADC.ATTN_11DB)

while True:
    val = adcIn.read_u16()
    # 有效区间 0~4095
    print(val/4095)
    utime.sleep(0.2)
