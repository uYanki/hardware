from machine import Pin
import hc595d
import utime

#定义管脚变量
SCLK=Pin(25)
RCLK=Pin(26)
DIO=Pin(27)

#每个数值循环显示的次数
COUNT=100


#初始化
smg=hc595d.HC595D(SCLK,RCLK,DIO)

#循环显示0000-9999
while True:
    #当前要显示的数字    
    numberStart=0
    #遍历0000-9999
    for i in range(10000):
        #每显示一个数值要循环一段时间
        #不然太快了看不见
        #595显示数字靠的是快速扫描
        #不是真的四位一起亮
        for j in range(COUNT):
            smg.outNumber(i)

# #循环显示0000-FFFF
# while True:
#     #当前要显示的数字    
#     numberStart=0
#     #遍历0000-FFFF即0000-65535
#     for i in range(65536):
#         #每显示一个数值要循环一段时间
#         #不然太快了看不见
#         #595显示数字靠的是快速扫描
#         #不是真的四位一起亮
#         for j in range(COUNT):
#             smg.outNumberHEX(i)
#         #直接用休眠是不行的
#         #utime.sleep(0.2)
    
