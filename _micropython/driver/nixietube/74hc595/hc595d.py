from machine import Pin

#字符编码列表
#          0    1	 2	  3	   4	5	 6	  7	   8	9	 A	  b    C    d	 E    F    -
#          0    1	 2	  3	   4	5	 6	  7	   8	9    10   11   12   13   14    15   16
charCode=[0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E,0xbf]

#基于74HC595D芯片驱动四位数码管显示的工具类
class HC595D(object):
    def __init__(self,sclk,rclk,dio):
        self.sclk = sclk
        self.rclk = rclk
        self.dio = dio

        self.sclk.init(Pin.OUT)
        self.rclk.init(Pin.OUT)
        self.dio.init(Pin.OUT)
    
    #一个比特结束
    def oneBitFinish(self):
        self.sclk.value(0)
        self.sclk.value(1)
    
    #一个字节结束
    def oneByteFinish(self):    
        self.rclk.value(0)
        self.rclk.value(1)

    #输出一个字节的数据
    def outByte(self,X):
        for i in range(8):
            if(X&0x80):
                self.dio.value(1)
            else:
                self.dio.value(0)
            X<<=1;
            self.oneBitFinish()       

    #输出一位数字 
    def outOne(self,X,index):
        self.outByte(charCode[X])
        self.outByte(index)
        self.oneByteFinish()

    #输出四位数字
    def outFour(self,XList):
        i=8
        for X in XList:
          self.outOne(X,int(i))
          i=i/2

    #输出数值(十进制)
    def outNumber(self,num):
        XList=[0,0,0,0]
        for i in range(4):
            temp=num%10
            XList[int(3-i)]=int(temp)
            num=num/10
        self.outFour(XList)
        
    #输出数值(十六进制)
    def outNumberHEX(self,num):
        XList=[0,0,0,0]
        for i in range(4):
            temp=num%16
            XList[int(3-i)]=int(temp)
            num=num/16
        self.outFour(XList)        



