from time import sleep
from speaker import Speaker
# barometer ; BNO - acc, gyro, magno ; speaker 
class state: 
    def __init__(self, speaker : Speaker):
        self.speaker = speaker

    def run(self):
        raise NotImplementedError("Not implemented - Abstract")

class diagnostic(state):
    def __init__(self, speaker : Speaker):
        self.stateid = 1 
        self.errors = False
        self.speaker = speaker

    def run(self):
        sleep(3)
        print("Diagonistic state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return calA(self.speaker)

class calA(state):
    def __init__(self, speaker : Speaker):
        self.stateid = 2 
        self.errors = False
        self.speaker = speaker
    def run(self):
        print("Calibration Acc state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return calG(self.speaker)

class calG(state):
    def __init__(self, speaker : Speaker):
        self.stateid = 3
        self.errors = False
        self.speaker = speaker
    def run(self):
        sleep(2)
        print("Calibration Gyro state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return calM(self.speaker)

class calM(state):
    def __init__(self, speaker :Speaker):
        self.stateid = 4
        self.errors = False
        self.speaker = speaker
    def run(self):
        print("Calibration Magnetometer state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return calStatic(self.speaker)


class calStatic(state):
    def __init__(self, speaker : Speaker):
        self.stateid = 5
        self.errors = False
        self.speaker = speaker
    def run(self):
        sleep(5)
        print("Static Calibration state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return preFlight(self.speaker)


class preFlight(state):
    def __init__(self, speaker : Speaker):
        self.stateid = 6
        self.errors = False
        self.speaker = speaker
    def run(self):
        sleep(2)
        print("Pre flight state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return flight(self.speaker)


class flight(state):
    def __init__(self, speaker : Speaker):
        self.stateid = 7
        self.errors = False
        self.speaker = speaker
    def run(self):
        print("Flight state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            return postFlight(self.speaker)



class postFlight(state):
    def __init__(self, speaker : Speaker):
        self.speaker = speaker
        self.stateid = 8
        self.errors = False
    def run(self):
        sleep(2)
        print("Post Flight state")
        if (self.errors):
            self.speaker.longBeep()
            return errorState(self.speaker)
        else: 
            self.speaker.beep(self.stateid)
            print("flight done!!!")
            return None
           


class errorState(state):
    def __init__(self, speaker : Speaker):
        self.speaker = speaker
        self.stateid = 9
        self.errors = False
    def run(self):
        sleep(1)
        print("ERROR state")

if __name__ == '__main__':
    speaker = Speaker()
    st = diagnostic(speaker) 
    while (st is not None):
       st = st.run()     
