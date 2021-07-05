from machine import UART, Pin
from utime import sleep, sleep_ms

OWNER_NUMBER = "0"

class sms:
    def __init__(self, rx: int, tx: int):
        self._uart = UART(1, baudrate=9600, rx=Pin(rx), tx=Pin(tx))

        # Perform self check
        if not self._check():
            print("fail")
            ...  # TODO replace this with a buzz or something

        self._send("AT+CMGF=1")  # Text message mode
        self._report()

        # TODO indicate sim setup complete

    def _send(self, cmd: str) -> str:
        self._uart.write(bytes((cmd+"\r\n").encode("ascii")))
        sleep_ms(50)
        resp = self._uart.read()
        if resp is None:
            return ""
        return resp.decode("ascii")

    def _check(self) -> bool:
        if "ok" not in self._send("at"):
            return False
        if "error" in self._send("at+ccid"):
            return False
        if "error" in self._send("at+creg?"):
            return False
        return True

    # Report will send commands to the sim800l and collect the responses.
    # Then the responses will be sent via sms to the "owner"
    def _report(self):
        rep = ""
        rep += self._send("at+csq")  # signal strength
        rep += self._send("AT+COPS?")  # connected network
        rep += self._send("AT+CBC")  # lipo state
        self.send_msg(rep)

    def send_msg(self, msg: str):
        self._send("AT+CMGS=\"{OWNER_NUMBER}\"")
        self._send(msg)
        self._send(chr(26))
        # self._uart.write(bytes(chr(26)+"\r\n"))


sleep(1)
print("hello world")

s = sms(5, 4)

s._send("AT")

t = lambda m: print(s._send(m))

while True:
    cmd = input("Enter a command: ")
    t(cmd)
