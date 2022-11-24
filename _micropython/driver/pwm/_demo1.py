from machine import Pin
import utime

# ����Ƶ��
pl = 50
# ����ռ�ձ�
zkb = 0.05
# ����ÿ���ڸߵ�ƽʱ��
highPer = (1.0/pl)*zkb
# ����ÿ���ڵ͵�ƽʱ��
lowPer = (1.0/pl)*(1.0-zkb)

print(lowPer)
print(highPer)

# ģ��PWM

ledRed = Pin(25, Pin.OUT)
while True:
    ledRed.value(0)
    utime.sleep(lowPer)
    ledRed.value(1)
    utime.sleep(highPer)
