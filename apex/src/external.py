TEST_MODE = True
if TEST_MODE:
    from unittest.mock import Mock  # Mock objects can do anything
    from time import sleep

    UART = Mock()

    def sleep_ms(x):
        return sleep(x/1000)
else:
    from machine import UART, Pin
    from utime import sleep_ms
