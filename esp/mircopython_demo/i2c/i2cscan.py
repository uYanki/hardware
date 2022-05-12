from machine import Pin, I2C

i2c = I2C(0, sda=Pin(23), scl=Pin(22), freq=400000)
print('Scan i2c bus...')
devices = i2c.scan()
if len(devices) == 0:
    print("No i2c device !")
else:
    print('i2c devices found:', len(devices))
for device in devices:
    print("Decimal address: ", device, " | Hexa address: ", hex(device))
