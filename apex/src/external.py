TEST_MODE = True
if TEST_MODE:
    from unittest.mock import Mock  # Mock objects can do anything
    
    I2C = Mock()
    UART = Mock()
    GPS_GtopI2C = Mock()
else:
    from busio import I2C, UART
    from adafruit_gps import GPS_GtopI2C

from time import sleep

sleep_ms = lambda m: sleep(m//1000)