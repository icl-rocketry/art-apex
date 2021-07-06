#The state machine has 3 states:
# 1. Preflight - calibrates all sensors, and waits for a text message containing the message "launch".
# 2. Flight - takes readings from sensors for FLIGHT_TIME minutes.
# 3. Postflight - takes gps readings and sends them to GROUND_CONTROL_PHONE, until a reply is received.
#                 It also plays sounds over the speakers, in case the earlier method failed

class state:
    def run(self):
        raise NotImplementedError("Not implemented")

class preflight(state):
    def run(self):
        pass

class flight(state):
    def run(self):
        pass

class postflight(state):
    def run(self):
        pass