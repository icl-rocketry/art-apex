from time import sleep
TEST_MODE = True
if TEST_MODE:
    from unittest.mock import Mock  # Mock objects can do anything
    from time import time_ns

    I2C = Mock()
    UART = Mock()
    GPS_GtopI2C = Mock()
    BNO08X_I2C = Mock()
    DPS310 = Mock()
    Rate = Mock()
    Mode = Mock()
    SampleCount = Mock()

    BNO_REPORT_LINEAR_ACCELERATION = Mock()
    BNO_REPORT_STEP_COUNTER = Mock()
    BNO_REPORT_ACTIVITY_CLASSIFIER = Mock()
    BNO_REPORT_ROTATION_VECTOR = Mock()
    BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR = Mock()

    def millis(): 
        return time_ns() // 1_000_000


else:
    from busio import I2C, UART
    from adafruit_gps import GPS_GtopI2C
    from adafruit_bno08x.i2c import (
        BNO08X_I2C,
        BNO_REPORT_LINEAR_ACCELERATION,
        BNO_REPORT_STEP_COUNTER,
        BNO_REPORT_ACTIVITY_CLASSIFIER,
        BNO_REPORT_ROTATION_VECTOR,
        BNO_REPORT_GEOMAGNETIC_ROTATION_VECTOR,
    )
    from adafruit_dps310 import DPS310, Rate, Mode, SampleCount
    from time import monotonic_ns

    def millis(): return monotonic_ns() // 1_000_000


def sleep_ms(m): return sleep(m//1000)
