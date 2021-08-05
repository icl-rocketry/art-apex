import time
import board
import pwmio
from random import randint

class Speaker: 
    def __init__(self, Aout = board.GP7):
        # speaker defaults as off
        self.speaker = pwmio.PWMOut(Aout, duty_cycle = 0, variable_frequency = True)

    def _playtone(self, frequency):
        self.speaker.frequency = frequency

    def noise(self):
        # generate white noise
        # better for localisation than pure tones
        minf = 1000
        maxf = 20000
        for _ in range(0, 4000):
            self._playtone(randint(minf, maxf))
            time.sleep(0.001)

    def _sweep(self, startf = 100, endf = 8100, step = 10, delay = 0.0025):
        for f in range(startf, endf, step):
            self._playtone(f)
            time.sleep(delay)

    def siren(self, cycles = 3):
        # 3x sweep up and down
        for _ in range(cycles):
            self._sweep()
            self._sweep(startf = 8100, endf = 100, step = -10)

    def shutup(self):
        self._playtone(0)
    