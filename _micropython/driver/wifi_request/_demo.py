# 功能：获取网络时间

import network
import machine
from machine import Pin
import urequests
import utime
import ujson

# 从JSON字符串中提取时间戳
# 注意由于计算精度问题时间戳最后3位不要


def getInfoFromJSONStr(jsonStr):
    jo = ujson.loads(jsonStr)
    timeStamp = jo["data"]
    return int(timeStamp["t"][:-3])  # 后3为是毫秒, 舍去掉

# 收数字转为字符串
# 若数字不足两位补前导0


def fromNumToStr(n):
    result = str(n)
    if len(result) < 2:
        result = "0"+result
    return result

# 将时间元组转换为时间字符串
# 注意年份和时区自定义调整


def fromTurpleToTimeStr(tup):
    return (str(tup[0]-30)+"-"+fromNumToStr(tup[1])+"-"+fromNumToStr(tup[2]),
            fromNumToStr(tup[3]+8)+":"+fromNumToStr(tup[4])+":"+fromNumToStr(tup[5]))


# 连接网络
station = network.WLAN(network.STA_IF)
while (not station.isconnected()):
    station.active(True)
    try:
        station.connect("HUAWEI-Y6AZGD", "password")
        utime.sleep(5.0)
    except OSError:
        print("OSError")
        utime.sleep(2.0)

# 获取时间戳JSON
response = urequests.get(
    "http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp")
# 将从JSON中提取的时间戳解析为日期时间信息元组
timeTup = utime.localtime(getInfoFromJSONStr(response.text))
# 基于日期时间信息元组生成显示所需的日期、时间字符串元组
timeNr = fromTurpleToTimeStr(timeTup)

print(timeNr)
