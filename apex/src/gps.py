# The gps class is designed as if the component always has a fix
# However since this isn't usually true, the class will wait for the gps to actually get a fix
# before reporting data. This means that the time taken to get data is variable


from busio import I2C
from adafruit_gps import GPS_GtopI2C


class gps:
    def __init__(self, i2c, frequency):
        self._gps = GPS_GtopI2C(i2c, debug=False)
        self._gps.send_command(
            b"PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
        self._gps.send_command(bytes(f"PMTK220,{1000//frequency}"))
        self._gps.update()
        self.status = self._gps.has_fix()

    def update(self):
        self._gps.update()
        self.status = self._gps.has_fix

    def _get_loc(self):
        return self._gps.latitude, self._gps.longitude

    def create_msg(self):
        self.update()
        if not self.status:
            return "No fix"
        else:
            lat, long = self._get_loc()
            link = "https://maps.google.com/maps?q=" + \
                str(round(lat, 7)) + "%2C" + str(round(long, 7))

            fixtime = "{:02}:{:02}:{:02}".format(
                self._gps.timestamp_utc.tm_hour,  # not get all data like year, day,
                self._gps.timestamp_utc.tm_min,  # month!
                self._gps.timestamp_utc.tm_sec)

            quality = self._gps.fix_quality()

            msg = "Location: {}\nFix acquired at {}\nFix Quality {}".format(
                link, fixtime, quality)
            return msg
