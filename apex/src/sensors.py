from time import monotonic_ns

from adafruit_dps310 import DPS310, Rate, Mode, SampleCount
from adafruit_bno08x.i2c import BNO08X_I2C
from adafruit_bno08x import (
    BNO_REPORT_ACCELEROMETER,
    BNO_REPORT_LINEAR_ACCELERATION,
    BNO_REPORT_ROTATION_VECTOR,
    BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR,
)


def millis():
    return monotonic_ns() // 1_000_000


DATA_SIZE = 17 # Millis, Lin_acc_x, Lin_acc_y, Lin_acc_z, acc_x, acc_y, acc_z, Rot_i1, Rot_j1, Rot_k1, Rot_real1, Rot_i2, Rot_j2, Rot_k2, Rot_real2, Pressure, Temperature
ERROR = 0XFFFFFFFF

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
        self._bno.enable_feature(BNO_REPORT_ACCELEROMETER)
        self._bno.enable_feature(BNO_REPORT_ROTATION_VECTOR)
        self._bno.enable_feature(BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR)

    def _get_accelerometer(self):
        try:
            return self._bno.linear_acceleration, self._bno.acceleration, self._bno.quaternion, self._bno.geomagnetic_quaternion
        except:
            return float(ERROR), float(ERROR), float(ERROR), float(ERROR)

    def _get_barometer(self):
        try:
            return self._dps310.pressure, self._dps310.temperature
        except:
            return float(ERROR), float(ERROR)

    def get(self):
        accel, lin_accel, quat, geo_quat = self._get_accelerometer()
        pressure, temperature = self._get_barometer()
        self._data[0] = millis()
        self._data[1:4] = lin_accel
        self._data[4:7] = accel
        self._data[7:11] = quat
        self._data[11:15] = geo_quat
        self._data[15] = pressure
        self._data[16] = temperature

        return self._data
