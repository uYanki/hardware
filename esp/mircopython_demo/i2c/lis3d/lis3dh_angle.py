from machine import Pin, I2C
from lis3d import LIS3DH_I2C
from time import sleep

from math import atan,atan2,sqrt

# *pitch = (atan(acce->AcceX / sqrt(acce->AcceY * acce->AcceY + acce->AcceZ * acce->AcceZ))) * 57.3f; // *57.3f = *180/3.14
# *roll = (atan(acce->AcceY / sqrt(acce->AcceX * acce->AcceX + acce->AcceZ * acce->AcceZ))) * 57.3f;  // *57.3f = *180/3.14

i2c = I2C(0, sda=Pin(2), scl=Pin(15), freq=400000)
# 
lis1 = LIS3DH_I2C(i2c, address=0x18)
lis2 = LIS3DH_I2C(i2c, address=0x19)
# 
while True:
    sleep(0.1)
    a1=lis1.acceleration
    a2=lis2.acceleration
    # 计算方法1
    pitch1 = atan(a1.x/ sqrt(a1.y * a1.y + a1.z * a1.z))* 57.3
    pitch2 = atan(a2.x/ sqrt(a2.y * a2.y + a2.z * a2.z))* 57.3
    roll1 = atan(a1.y/ sqrt(a1.x * a1.x + a1.z * a1.z))* 57.3
    roll2 = atan(a2.y/ sqrt(a2.x * a2.x + a2.z * a2.z))* 57.3
    # 计算方法2
    _pitch1 = atan2(a1.x,a1.z)* 57.3
    _pitch2 = atan2(a2.x,a2.z)* 57.3
    _roll1 = atan2(a1.y,a1.z)* 57.3
    _roll2 = atan2(a2.y,a2.z)* 57.3
    print(a1.x,a1.y,a1.z,pitch1,roll1,_pitch1,_roll1,
          a2.x,a2.y,a2.z,pitch2,roll2,_pitch2,_roll2)
    print(pitch1-pitch2,_pitch1-_pitch2)
    
    

    
