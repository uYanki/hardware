# https://github.com/hoffstadt/DearPyGui/tree/master/docs/source/documentation

import serial
from dearpygui import demo
from dearpygui import dearpygui as dpg

from PyQt5.QtSerialPort import QSerialPortInfo
portname = QSerialPortInfo.availablePorts()[0].portName()
# portname = "COM9"
uart = serial.Serial(portname, baudrate=115200, bytesize=8, stopbits=1, timeout=1)


data_x = []
data_y = []

show_point_num = 400  # 显示的点数
store_num = show_point_num * 2.1  # 缓冲点数大小


def cfg_show(sender, app_data, user_data):
    global show_point_num, store_num  # 指定使用全局变量
    show_point_num = app_data
    store_num = show_point_num * 2.1


dpg.create_context()
dpg.create_viewport(title="serial", width=600, height=400)
with dpg.window(tag="MainWindow"):
    with dpg.plot(label=" Series", height=-1, width=-1, no_menus=True):
        with dpg.popup(dpg.last_item(), min_size=(0, 0)):
            dpg.add_text("count of points")
            dpg.add_slider_int(min_value=100, max_value=10000, default_value=show_point_num, callback=cfg_show)
        dpg.add_plot_axis(dpg.mvXAxis, tag="x-axis")
        with dpg.plot_axis(dpg.mvYAxis, tag="y-axis"):
            dpg.add_line_series(data_x, data_y, tag="chart")
        dpg.set_axis_limits("y-axis", ymin=-2, ymax=2)

dpg.set_primary_window("MainWindow", True)
dpg.setup_dearpygui()
dpg.show_viewport()

cnt = 0  # 总共采集的点数
cnt_buf = 0  # 缓冲区的点数

import numpy as np
# dpg.start_dearpygui()
while dpg.is_dearpygui_running():
    try:
        line = uart.readline()[:-2]  # 行读取 & 取换行符
        line = line.decode('utf-8', errors='ignore')  # 对字节数组进行解码

        if cnt_buf > store_num:  # 防list过大导致程序崩溃, 就进行了切片处理
            cnt_buf = show_point_num
            data_x = data_x[show_point_num:]
            data_y = data_y[show_point_num:]

        # 添加数据
        for i,v in enumerate(line.split(',')):
            data_x.append(cnt)
            data_y.append(int(v,10)/32678)
            cnt +=1 

        # 更新图表
        dpg.set_value("chart", [data_x, data_y])
        dpg.set_axis_limits("x-axis", cnt - show_point_num, cnt)

        cnt += 1
        cnt_buf += 1

    except Exception as e:
        print("Exception", e)

    dpg.render_dearpygui_frame()

dpg.destroy_context()
