from machine import UART, Pin
Start_Byte = 0x7E
Version_Byte = 0xFF
Command_Length = 0x06
Acknowledge = 0x00
End_Byte = 0xEF

def clamp(x, minimum, maximum):
    return max(minimum, min(x, maximum))

def split(num):
    return num >> 8, num & 0xFF

class Player():
    def __init__(self, uart, busy_pin,volume=1.0):
        self._volume = None
        self.uart = uart
        if busy_pin is not None:
            busy_pin.init(mode=Pin.IN, pull=Pin.PULL_UP)
        self.busy_pin = busy_pin
        if volume is not None:
            self.volume(volume)

    def command(self, CMD, Par1, Par2):
        Checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2)
        HighByte, LowByte = split(Checksum)
        CommandLine = bytes([b & 0xFF for b in [
            Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
            Par1, Par2, HighByte, LowByte, End_Byte
        ]])
        self.uart.write(CommandLine)

    #设置音量
    def volume(self, volume=None):
        if volume is None:
            return self._volume
        else:
            self._volume = float(clamp(volume, 0, 1.0))
            val = int(30.0 * self._volume)
            self.command(0x06,0 ,val)
            
    #播放指定序号曲目      
    def playSpec(self,trackNo):
        self.command(0x03, 0x00, trackNo)
    
    #循环播放指定序号曲目      
    def playSpecRepeat(self,trackNo):
        self.command(0x08,0x00,trackNo)
        
    #播放下一个曲目      
    def playNext(self):
        self.command(0x01,0x00,0x00)
        
    #播放上一个曲目      
    def playPrevious(self):
        self.command(0x02,0x00,0x00)
    
    #暂停      
    def pause(self):
        self.command(0x0E,0x00,0x00)
    
    #继续播放      
    def play(self):
        self.command(0x0D,0x00,0x00)
    
    #模块复位
    def reset(self):
        self.command(0x0C,0x00,0x00)
        
    #是否播放中
    def isBusy(self):
        return self.busy_pin.value() == 0

