from machine import Timer
import utime

#总次数
TOTAL_TIMES=10

#全局计数器
aCount=0
#定时任务对应的方法
def taskA(tim):
    global aCount
    aCount=aCount+1
    print('A Count>',aCount)
    if aCount==TOTAL_TIMES:
        tim.deinit()
        print('A Finish!')

#创建硬件定时器1对象
#ESP32中一共四个定时器可供选用
myTimerA=Timer(1)
#初始化定时器 
#Timer.ONE_SHOT  Timer.PERIODIC
myTimerA.init(period=600,mode=Timer.PERIODIC,callback=taskA)

#主程序中的其他任务
for i in range(20):
    print("Main ",i)
    utime.sleep(0.3)
    




