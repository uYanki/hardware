import serial
from dearpygui import demo
from dearpygui import dearpygui as dpg

from PyQt5.QtSerialPort import QSerialPortInfo
portname = QSerialPortInfo.availablePorts()[0].portName()
# portname = "COM9"

uart = serial.Serial(portname, baudrate=115200, bytesize=8, stopbits=1, timeout=1)

data_x = []
data_y = []

dpg.create_context()
dpg.create_viewport(title="fft serial", width=600, height=400)
with dpg.window(tag="MainWindow"):
    with dpg.plot(label=" Series", height=-1, width=-1, no_menus=True):
        dpg.add_plot_axis(dpg.mvXAxis, tag="x-axis")
        with dpg.plot_axis(dpg.mvYAxis, tag="y-axis"):
            dpg.add_line_series(data_x, data_y, tag="chart")
        dpg.set_axis_limits("y-axis", ymin=-100, ymax=6000)
dpg.set_primary_window("MainWindow", True)
dpg.setup_dearpygui()
dpg.show_viewport()


from scipy import fftpack
import numpy as np

sampling_freq = 38400  # 采样频率
chunk_size = 32  # 块大小

'''
注: 串口的数据量过大会导致显示延迟. (在下位机里加延时)
'''

while dpg.is_dearpygui_running():
    try:

        current_data = np.zeros(chunk_size, dtype=int)
        for i in range(chunk_size):
            current_data[i] = uart.readline()

        freq_sig = np.abs(fftpack.fft(current_data) * 2 * np.pi / chunk_size)
        freqs = fftpack.fftfreq(len(current_data)) * sampling_freq

        _len = int(len(freqs) / 2)  # 截半
        _freq = freqs[:_len]
        _freq_sig = freq_sig[:_len]

        dpg.set_value("chart", [_freq, _freq_sig])
        dpg.set_axis_limits("x-axis", np.min(_freq), np.max(_freq) + 2)

    except Exception as e:
        print(e)

    dpg.render_dearpygui_frame()

dpg.destroy_context()
