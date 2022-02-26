from time import sleep, monotonic_ns
from sensors import Sensors
from speaker import Speaker

# barometer ; BNO - acc, gyro, magno ; speaker 

DATA_RATE = 10  # 10 Hz
TIME_INTERVAL_DATA_TRANSFER = 1 / DATA_RATE
RECORDING_TIME = 15*60
NUMBER_OF_TRANSFERS = RECORDING_TIME * DATA_RATE

class state:
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 0
        self.speaker = speaker
        self.sensors = sensors
        self.errors = False

    def _run (self):
        raise NotImplementedError()

    def run (self):
        next_state = self._run()
        if self.errors:
            self.speaker.longBeep()
            return errorState
       
        self.speaker.beep(self.stateid)
        return next_state(self.speaker, self.sensors)


class diagnostic(state):
    stateid = 1

    def _run (self):
        sleep(3)
        print("Diagonistic state")
        return calA

class calA(state):
    stateid = 2

    def _run (self):
        print("Calibration Acc state")
        return calG

class calG(state):
    stateid = 3

    def _run (self):
        sleep(2)
        print("Calibration Gyro state")
        return calM

class calM(state):
    stateid = 4

    def _run (self):
        print("Calibration Magnetometer state")
        return calStatic


class calStatic(state):
    stateid = 5

    def _run (self):
        sleep(5)
        print("Static Calibration state")
        return preFlight


class preFlight(state):
    stateid = 6

    def _run (self):
        sleep(2)
        print("Pre flight state")
        return flight


class flight(state):
    stateid = 7

    def _run (self):
        print("Flight state")
        f = open("datafile", "wb")
        for _ in range(NUMBER_OF_TRANSFERS):
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
       
    def _run (self):
        sleep(2)
        print("Post Flight state")
           
class errorState(state):
    stateid = 9

    def _run (self):
        sleep(1)
        print("ERROR state")