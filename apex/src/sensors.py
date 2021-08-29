from time import monotonic_ns, sleep

from adafruit_dps310 import DPS310, Rate, Mode, SampleCount
from adafruit_bno08x.i2c import BNO08X_I2C
from adafruit_bno08x import (
    BNO_REPORT_ACCELEROMETER,
    BNO_REPORT_GYROSCOPE,
    BNO_REPORT_MAGNETOMETER,
    BNO_REPORT_LINEAR_ACCELERATION,
    BNO_REPORT_ROTATION_VECTOR,
    BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR,
)


def millis():
    return monotonic_ns() // 1_000_000


DATA_SIZE = 16  # Millis, Rot_i2, Rot_j2, Rot_k2, Rot_real2, Pressure, Temperature, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, mag_x, mag_y, mag_z
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
        self._bno.enable_feature(BNO_REPORT_ROTATION_VECTOR)
        self._bno.enable_feature(BNO_REPORT_ACCELEROMETER)
        self._bno.enable_feature(BNO_REPORT_GYROSCOPE)
        self._bno.enable_feature(BNO_REPORT_MAGNETOMETER)

    def _get_accelerometer(self):
        try:
            return self._bno.quaternion, self._bno.acceleration, self._bno.gyro, self._bno.magnetic
        except:
            return [float(ERROR)]*4, [float(ERROR)]*3, [float(ERROR)]*3, [float(ERROR)]*3

    def _get_barometer(self):
        try:
            return self._dps310.pressure, self._dps310.temperature
        except:
            return float(ERROR), float(ERROR)

    def calibrate(self):
        self._bno.begin_calibration()
        sleep(15)

    def get(self):
        quat, accel, gyro, mag = self._get_accelerometer()
        pressure, temperature = self._get_barometer()
        self._data[0] = millis()
        self._data[1:5] = quat
        self._data[5] = pressure
        self._data[6] = temperature
        self._data[7:10] = accel
        self._data[10:13] = gyro
        self._data[13:16] = mag

        return self._data
