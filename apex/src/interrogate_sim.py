import time
import busio
import board
import pwmio

rled = pwmio.PWMOut(board.LED_R, frequency=440)
gled = pwmio.PWMOut(board.LED_G, frequency=440)
bled = pwmio.PWMOut(board.LED_B, frequency=440)

def LED(r,g,b):
    rduty = int(65535 -(65535 * r/255))
    gduty = int(65535 -(65535 * g/255))
    bduty = int(65535 -(65535 * b/255))
    rled.duty_cycle = rduty
    gled.duty_cycle = gduty
    bled.duty_cycle = bduty

print("Initialising SIM")
LED(255, 0, 0)
sim = busio.UART(baudrate = 9600, rx = board.GP5, tx = board.GP4)

def send(cmd: str) -> str:
        sim.write(bytes((cmd+"\r\n").encode("ascii")))
        time.sleep(0.05)
        resp = sim.read()
        if resp is None:
            print("No response")
            return "No response"
        print(resp.decode("ascii"))
        return resp.decode("ascii")

while True:
    inp = input("Command: ")
    send(inp)
    time.sleep(1)
