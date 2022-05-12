from sht30 import SHT30
from time import sleep

sht = SHT30(scl_pin=3, sda_pin=2, i2c_address=0x44)

while True:
    print(sht.measure())
    sleep(0.3)
