from time import monotonic_ns

from adafruit_dps310 import DPS310, Rate, Mode, SampleCount
from adafruit_bno08x.i2c import BNO08X_I2C
from adafruit_bno055 import (
    BNO_REPORT_LINEAR_ACCELERATION,
    BNO_REPORT_STEP_COUNTER,
    BNO_REPORT_ACTIVITY_CLASSIFIER,
    BNO_REPORT_ROTATION_VECTOR,
    BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR,
)


def millis():
    return monotonic_ns() // 1_000_000


DATA_SIZE = 16  # Millis, Acc_x, Acc_y, Acc_z, Rot_i1, Rot_j1, Rot_k1, Rot_real1, Rot_i2, Rot_j2, Rot_k2, Rot_real2, Steps, Activity, Pressure, Temperature


class sensors:

    data_size = DATA_SIZE
    _data = [float()] * DATA_SIZE
    _start = millis()

    # All sensors will share the same i2c bus
    def __init__(self, i2c):
        # Initialise barometer - TODO tweak the rate and samplecounts
        self._dps310 = DPS310(i2c)
        self._dps310.reset()
        # 2 samples per pressure measurement
        self._dps310.pressure_oversample_count = SampleCount.COUNT_2
        self._dps310.pressure_rate = Rate.RATE_16_HZ
        self._dps310.temperature_oversample_count = SampleCount.COUNT_16
        self._dps310.temperature_rate = Rate.RATE_16_HZ
        self._dps310.mode = Mode.CONT_PRESTEMP
        self._dps310.wait_temperature_ready()
        self._dps310.wait_pressure_ready()

        # Initialise imu
        self._bno = BNO08X_I2C(i2c)
        self._bno.enable_feature(BNO_REPORT_LINEAR_ACCELERATION)
        self._bno.enable_feature(BNO_REPORT_ROTATION_VECTOR)
        self._bno.enable_feature(BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR)
        self._bno.enable_feature(BNO_REPORT_ACTIVITY_CLASSIFIER)
        self._bno.enable_feature(BNO_REPORT_STEP_COUNTER)

    def get(self):
        self._data[0] = millis()
        self._data[1:4] = self._bno.linear_acceleration
        self._data[4:8] = self._bno.quaternion
        self._data[8:12] = self._bno.geomagnetic_quaternion
        self._data[12] = self._bno.steps
        self._data[13] = 0  # TODO convert classification to index
        self._data[14] = self._dps310.pressure
        self._data[15] = self._dps310.temperature

        return self._data
