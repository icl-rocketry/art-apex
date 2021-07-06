#The state machine has 3 states:
# 1. Preflight - calibrates all sensors, and waits for a text message containing the message "launch".
# 2. Flight - takes readings from sensors for FLIGHT_TIME minutes.
# 3. Postflight - takes gps readings and sends them to GROUND_CONTROL_PHONE, until a "Received" is received.
#                 It also plays sounds over the speakers, in case the earlier method failed

from sensors import sensors
from gps import gps
from sms import sms
from external import sleep_ms

class state:
    def run(self):
        raise NotImplementedError("Not implemented")

_sms = sms(0, 0)
_gps = gps(0, 0, 1000)
_sensors = sensors(0, 0)
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
            self._storage.write(data)

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
        