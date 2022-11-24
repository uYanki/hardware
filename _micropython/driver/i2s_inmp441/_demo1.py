# wav recorder
# 实现功能: 录音到sd卡

from machine import I2S, SPI
from machine import Pin
from WAVSaveUitl import *
import sdcard
import os
import utime

# 音频文件采样率
sampleRate = 8000
# 每音频采样比特数
bitsPerSample = 16
# 缓冲字节数
bufSize = 32768
# 一次录音数据的总字节数
datasize = bufSize*4

# I2S所需管脚
# 数据时钟(某宝INMP441模块SCK)
sck_pin = Pin(23)
# 帧时钟(某宝INMP441模块模块WS)
ws_pin = Pin(22)
# 数据(某宝INMP441模块模块SD)
sd_pin = Pin(21)

# 创建用于音频录制的I2S对象
audioInI2S = I2S(0,
                 sck=sck_pin, ws=ws_pin, sd=sd_pin,
                 mode=I2S.RX,
                 bits=bitsPerSample,
                 format=I2S.STEREO,
                 rate=sampleRate,
                 ibuf=bufSize)
# 音频数据读取缓冲
readBuf = bytearray(bufSize)

# 初始化SD卡访问相关========================
# SPI 1的使能端口
cs = Pin(14, Pin.OUT)
# 使用SPI
spi = SPI(1, baudrate=8000000, sck=Pin(25), mosi=Pin(26), miso=Pin(27))
# 创建SDCard对象
sd = sdcard.SDCard(spi, cs)
# 将SDCard对象映射为VfsFat文件对象
vfs = os.VfsFat(sd)
# 将VfsFat文件对象挂载到指定路径
os.mount(vfs, '/esp32')
# 切换工作目录
os.chdir('/esp32')
# 初始化SD卡访问相关========================

# 创建音频文件的文件头
headData = createWavHeader(sampleRate, bitsPerSample, 2, datasize)
# 打开要录制的音频文件
fOut = open("aaa.wav", "wb")
# 写出文件头数据
fOut.write(headData)
print("ready.......")
# 休眠一点时间
utime.sleep(2.0)

# 音频数据字节计数器
byteCount = 0
# 循环录制音频
while True:
    # 读取音频数据
    currByteCount = audioInI2S.readinto(readBuf)
    # 将音频数据写出到文件
    fOut.write(readBuf)
    # 更新音频数据字节计数器值
    byteCount = byteCount+currByteCount
    # 若数据量到达预设
    if (byteCount >= datasize):
        # 退出循环
        break
# 关闭文件
fOut.close()
# 停止I2S工作
audioInI2S.deinit()
print("finish.... ")
