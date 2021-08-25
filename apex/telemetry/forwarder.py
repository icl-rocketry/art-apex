import asyncio
import websockets
from datetime import datetime
import struct

IP = "192.168.1.16"
UDP_PORT = 8080
WS_PORT = 8081

FRAME_SIZE = 24  # time, quaternion, alt

# This exists in case a normal packet accidentally has the bytes "end" in it, which would mess things up
def is_end_msg(msg):
    return msg[0] == ord("e")


class Frame:
    def __init__(self, time, quat_i, quat_j, quat_k, quat_real,alt):
        self.time = time
        self.quat_i = quat_i
        self.quat_j = quat_j
        self.quat_k = quat_k
        self.quat_real = quat_real
        self.alt = alt

    def to_json(self):
        return "{" + f"""
        \"time\": {self.time},
        \"quat_i\": {self.quat_i},
        \"quat_j\": {self.quat_j},
        \"quat_k\": {self.quat_k},
        \"quat_real\": {self.quat_real},
        \"alt\": {self.alt}
        """.replace(" ", "") + "}"

    def to_csv(self):
        return f"{self.time},{self.quat_i},{self.quat_j},{self.quat_k},{self.quat_real},{self.alt}\n"

class EndFrame:
    def to_json(self):
        return "END"

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
    def __init__(self, queue):
        self.started = False
        self.queue = queue

    def connection_made(self, _):
        print("connection")

    def datagram_received(self, data, _):
        if not self.started:
            if "start" in data.decode("ascii"):
                self.started = True
                # self.file = open(f"dumps/broadcast_{datetime.now().strftime('%H_%M_%S_%m_%d_%Y')}.csv", "w")
                print("starting")
            return
        
        if is_end_msg(data):
            # self.file.close()
            self.started = False
            self.queue.put_nowait(EndFrame())
            print("stopped")
            return
        
        try:
            for frame in parse(data):
                # self.file.write(frame.to_csv())
                self.queue.put_nowait(frame) #Won't error since the queue must have an unlimited size
        except:
            print("Corrupted frame")

class Websockets:
    def __init__(self, queue):
        self.queue = queue
        self.clients = {}
        self.msgs = []
        self.msg_lock = asyncio.Lock()

    async def handler(self, websocket, path):
        self.clients[path] = websocket
        async with self.msg_lock:
            for msg in self.msgs:
                await websocket.send(msg.to_json())
        try:
            while True:
                data = await websocket.recv()
                print(f"{path} sent {data}") #Basically discard data
        except websockets.exceptions.ConnectionClosed:
            print(self.clients, path)
            del self.clients[path]
    
    async def broadcast(self):
        while True:
            frame = await self.queue.get()
            for client in self.clients.values():
                await client.send(frame.to_json())
            async with self.msg_lock:
                if frame.to_json() == "END":
                    self.msgs.clear()
                else:
                    self.msgs.append(frame)


async def main():
    queue = asyncio.Queue()
    ws = Websockets(queue)

    loop = asyncio.get_running_loop()

    await websockets.serve(ws.handler, IP, WS_PORT)
    await loop.create_datagram_endpoint(
        lambda: UDPProtocol(queue),
        local_addr=(IP, UDP_PORT),
    )
    await ws.broadcast()


    while True:
        await asyncio.sleep(60*60)

if __name__ == "__main__":
    asyncio.run(main())
