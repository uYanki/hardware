from machine import Pin
import ssd1306


def drawTask(oled):
    # 清空屏幕
    oled.fill(1)
    oled.show()


# IIC接口的两个GPIO口
sda = Pin(25)
scl = Pin(26)
# 创建IIC接口对象
i2c = machine.I2C(0, sda=sda, scl=scl, freq=400000)

# 选择第1个连接了OLED屏幕的通道
# 这里选择的是TCA9548A的0号通道
# 0——0000 0001
i2c.writeto(0x70, b'\x01')
# 初始化这个Oled屏幕
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# 选择第2个连接了OLED屏幕的通道
# 这里选择的是TCA9548A的1号通道
# 0——0000 0010
i2c.writeto(0x70, b'\x02')
# 初始化这个Oled屏幕
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# 选择第3个连接了OLED屏幕的通道
# 这里选择的是TCA9548A的2号通道
# 0——0000 0100
i2c.writeto(0x70, b'\x04')
# 初始化这个Oled屏幕
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# 关闭TCA9548A的所有通道
#i2c.writeto(0x70, b'\x00')

# 启用TCA9548A的0号通道
i2c.writeto(0x70, b'\x01')
# 绘制画面
drawTask(oled)

# 启用TCA9548A的1号通道
i2c.writeto(0x70, b'\x02')
# 绘制画面
drawTask(oled)

# 启用TCA9548A的3号通道
i2c.writeto(0x70, b'\x04')
# 绘制画面
drawTask(oled)
