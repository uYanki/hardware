from machine import Pin, SPI
import sdcard2 as sdcard
import os

# 读取数据块的方法


def readDataBlocks(sd):
    # 读取缓冲区(1个区块的大小)
    buf = bytearray(512)
    # 读数据快
    sd.readblocks(0, buf)
    # 打印数据块中前64个字节的数据
    for i in range(0, 64):
        # 每行打印16个数据
        # 一行的一半给一个空格便于人眼观察
        if i % 16 == 8:
            print('  ', end='')
        # 如果是一行开始则打印本行首字节数据的
        # 4位16进制地址，4位地址结尾用“:”分隔
        if i % 16 == 0:
            print()
            print('{:04X}'.format(i), end=': ')
        # 打印一个字节的数据，包含两个16进制数
        print('{:02X}'.format(buf[i]), end=' ')
    print()


# SPI 1的使能端口
cs = Pin(14, Pin.OUT)
# 使用SPI
spi = SPI(1, baudrate=8000000, sck=Pin(25), mosi=Pin(26), miso=Pin(27))
# 创建SDCard对象
sd = sdcard.SDCard(spi, cs)
# 读取前512字节的数据块并打印
readDataBlocks(sd)
print("=======================================================")
# 将SDCard对象映射为VfsFat文件对象
vfs = os.VfsFat(sd)
# 将VfsFat文件对象挂载到指定路径
os.mount(vfs, '/esp32')
# 列出其中所有文件的名称
fl = os.listdir('/esp32')
print(fl)
# 切换工作目录
os.chdir('/esp32')
# 写入一个文件测试
fOut = open("sdtest.txt", "wt")
fOut.write("Hello Sdcard! ")
fOut.write("This is the msg from Pico!")
fOut.close()
# 列出其中所有文件的名称
fl = os.listdir('/esp32')
print(fl)
# 读入文件内容测试
fIn = open("sdtest.txt", "rt")
nr = fIn.readline()
print(nr)
fIn.close()
# 卸载前面挂载的文件
os.umount("/esp32")
