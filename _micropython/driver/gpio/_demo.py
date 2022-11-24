from machine import Pin
import utime


led = Pin(25, Pin.OUT)
led.value(0)

# 输入脚，PULL_DOWN -> 下拉电阻, PULL_UP -> 上拉电阻

btnUp = Pin(12, Pin.IN, Pin.PULL_UP)
btnDown = Pin(13, Pin.IN, Pin.PULL_DOWN)


def myInteruptHandler(pin):
    print(pin, pin.value())


btnUp.irq(trigger=Pin.IRQ_RISING, handler=myInteruptHandler)
btnDown.irq(trigger=Pin.IRQ_FALLING, handler=myInteruptHandler)


while True:
    led.togglt()
    utime.sleep(0.5)  # 500ms
