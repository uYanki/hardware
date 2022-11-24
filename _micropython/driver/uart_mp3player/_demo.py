from machine import UART, Pin
from utime import sleep_ms
from BNPlayer import Player

#创建控制MP3模块用的uart
uart=UART(1,baudrate=9600,bits=8,parity=None,stop=1,tx=25,rx=26)
#创建播放器对象
#参数序列为 uart 是否忙状态读入GPIO 音量(0.0~1.0)
player = Player(uart,Pin(27),0.8)
#设置音量
player.volume(1.0)
#休眠500毫秒(注意各个播放相关动作之间不能紧跟，至少需要休眠500ms)
sleep_ms(500)
#播放TF卡中的序号为2的乐曲
#序号从1开始数
#序号不是文件名，是文件在TF卡中的存储顺序
player.playSpec(2)
#休眠500毫秒(注意各个播放相关动作之间不能紧跟，至少需要休眠500ms)
sleep_ms(1000)
#打印当前是否忙（即是否正在播放）
print("is playing>",player.isBusy())
#休眠5000毫秒(注意各个播放相关动作之间不能紧跟，至少需要休眠500ms)
sleep_ms(5000)
#暂停播放
player.pause()
#休眠500毫秒(注意各个播放相关动作之间不能紧跟，至少需要休眠500ms)
sleep_ms(500)
#打印当前是否忙（即是否正在播放）
print("is playing>",player.isBusy())
#休眠5000毫秒(注意各个播放相关动作之间不能紧跟，至少需要休眠500ms)
sleep_ms(5000)
#继续播放
player.play()


            