from adafruit_dps310.advanced import DPS310, SampleCount, Rate, Mode
import busio
from board import *
import time

i2c = busio.I2C(SCL1, SDA1)

dps310 = DPS310(i2c)
dps310.pressure_oversample_count = SampleCount.COUNT_2
dps310.pressure_rate = Rate.RATE_16_HZ
dps310.temperature_oversample_count = SampleCount.COUNT_16
dps310.temperature_rate = Rate.RATE_16_HZ
dps310.mode = Mode.CONT_PRESTEMP

while True: 
    print("Temperature = %.2f *C"%dps310.temperature)
    print("Pressure = %.2f hPa"%dps310.pressure)
    print("")
    time.sleep(0.01)

i2c.deinit()