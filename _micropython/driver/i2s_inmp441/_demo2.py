# wav player
# 实现功能: 播放sd卡内的音频（_demo1.py 采集到的音频）

from machine import I2S, SPI
from machine import Pin
import sdcard
import os
import utime

# I2S所需管脚
# 数据时钟(BCLK)
sck_pin = Pin(21)
# 帧时钟(LRC)
ws_pin = Pin(22)
# 数据(DIN)
sd_pin = Pin(23)

# 创建用于音频播放的I2S对象
audioOutI2S = I2S(
    # I2S编号
    1,
    # I2S所需管脚
    sck=sck_pin, ws=ws_pin, sd=sd_pin,
    # 设置为发送模式（对应于音频播放）
    mode=I2S.TX,
    # 采样深度（即每个采样的比特数）
    bits=16,
    # 设置为立体声
    format=I2S.STEREO,
    # 采样率
    rate=8000,
    # 内部缓存字节数
    ibuf=16384)

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

# 打开音频文件
wavFile = open('aaa.wav', 'rb')

# 跳转到第一个音频数据字节
pos = wavFile.seek(44)

# 音频数据缓冲
wavSamplesBuf = bytearray(16384)
# 创建对应的内存查看对象（用于减少主循环中的堆内存分配）
wavSamplesBufMV = memoryview(wavSamplesBuf)

# 播放主循环
while True:
    try:
        # 读取一批音频采样数据
        readCount = wavFile.readinto(wavSamplesBufMV)
        # 数据写出字节数量计数器
        writtenCount = 0
        # 如果音频文件结束
        if readCount == 0:
            # 跳转到第一个音频数据字节
            pos = wavFile.seek(44)
            # break
        else:
            # 如果当前音频数据缓冲中的数据还没有都输出
            while writtenCount < readCount:
                writtenCount += audioOutI2S.write(
                    wavSamplesBufMV[writtenCount:readCount])
    except (Exception) as e:
        print('caught exception {} {}'.format(type(e).__name__, e))
        break
# 关闭音频文件
wavFile.close()
# 释放I2S对象
audioOutI2S.deinit()
