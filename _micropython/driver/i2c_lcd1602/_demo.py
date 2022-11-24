import utime

import machine
from machine import I2C
from lcd_api import LcdApi
from i2c_lcd import I2cLcd

I2C_ADDR     = 0x27
I2C_NUM_ROWS = 4
I2C_NUM_COLS = 20


#如果切换I2C接口要换I2C接口编号   以及针脚编号
i2c = I2C(0, sda=machine.Pin(25), scl=machine.Pin(26), freq=400000)
lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)    

while True:
    lcd.clear()
    lcd.putstr("It Works!")
    utime.sleep(2)
    lcd.clear()
    time = utime.localtime()
    lcd.putstr("{HH}:{MM}:{SS}".format(
        HH=str(time[3]), MM=str(time[4]), SS=str(time[5])))
    utime.sleep(2)
    lcd.show_cursor()
    utime.sleep(4)
    lcd.hide_cursor()
    utime.sleep(2)
    lcd.blink_cursor_on()
    utime.sleep(4)
    lcd.blink_cursor_off()
    utime.sleep(2)
    lcd.backlight_off()
    utime.sleep(2)
    lcd.backlight_on()
    utime.sleep(2)
    lcd.display_off()
    utime.sleep(2)
    lcd.display_on()
    utime.sleep(2)
    string = ""
    for x in range(32, 32+I2C_NUM_ROWS*I2C_NUM_COLS):
        string += chr(x)
    lcd.putstr(string)
    utime.sleep(2)




