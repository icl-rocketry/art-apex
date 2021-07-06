TEST_MODE = True
if TEST_MODE:
    from unittest.mock import Mock  # Mock objects can do anything
    from time import time_ns
    I2C = Mock()
    UART = Mock()
    GPS_GtopI2C = Mock()
    millis = lambda: time_ns() // 1_000_000
else:
    from busio import I2C, UART
    from adafruit_gps import GPS_GtopI2C
    from time import monotonic_ns
    millis = lambda: monotonic_ns() // 1_000_000


from time import sleep

sleep_ms = lambda m: sleep(m//1000)