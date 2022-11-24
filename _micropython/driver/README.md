### 如何使用

默认启动文件：

- esp -> boot.py

- rp2 -> main.py

使用 `Thonny` 进行 `micropython` 开发

`docs`:  https://docs.micropython.org/en/latest/library/index.html

`fireware`:  https://micropython.org/download/

`lib`: https://pypi.org/search/?q=  (搜索相应库即可)

* 查看支持的模块

help("modules")

* 查看函数列表

import machine

print(dir(machine))

* 关闭串口输出

在串口输出时，不能保存文件，点击停止，然后在命令行里按 ctrtl + c

* 注释 / 取消注释

注释: alt+3. 取消注释: alt+4

### 上传文件到 mircopython 设备中

1. 安装 python 库：pip install adafruit-ampy
2. 运行命令行，上传文件：python ampy --port [COM] put [filepath]
- eg: ampy --port COM4 put README.txt

### 资料代码来源

module from (部分模块资料来源):

* https://shop111496966.taobao.com/
* https://shop58494233.taobao.com/
* https://www.semiee.com/

code from (部分代码来源):

* https://github.com/wyf97402/-MicroPython-ESP32-
* http://www.86x.org/en/latet/esp32/quickref.html

