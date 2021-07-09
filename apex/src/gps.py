#The gps class is designed as if the component always has a fix
#However since this isn't usually true, the class will wait for the gps to actually get a fix
#before reporting data. This means that the time taken to get data is variable


from external import I2C
from external import GPS_GtopI2C

class gps:
    def __init__(self, i2c, frequency):
        self._gps = GPS_GtopI2C(i2c, debug=False)
        self._gps.send_command(b"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
        self._gps.send_command(bytes(f"PMTK220,{1000//frequency}"))

    #This might take some more time depending on whether the gps has got a fix
    #Note, if the gps loses fix in the middle of this method, then anything could happen - but it's unlikely
    def get_long_lat(self):
        self._gps.update()
        while not self._gps.has_fix:
            self._gps.update()
        return self._gps.longitude, self._gps.latitude
    