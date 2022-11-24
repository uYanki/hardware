import machine
import math
import time
import sys


class hmc5883l():

    __scales = {
        0.88: [0, 0.73],
        1.30: [1, 0.92],
        1.90: [2, 1.22],
        2.50: [3, 1.52],
        4.00: [4, 2.27],
        4.70: [5, 2.56],
        5.60: [6, 3.03],
        8.10: [7, 4.35],
    }

    def __init__(self, address=0x1E, gauss=1.3, declination=(0,0)):
        self.bus =  machine.SoftI2C(scl=machine.Pin(22), sda=machine.Pin(21))
        self.address = address

        (degrees, minutes) = declination
        self.__declDegrees = degrees
        self.__declMinutes = minutes
        self.__declination = (degrees + minutes / 60) * math.pi / 180

        (reg, self.__scale) = self.__scales[gauss]
        self.bus.writeto_mem(self.address, 0x00, bytes([0x70])) # 8 Average, 15 Hz, normal measurement
        self.bus.writeto_mem(self.address, 0x01, bytes([reg << 5])) # Scale
        self.bus.writeto_mem(self.address, 0x02, bytes([0x00])) # Continuous measurement

    def declination(self): # 磁偏角
        return (self.__declDegrees, self.__declMinutes)

    def twos_complement(self, val, len):
        # Convert twos compliment to integer
        if (val & (1 << len - 1)):
            val = val - (1<<len)
        return val

    def axes(self):
        xmsb,xlsb,zmsb,zlsb,ymsb,ylsb = self.bus.readfrom_mem(self.address, 0x03,6)

        x = self.twos_complement(xlsb + (xmsb << 8),16)
        y = self.twos_complement(ylsb + (ymsb << 8),16)
        z = self.twos_complement(zlsb + (zmsb << 8),16)
        
        return (x,y,z)

    def heading(self): # 朝向
        (x, y, z) = self.axes()
        headingRad = math.atan2(y, x)
        headingRad += self.__declination

        # Correct for reversed heading
        if headingRad < 0:
            headingRad += 2 * math.pi

        # Check for wrap and compensate
        elif headingRad > 2 * math.pi:
            headingRad -= 2 * math.pi

        # Convert to degrees from radians
        headingDeg = headingRad * 180 / math.pi
        return headingDeg

    def degrees(self, headingDeg):
        degrees = math.floor(headingDeg)
        minutes = round((headingDeg - degrees) * 60)
        return (degrees, minutes)

    def __str__(self):
        (x, y, z) = self.axes()
        return "Axis X: " + str(x) + "\n" \
               "Axis Y: " + str(y) + "\n" \
               "Axis Z: " + str(z) + "\n" 


compass = hmc5883l(gauss = 4.7, declination = (-2,5))

while True:
#     print(compass)
#     print(compass.heading())  指和手机的指南针相近
    sys.stdout.write("\rHeading: " + str(compass.degrees(compass.heading())))
    time.sleep(0.5)
