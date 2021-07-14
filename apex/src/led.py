import board
import pwmio

class LED:
    def __init__(self):
        self.rled = pwmio.PWMOut(board.LED_R, frequency = 1000)
        self.gled = pwmio.PWMOut(board.LED_G, frequency = 1000)
        self.bled = pwmio.PWMOut(board.LED_B, frequency = 1000)
        self.state = False
        self.r = 255
        self.g = 255
        self.b = 255

    def _set(self, r, g, b):
        rduty = int(65535 -(65535 * r/255))
        gduty = int(65535 -(65535 * g/255))
        bduty = int(65535 -(65535 * b/255))
        self.rled.duty_cycle = rduty
        self.gled.duty_cycle = gduty
        self.bled.duty_cycle = bduty

    def colour(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b
        self._set(r, g, b)
    
    def toggle(self):
        self.state = not self.state
        self._set(self.r*self.state, self.g*self.state, self.b*self.state)

        