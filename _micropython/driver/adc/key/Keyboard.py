
def keyCode(adc):  # 6个按键
    keyValue = adc.read_u16()
    print(keyValue)
    if keyValue <= 3000:
        # 标准值GND 0
        # print("UP")
        return 0
    elif keyValue >= 28000 and keyValue <= 33536:
        # 标准值 1/2 VCC 32768
        # print("LEFT")
        return 1
    elif keyValue >= 38000 and keyValue <= 42000:
        # 标准值 2/3 VCC43690
        # print("DOWN")
        return 2
    elif keyValue >= 43500 and keyValue <= 46500:
        # 标准值 3/4 VCC 49151
        # print("RIGHT")
        return 3
    elif keyValue >= 47500 and keyValue <= 50500:
        # 标准值 4/5 VCC 52428
        # print("A")
        return 4
    elif keyValue >= 51000 and keyValue <= 53500:
        # 标准值 5/6 VCC 54613
        # print("B")
        return 5
    return -1
