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
import pwmio
from led import LED
from speaker import Speaker
import time

led = LED()
led.colour(255, 255, 255)
led.on()

speaker = Speaker()

class state:
    def run(self):
        raise NotImplementedError("Not implemented")

#FIXME these pins might be wrong
i2c = I2C(scl = board.GP27, sda = board.GP26, frequency = 100000, timeout = 100000)

_sms = sms(rx = board.GP5, tx = board.GP4)
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
        wait = 0
        while True:
            resp = _sms.recv_msg()
            _gps.update()

            if resp == "siren":
                speaker.siren()
            
            elif resp == "location":
                msg = _gps.create_msg()
                _sms.send_msg(msg)

            time.sleep(0.5)

        