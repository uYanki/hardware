from micropython import const
import time

READ = const(0x03)
FAST_READ = const(0x0B)


class gt20l:
    def __init__(self, spi, cs):
        self.cs = cs
        self.spi = spi

    def get_font(self, addr, byte):  # 通过地址返回字模
        self.cs.value(1)
        self.cs.value(0)
        time.sleep_ms(10)
        self.spi.write(bytearray([READ]))
        self.spi.write(bytearray([addr >> 16]))
        self.spi.write(bytearray([addr >> 8 & 0xFF]))
        self.spi.write(bytearray([addr & 0xFF]))
        data = self.spi.read(byte)
        self.cs.value(1)
        data = list(map(lambda x: hex(x), list(data)))
        return data

    def get_gb2312_font(self, gb):  # 获取gb2312中文字模
        BaseAdd = 0
        if gb[0] == 0xA9 and gb[1] >= 0xA1:
            addr = (282+(LSB-0xA1))*32+BaseAdd
        elif gb[0] >= 0xA1 and gb[0] <= 0xA3 and gb[1] >= 0xA1:
            addr = ((gb[0]-0xA1)*94+(gb[1]-0xA1))*32+BaseAdd
        elif gb[0] >= 0xB0 and gb[0] <= 0xF7 and gb[1] >= 0xA1:
            addr = ((gb[0]-0xB0)*94+(gb[1]-0xA1)+846)*32+BaseAdd
        return self.get_font(addr, 32)

    def get_gb2312_efont(self, gb):  # 获取gb2312国标扩展字符字模
        BaseAdd = 0x3b7d0
        # FontCode=gb[0]<<8|gb[1]
        FontCode = gb
        if FontCode >= 0xAAA1 and FontCode <= 0xAAFE:
            addr = (FontCode-0xAAA1)*16+BaseAdd
        elif FontCode >= 0xABA1 and FontCode <= 0xABC0:
            addr = (FontCode-0xABA1+95)*16+BaseAdd
        return self.get_font(addr, 16)

    def get_57ascll(self, let):  # 获取5x7ascll字模
        BaseAdd = 0x3bfc0
        ASCIICode = ord(let)
        if ASCIICode >= 0x20 and ASCIICode <= 0x7E:
            addr = (ASCIICode-0x20)*8+BaseAdd
        return self.get_font(addr, 8)

    def get_78ascll(self, let):  # 获取7x8ascll字模
        BaseAdd = 0x66c0
        ASCIICode = ord(let)
        if ASCIICode >= 0x20 and ASCIICode <= 0x7E:
            addr = (ASCIICode-0x20)*8+BaseAdd
        return self.get_font(addr, 8)

    def get_816ascll(self, let):  # 获取8x16ascll字模
        BaseAdd = 0x3b7c0
        ASCIICode = ord(let)
        if ASCIICode >= 0x20 and ASCIICode <= 0x7E:
            addr = (ASCIICode-0x20)*16+BaseAdd
        return self.get_font(addr, 16)

    def get_16Arial_ascll(self, let):  # 获取16点阵不等宽ascll字模
        BaseAdd = 0x3c2c0
        ASCIICode = ord(let)
        if ASCIICode >= 0x20 and ASCIICode <= 0x7E:
            addr = (ASCIICode-0x20)*34+BaseAdd
        return self.get_font(addr, 34)

    def get_816bold_ascll(self, let):  # 获取8x16粗体ascll字模
        BaseAdd = 0x3cf80
        ASCIICode = ord(let)
        if ASCIICode >= 0x20 and ASCIICode <= 0x7E:
            addr = (ASCIICode-0x20)*16+BaseAdd
        return self.get_font(addr, 16)

    def get_TimesNewRoman_ascll(self, let):  # 获取16点阵白头不等宽ascll字模
        BaseAdd = 0x3d580
        ASCIICode = ord(let)
        if ASCIICode >= 0x20 and ASCIICode <= 0x7E:
            addr = (ASCIICode-0x20)*34+BaseAdd
        return self.get_font(addr, 34)
