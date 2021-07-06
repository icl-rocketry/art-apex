from external import millis

DATA_SIZE = 9

class sensors:

    data_size = DATA_SIZE
    _data = [float()] * DATA_SIZE
    _start = millis()

    #All sensors will share the same i2c bus
    def __init__(self, rx, tx):
        #Initialise sensors
        ...
    
    def get(self):
        return self._data