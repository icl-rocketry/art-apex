from time import monotonic_ns
import struct
from adafruit_dps310.advanced import DPS310, Mode, Rate, SampleCount
import board
from busio import I2C


#Data structure:
# Time: uint32
# Pressure: float32
# Temperature: float32

_DATA_SIZE = 3 * 4

class Sensors:
    _bytes = bytearray(_DATA_SIZE)

    def __init__(self) -> None:
        i2c = I2C(sda=board.SDA1, scl=board.SCL1) #type: ignore
        self._dps = DPS310(i2c)
        self._dps.reset()
        self._dps.pressure_oversample_count = SampleCount.COUNT_4 #type: ignore
        self._dps.pressure_rate = Rate.RATE_64_HZ #type: ignore
        self._dps.temperature_oversample_count = SampleCount.COUNT_4 #type: ignore
        self._dps.temperature_rate = Rate.RATE_64_HZ #type: ignore
        self._dps.mode = Mode.CONT_PRESTEMP #type: ignore
      
        self._dps.initialize()
        self._dps.wait_pressure_ready()
        self._dps.wait_temperature_ready()


    def packData(self) -> bytearray:
        now = monotonic_ns() // 1_000_000 #To ms
        pressure = self._dps.pressure
        temperature = self._dps.temperature

        struct.pack_into("I", self._bytes, 0, now)
        struct.pack_into("f", self._bytes, 4, pressure)
        struct.pack_into("f", self._bytes, 8, temperature)

        return self._bytes
