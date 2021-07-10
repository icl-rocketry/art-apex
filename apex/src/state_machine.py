#The state machine has 3 states:
# 1. Preflight - calibrates all sensors, and waits for a text message containing the message "launch".
# 2. Flight - takes readings from sensors for FLIGHT_TIME minutes.
# 3. Postflight - takes gps readings and sends them to GROUND_CONTROL_PHONE, until a "Received" is received.
#                 It also plays sounds over the speakers, in case the earlier method failed

from sensors import sensors
import board
from gps import gps
from sms import sms
from external import sleep_ms
import struct

class state:
    def run(self):
        raise NotImplementedError("Not implemented")

#FIXME these pins might be wrong
i2c = I2C(sda=board.GP2, scl=board.GP3, timeout=10000)

_sms = sms(0, 0)
_sensors = sensors(i2c)
_gps = gps(i2c, 1000)
class preflight(state):
    def run(self):
        while "launch" not in _sms.recv_msg():
            sleep_ms(500)
        _sms.send_msg("launching")
        return flight()

class flight(state):

    #TODO optimise buffer size such that data can be flushed in the same amount of time that would be
    #spent sleeping - Nasty hack to get uasyncio to work in circuitpython
    _recordings_before_flush = 3
    _buffer_size = _recordings_before_flush * sensors.data_size
    _capture_rate = 20
    _delay = 1000//_capture_rate
    _flight_time = 5 * 60 * _capture_rate #5 minutes of flight

    def __init__(self):
        self._storage = open("log.bin", "wb", buffering=self._buffer_size)

    def run(self):
        i = 0
        while i < self._flight_time:
            i += 1
            data = _sensors.get()
            for reading in data:
                self._storage.write(struct.pack("f", reading))

            if i % self._recordings_before_flush != 0:
                sleep_ms(self._delay)
        return postflight()

class postflight(state):
    def run(self):
        resp = ""
        while "Received" not in resp:
            location = _gps.get_long_lat()
            _sms.send_msg(str(location))
            resp = _sms.recv_msg()
        