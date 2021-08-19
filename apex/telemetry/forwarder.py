import asyncio
from datetime import datetime
import struct

UDP_IP = "192.168.1.16"
UDP_PORT = 8080

FRAME_SIZE = 4 + 12 + 12 + 12 + 4  # time, accel, gyro, mag, alt

# This exists in case a normal packet accidentally has the bytes "end" in it, which would mess things up


def is_end_msg(msg):
    return msg[0] == ord("e")


class Frame:
    def __init__(self, time, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, mag_x, mag_y, mag_z, alt):
        self.time = time
        self.accel_x = accel_x
        self.accel_y = accel_y
        self.accel_z = accel_z
        self.gyro_x = gyro_x
        self.gyro_y = gyro_y
        self.gyro_z = gyro_z
        self.mag_x = mag_x
        self.mag_y = mag_y
        self.mag_z = mag_z
        self.alt = alt

    def to_json(self):
        return "{" + f"""
        \"time\": {self.time},
        \"accel_x\": {self.accel_x},
        \"accel_y\": {self.accel_y},
        \"accel_z\": {self.accel_z},
        \"gyro_x\": {self.gyro_x},
        \"gyro_y\": {self.gyro_y},
        \"gyro_z\": {self.gyro_z},
        \"mag_x\": {self.mag_x},
        \"mag_y\": {self.mag_y},
        \"mag_z\": {self.mag_z},
        \"alt\": {self.alt}
        """.replace(" ", "") + "}"

    def to_csv(self):
        return f"{self.time},{self.accel_x},{self.accel_y},{self.accel_z},{self.gyro_x},{self.gyro_y},{self.gyro_z},{self.mag_x},{self.mag_y},{self.mag_z},{self.alt}\n"


def parse(msg):
    arr = bytearray(msg[1:])
    frames = []
    for i in range(0, len(arr), FRAME_SIZE):
        frame = []
        for j in range(0, FRAME_SIZE, 4):
            frame += [struct.unpack("f", arr[i+j:i+j+4])[0]]
        frames.append(Frame(*frame))
    return frames

class UDPProtocol:
    def __init__(self):
        self.started = False

    def connection_made(self, _):
        print("connection")

    def datagram_received(self, data, _):
        if not self.started:
            if "start" in data.decode("ascii"):
                self.started = True
                self.file = open(f"dumps/broadcast_{datetime.now().strftime('%H_%M_%S_%m_%d_%Y')}.csv", "w")
                print("starting")
                return
        
        if is_end_msg(data):
            self.file.close()
            self.started = False
            print("stopped")
            return
        
        for frame in parse(data):
            self.file.write(frame.to_csv())

            
    

async def main():
    loop = asyncio.get_running_loop()
    transport, protocol = await loop.create_datagram_endpoint(
        lambda: UDPProtocol(),
        local_addr=(UDP_IP, UDP_PORT),
    )

    while True:
        await asyncio.sleep(60*60)

if __name__ == "__main__":
    asyncio.run(main())
