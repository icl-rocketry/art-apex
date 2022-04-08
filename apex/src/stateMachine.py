from time import sleep, monotonic_ns
from sensors import Sensors
from speaker import Speaker
import neopixel
import board

# barometer ; BNO - acc, gyro, magno ; speaker 

DATA_RATE = 10  # 10 Hz
TIME_INTERVAL_DATA_TRANSFER = 1 / DATA_RATE
RECORDING_TIME = 15*60
N_RECORDINGS = RECORDING_TIME * DATA_RATE
PREFLIGHT_WAIT_TIME = 15*60

pixel = neopixel.NeoPixel(board.NEOPIXEL, 1) #type: ignore
pixel.brightness = 0.3

class state:
    colour = (0, 0, 0)
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 0
        self.speaker = speaker
        self.sensors = sensors
        self.errors = False

    def _run(self) -> type[state] | None:
        raise NotImplementedError()

    def run (self) -> type[state] | None:
        pixel.fill(self.colour)
        next_state = self._run()
        if self.errors:
            self.speaker.longBeep(3)
            return errorState
       
        self.speaker.beep(self.stateid)
        if next_state != None:
            return next_state(self.speaker, self.sensors)


class diagnostic(state):
    stateid = 1
    colour = (255, 255, 0)

    def __init__(self):
        self.speaker = Speaker()
        self.sensors = Sensors()

    def _run (self) -> type[calA]:
        sleep(3)
        print("Diagonistic state")
        return calA

class calA(state):
    stateid = 2
    colour = (0, 0, 255)

    def _run (self) -> type[calG]:
        print("Calibration Acc state")
        return calG

class calG(state):
    stateid = 3
    colour = (255, 255, 255)

    def _run (self) -> type[calM]:
        sleep(2)
        print("Calibration Gyro state")
        return calM

class calM(state):
    stateid = 4
    colour = (255, 0, 255)

    def _run (self) -> type[calStatic]:
        sleep(2)
        print("Calibration Magnetometer state")
        return calStatic


class calStatic(state):
    stateid = 5
    colour = (0, 255, 255)

    def _run (self) -> type[preFlight]:
        sleep(5)
        print("Static Calibration state")
        return preFlight


class preFlight(state):
    stateid = 6
    colour = (255,165,0)

    def _run (self) -> type[flight]:
        sleep(PREFLIGHT_WAIT_TIME)
        print("Pre flight state")
        self.speaker.beep(10)
        return flight


class flight(state):
    stateid = 7
    colour = (255, 0, 0)
    def _run (self) -> type[postFlight]:
        print("Flight state")
        f = open("datafile", "wb")
        for _ in range(N_RECORDINGS):
            start_time = monotonic_ns()
            data = self.sensors.packData()
            f.write(data)
            end_time = monotonic_ns()
            time_taken = (end_time - start_time) / 1000_000_000
            sleep(max(0, TIME_INTERVAL_DATA_TRANSFER - time_taken))

        f.flush()
        f.close()
        return postFlight

class postFlight(state):
    stateid = 8
    colour = (108, 122, 137)
       
    def _run(self) -> None:
        while True:
            self.speaker.siren(3)
            sleep(10)
        
class errorState(state):
    stateid = 9
    colour = (255, 255, 0)
    def _run(self) -> None:
        while True:
            self.speaker.siren(4)
            sleep(10)