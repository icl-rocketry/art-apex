import struct

_DATA_SIZE = 48

class Sensors:
    def __init__(self) -> None:
        pass

    def _getBarometer(self):
        temp = 2 
        pressure = 2
        return pressure, temp

    def _getAccelerometer(self):
        print("reading acc data")
        return [1.111, 2.222, 3.333]  

    def _getGyroscope(self):
        print("reading gyro data")
        return [1.111, 2.222, 3.333] 

    def _getMagnetic(self):
        print("reading Mag data")
        return [1.111, 2.222, 3.333]  
    
    def calibrate(self):
        print("Calibrating")
    
    def packData(self):
        acc = self._getAccelerometer()
        gyro = self._getGyroscope()
        mag = self._getMagnetic()
        pressure, temp = self._getBarometer()
        time = 1000
        arr = bytearray(_DATA_SIZE)
        struct.pack_into("fff", arr, 0, acc[0], acc[1], acc[2])
        struct.pack_into("fff", arr, 12, gyro[0], gyro[1], gyro[2])
        struct.pack_into("fff", arr, 24, mag[0], mag[1], mag[2])
        struct.pack_into("f", arr, 36, pressure)
        struct.pack_into("f", arr, 40, temp)
        struct.pack_into("f", arr, 44, time)
        return arr

if __name__ == '__main__':
    sensors = Sensors()
    print(sensors.packData())

