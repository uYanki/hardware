from machine import I2C,Pin
from servo import Servos

i2cO=I2C(0,sda=Pin(25),scl=Pin(26),freq=10000)
qd=Servos(i2cO)
qd.position(0,90)
qd.position(1,90)
