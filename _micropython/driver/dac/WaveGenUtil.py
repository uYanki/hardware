import utime
import math

class WaveGenerator(object):
    def __init__(self,dacObj):
        self.dacObj=dacObj
        self.workFlag=True        

    #发生锯齿波形
    def jcbGen(self):
       while self.workFlag:
        for i in range(128):        
            self.dacObj.write(i*2)
            
    #发生三角波形
    def triGen(self):
       while self.workFlag:
        for i in range(64):        
            self.dacObj.write(i*4)
        for i in range(1,64):        
            self.dacObj.write((63-i)*4)
    
    #发生正弦波形
    def sinGen(self):
       while self.workFlag:
           for i in range(64):
               curr=math.sin(i*2*math.pi/64)*0.5+0.5
               self.dacObj.write(int(curr*255))

            
            
            
            
