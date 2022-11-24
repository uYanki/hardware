from machine import Pin
from machine import DAC
import utime
from WaveGenUtil import WaveGenerator

# 波形发生器

dac = DAC(Pin(25))
wg = WaveGenerator(dac)
# wg.jcbGen()
# wg.triGen()
wg.sinGen()
