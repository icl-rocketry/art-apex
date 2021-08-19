import asyncio
import websockets

IP = "192.168.1.16"
WS_PORT = 8081

async def hello():
    uri = f"ws://{IP}:{WS_PORT}"
    async with websockets.connect(uri) as websocket:
        while True:
            frame = await websocket.recv()
            if frame == "END":
                print("Done")
                break
            print(frame)

asyncio.get_event_loop().run_until_complete(hello())
