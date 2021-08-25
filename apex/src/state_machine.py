# The state machine has 3 states:
# 1. Preflight - calibrates all sensors, and waits for a text message containing the message "launch".
# 2. Flight - takes readings from sensors for FLIGHT_TIME minutes.
# 3. Postflight - takes gps readings and sends them to GROUND_CONTROL_PHONE, until a "Received" is received.
#                 It also plays sounds over the speakers, in case the earlier method failed

import struct
from time import sleep, monotonic_ns
import board
from busio import I2C

from sensors import sensors, millis
from gps import gps
from sms import sms
from led import LED
from speaker import Speaker

led = LED()
led.colour(255, 255, 255)
led.on()

speaker = Speaker()


def sleep_ms(ms):
    sleep(ms//1000)


# FIXME these pins might be wrong
i2c = I2C(scl=board.GP27, sda=board.GP26, timeout=10000000)

class state:
    def run(self):
        raise NotImplementedError("Not implemented")

class diagnostic(state):
    def run(self):
        sms_ = sms(rx=board.GP5, tx=board.GP4)
        sms_.report()
        sensors_ = sensors(i2c)
        sms_.send_msg("sensors ok")
        gps_ = gps(i2c, 1000)
        sms_.send_msg("gps ok")
        print("Done")
        led.colour(0, 255, 0)
        led.on()
        while sms_.recv_msg() != "ok":
            sleep_ms(1000)
            led.toggle()
        return preflight(sms_, sensors_, gps_)

class preflight(state):
    def __init__(self, sms, sensors, gps):
        self._sms = sms
        self._sensors = sensors
        self._gps = gps

    def run(self):
        led.colour(0, 0, 255)
        while self._sms.recv_msg() != "launch":
            sleep_ms(1000)
            led.on()
        self._sms.send_msg("launching")
        return flight(self._sms, self._sensors, self._gps)


class flight(state):
    # TODO optimise buffer size such that data can be flushed in the same amount of time that would be
    # spent sleeping
    _recordings_before_flush = 180
    _buffer_size = _recordings_before_flush * sensors.data_size
    _capture_rate = 20
    _delay = 1000//_capture_rate
    _flight_time = 5 * 60 * _capture_rate  # 5 minutes of flight

    def __init__(self, sms, sensors, gps):
        self._sms = sms
        self._sensors = sensors
        self._gps = gps
        self._sensor_storage = open("log.bin", "wb", buffering=self._buffer_size)
        self._gps_storage = open("gps.bin", "wb")

    def run(self):
        i = 0
        led.colour(255, 0, 0)

        capture_gps = False
        
        #Problem:
        #Sim module will need long (50ms+) waits to work
        #But we can't afford that in a single threaded environment
        #Solution:
        #Use pkt_wait to provide the sleeps
        pkt_wait = 1
        pkt = bytearray(24)
        self._sms.connect()

        while i < self._flight_time:
            pkt_wait -= 1
            i += 1
            start = millis()
            data = self._sensors.get()
            for reading in data:
                self._sensor_storage.write(struct.pack("f", reading))

            if pkt_wait == 0:
                pkt[0] = data[0] #time
                pkt[1:5] = data[4:8] #geo_quaternion
                pkt[5] = data[12] #altitude
                pkt_wait = self._sms.send_pkt(pkt)

            if capture_gps:
                long, lat, alt = self._gps.get_loc()
                self._gps_storage.write(struct.pack("fff", [long, lat, alt]))

            capture_gps = not capture_gps
            end = millis()
            sleep_ms(max(0, self._delay - (end - start)))
        self._sensor_storage.flush()
        self._sensor_storage.close()
        self._gps_storage.flush()
        self._gps_storage.close()
        self._sms.disconnect()
        return postflight(self._sms, self._gps)


class postflight(state):
    def __init__(self, sms, gps):
        self._sms = sms
        self._gps = gps

    def run(self):
        resp = ""
        wait = 0
        led.colour(0, 255, 255)
        while True:
            resp = self._sms.recv_msg()
            self._gps.update()

            if resp == "siren":
                speaker.siren()

            elif resp == "location":
                msg = self._gps.create_msg()
                self._sms.send_msg(msg)

            sleep(0.5)

            wait += 1

            if wait == 60:
                speaker.siren()
                wait = 0
