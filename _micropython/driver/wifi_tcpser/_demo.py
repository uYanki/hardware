import time
import socket
import network
from machine import Pin, ADC
from time import sleep

ad_rt = ADC(Pin(36))
ad_rp = ADC(Pin(39))

SSID = "TP-LINK_1DAA6A"
PASSWORD = "haha2021@@"
port = 6000


wlan = None
tcpser = None


def connectWifi(ssid, passwd):
    global wlan
    wlan = network.WLAN(network.STA_IF)  # create a wlan object
    wlan.active(True)  # Activate the network interface
    wlan.disconnect()  # Disconnect the last connected WiFi
    wlan.connect(ssid, passwd)  # connect wifi
    while (wlan.ifconfig()[0] == '0.0.0.0'):
        print('connecting WIFI')
        time.sleep(1)
    return True


# Catch exceptions,stop program if interrupted accidentally in the 'try'
try:
    connectWifi(SSID, PASSWORD)
    ip = wlan.ifconfig()[0]  # get ip addr
    tcpser = socket.socket()  # create socket
    tcpser.bind((ip, port))  # bind ip and port
    tcpser.listen(1)  # listen message
    # Set the value of the given socket option
    tcpser.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print(f'tcp server running: {ip}:{port}')

    while True:
        print("accepting.....")
        conn, addr = tcpser.accept()  # Accept a connection,conn is a new socket object
        print(addr, "connected")
        ret = conn.send('##{PLOT.CFG|RT:0;RP:0;}##\r\n')
        while True:
            #       data = conn.recv(1024)                #Receive 1024 byte of data from the socket
            #       if(len(data) == 0):
            #         print("close socket")
            #         conn.close()                        #if there is no data,close
            #         break
            #       print(data)
            ret = conn.send('##{PLOT.ADD|RT:'+f'{ad_rt.read_u16()}' +
                            ';RP:' + f'{ad_rp.read_u16()}'+';}##\r\n')  # send data
            sleep(0.2)
except:
    if (tcpser):
        tcpser.close()
    wlan.disconnect()
    wlan.active(False)
