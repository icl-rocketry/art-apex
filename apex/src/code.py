from stateMachine import diagnostic
from speaker import Speaker 
from sensors import Sensors
import time
import board
import neopixel

speaker = Speaker()
sensors = Sensors()
state = diagnostic(speaker, sensors) 
try: 
    while (state is not None):
       state = state.run()
except Exception as e:
    file = open("error.txt", "w")
    file.write(str(e))
    file.close() 
    raise e

for i in range(10):
    print(sensors._getBarometer())

pixel = neopixel.NeoPixel(board.NEOPIXEL, 1)

pixel.brightness = 0.3

# while True:
#     pixel.fill((255, 0, 0))
#     time.sleep(0.5)
#     pixel.fill((0, 255, 0))
#     time.sleep(0.5)
#     pixel.fill((0, 0, 255))
#     time.sleep(0.5)