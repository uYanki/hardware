from machine import Pin,PWM
import utime

#PWM����Ƶ��(����100KHz����Ƶ�ʾͲ�׼��)
pl=100000
#PWM����ռ�ձ�
zkb=1.0
#ռ�ձȱ仯ֵ����ֵ
zkbSpan=0.01
#ռ�ձȲ���
zkbStep=-zkbSpan
#PWM�����ȣ�ȡֵ��Χ0-65535��
#0-����ռ�ձ�0%��65535-����ռ�ձ�100%
PWM_PulseWidth=int(65535*zkb)

#��ɫ����ɫLED��GPIO�ܽ�
pwmRed=PWM(Pin(25))
pwmGreen=PWM(Pin(26))
#����pwmƵ��
pwmRed.freq(pl)
pwmGreen.freq(pl)

while True:
    #����������
    pwmRed.duty_u16(PWM_PulseWidth)
    pwmGreen.duty_u16(65535-PWM_PulseWidth)
    zkb=zkb+zkbStep
    PWM_PulseWidth=int(65535*zkb)
    if zkb<=zkbSpan/10: zkbStep=zkbSpan
    if zkb>=1: zkbStep=-zkbSpan
    utime.sleep(0.02)

    
    




