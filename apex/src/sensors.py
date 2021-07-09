from external import (
    millis,
    BNO08X_I2C,
    DPS310,
    Rate,
    Mode,
    SampleCount,
    BNO_REPORT_LINEAR_ACCELERATION,
    BNO_REPORT_STEP_COUNTER,
    BNO_REPORT_ACTIVITY_CLASSIFIER,
    BNO_REPORT_ROTATION_VECTOR,
    BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR,
)

DATA_SIZE = 16 #Millis, Acc_x, Acc_y, Acc_z, Rot_i1, Rot_j1, Rot_k1, Rot_real1, Rot_i2, Rot_j2, Rot_k2, Rot_real2, Steps, Activity, Pressure, Temperature
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
        self.data[0] = millis()
        self.data[1:4] = self._bno.linear_acceleration
        self.data[4:8] = self._bno.quaternion
        self.data[8:12] = self._bno.geomagnetic_quaternion
        self.data[12] = self._bno.steps
        self.data[13] = 0 #TODO convert classification to index
        self.data[14] = self._dps310.pressure
        self.data[15] = self._dps310.temperature

        return self.data
