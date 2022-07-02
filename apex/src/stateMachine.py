from time import sleep, monotonic_ns
from sensors import Sensors
from speaker import Speaker
import neopixel
import board
import os

# barometer ; BNO - acc, gyro, magno ; speaker 

DATA_RATE = 10  # 10 Hz
TIME_INTERVAL_DATA_TRANSFER = 1 / DATA_RATE
RECORDING_TIME = 15 * 60 #15 * 60
N_RECORDINGS = RECORDING_TIME * DATA_RATE
PREFLIGHT_WAIT_TIME = 30 * 60 # 30 * 60

pixel = neopixel.NeoPixel(board.NEOPIXEL, 1) #type: ignore
pixel.brightness = 0.3

class state:
    colour = (0, 0, 0)
    def __init__(self, speaker : Speaker, sensors : Sensors):
        self.speaker = speaker
        self.sensors = sensors

    def _run(self) -> type[state] | None:
        raise NotImplementedError()

    def run (self) -> type[state] | None:
        pixel.fill(self.colour)
        try:
            next_state = self._run()
        except Exception as e:
            pixel.fill((255, 0, 0))
            self.speaker.longBeep(3)
            raise e
       
        self.speaker.beep(self.stateid)
        if next_state != None:
            return next_state(self.speaker, self.sensors)


class diagnostic(state):
    stateid = 1
    colour = (255, 255, 0) ## yellow

    def __init__(self):
        pass

    def _run (self) -> type[calA]:
        print("Diagnostic state")
        self.speaker = Speaker()
        self.sensors = Sensors()
        if "datafile" in os.listdir():
            raise Exception("Datafile currently exists, can't record")
        elif "boot.py" not in os.listdir():
            raise Exception("Can't write data - no boot.py found")
        return calA

class calA(state):
    stateid = 2
    colour = (0, 0, 255) ##Blue

    def _run (self) -> type[calG]:
        self.sensors.calibrate()
        sleep(2)
        print("Calibration Acc state")
        return calG

class calG(state):
    stateid = 3
    colour = (255, 255, 255) #white

    def _run (self) -> type[calM]:
        print("Calibration Gyro state")
        sleep(2)
        return calM

class calM(state):
    stateid = 4
    colour = (255, 0, 255) #magenta 

    def _run (self) -> type[calStatic]:
        print("Calibration Magnetometer state")
        sleep(2)
        return calStatic


class calStatic(state):
    stateid = 5
    colour = (0, 255, 255) #cyan

    def _run (self) -> type[preFlight]:
        print("Static Calibration state")
        sleep(5)
        return preFlight


class preFlight(state):
    stateid = 6
    colour = (255,165,0) #orange 

    def _run (self) -> type[flight]:
        print("Pre flight state")
        sleep(PREFLIGHT_WAIT_TIME)
        self.speaker.playsong("rick.mp3")
        return flight


class flight(state):
    stateid = 7
    colour = (255, 0, 0) #red
    def _run (self) -> type[postFlight]:
        print("Flight state")
        try:
            f = open("datafile", "wb")
            for _ in range(N_RECORDINGS):
                start_time = monotonic_ns()
                data = self.sensors.packData()
                f.write(data)
                end_time = monotonic_ns()
                time_taken = (end_time - start_time) / 1000_000_000
                sleep(max(0, TIME_INTERVAL_DATA_TRANSFER - time_taken))
        except Exception as e:
            print(e)
        finally:
            f.flush()
            f.close()
        return postFlight

class postFlight(state):
    stateid = 8
    colour = (108, 122, 137) #greyish blue      
    def _run(self) -> None:
        print("Post flight state")
        sleep(60*20)
        while True:
            self.speaker.siren(1)
            self.speaker.noise()
            self.speaker.shutup()
            sleep(60)
