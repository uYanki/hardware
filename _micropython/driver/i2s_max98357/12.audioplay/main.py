from machine import I2S
from machine import Pin


"""
5V/3.3V -- VIN
"""

# 初始化引脚定义
sck_pin = Pin(12)  # 串行时钟输出 BCLK
ws_pin = Pin(14)  # 字时钟 LRC
sd_pin = Pin(13)  # 串行数据输出 DIN


"""
sck 是串行时钟线的引脚对象
ws 是单词选择行的引脚对象
sd 是串行数据线的引脚对象
mode 指定接收或发送
bits 指定样本大小（位），16 或 32
format 指定通道格式，STEREO（左右声道） 或 MONO(单声道)
rate 指定音频采样率（样本/秒）
ibuf 指定内部缓冲区长度（字节）
"""

# 初始化i2s
audio_out = I2S(1, sck=sck_pin, ws=ws_pin, sd=sd_pin, mode=I2S.TX, bits=16, format=I2S.MONO, rate=16000, ibuf=20000)


wavtempfile = "test.wav"
with open(wavtempfile, 'rb') as f:

    # 跳过文件的开头的44个字节，直到数据段的第1个字节
    pos = f.seek(44)

    # 用于减少while循环中堆分配的内存视图
    wav_samples = bytearray(1024)
    wav_samples_mv = memoryview(wav_samples)

    print("开始播放音频...")

    # 并将其写入I2S DAC
    while True:
        try:
            num_read = f.readinto(wav_samples_mv)

            # WAV文件结束
            if num_read == 0:
                break

            # 直到所有样本都写入I2S外围设备
            num_written = 0
            while num_written < num_read:
                num_written += audio_out.write(wav_samples_mv[num_written:num_read])

        except Exception as ret:
            print("产生异常...", ret)
            break
