from machine import I2S
from machine import Pin

# I2S所需管脚
# 数据时钟(BCLK)
sck_pin = Pin(21)
# 帧时钟(LRC) Left/Right Clock
ws_pin = Pin(22)
# 数据(DIN)
sd_pin = Pin(27)


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
    # 设置为单声道
    format=I2S.MONO,  # 单声道 MONO, 双声道 STEREO
    # 采样率
    rate=16000,
    # 内部缓存字节数
    ibuf=4096)

# 打开音频文件
wavFile = open('test_fl.wav', 'rb')

# 跳转到第一个音频数据字节
pos = wavFile.seek(44)

# 音频数据缓冲
wavSamplesBuf = bytearray(4096)
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
