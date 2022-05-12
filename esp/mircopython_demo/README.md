使用 `Thonny` 进行 `micropython` 开发



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