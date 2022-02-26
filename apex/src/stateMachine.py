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
        self.speaker = speaker
        self.sensors = sensors

    def run(self):
        raise NotImplementedError("Not implemented - Abstract")

class diagnostic(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 1 
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors

    def run(self):
        sleep(3)
        print("Diagonistic state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return calA(self.speaker, self.sensors)

class calA(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 2 
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors
    def run(self):
        print("Calibration Acc state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return calG(self.speaker, self.sensors)

class calG(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 3
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors
    def run(self):
        sleep(2)
        print("Calibration Gyro state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return calM(self.speaker, self.sensors)

class calM(state):
    def __init__(self, speaker :Speaker, sensors : Sensors):
        self.stateid = 4
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors
    def run(self):
        print("Calibration Magnetometer state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return calStatic(self.speaker, self.sensors)


class calStatic(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 5
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors
    def run(self):
        sleep(5)
        print("Static Calibration state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return preFlight(self.speaker, self.sensors)


class preFlight(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 6
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors
    def run(self):
        sleep(2)
        print("Pre flight state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return flight(self.speaker, self.sensors)


class flight(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.stateid = 7
        self.errors = False
        self.speaker = speaker
        self.sensors = sensors
    def run(self):
        print("Flight state")
        f = open("datafile", "wb")
        for _ in range(NUMBER_OF_TRANSFERS):
            start_time = monotonic_ns()
            data = self.sensors.packData()
            f.write(data)
            end_time = monotonic_ns()
            time_taken = (end_time - start_time) / 1000_000_000
            sleep(max(0, TIME_INTERVAL_DATA_TRANSFER - time_taken))
        else:
            f.flush()
            f.close()
            
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            return postFlight(self.speaker, self.sensors)
    
class postFlight(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.speaker = speaker
        self.stateid = 8
        self.errors = False
        self.sensors = sensors
    def run(self):
        sleep(2)
        print("Post Flight state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker, self.sensors)
        else: 
            self.speaker.beep(self.stateid)
            print("flight done!!!")
            return None
           
class errorState(state):
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.speaker = speaker
        self.stateid = 9
        self.errors = False
        self.sensors = sensors
    def run(self):
        sleep(1)
        print("ERROR state")

 
