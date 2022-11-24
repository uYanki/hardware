from machine import UART, Pin
import utime

# 创建UART
uart = UART(1, baudrate=921600, bits=8, parity=None, stop=1, tx=25, rx=26)

# 不断循环收发数据
while True:
    # 声明一个字符串
    s = "Hello PC!\n"
    # 将字符串编码并发送
    uart.write(s.encode('utf-8'))
    # 获取接收到的数据的字节数
    count = uart.any()
    # 如果需要接收的字节数大于0
    if count > 0:
        # 读取数据字节序列
        recByte = uart.read(count)
        # 打印数据字节序列
        print(recByte)
    # 休眠
    utime.sleep(1)
