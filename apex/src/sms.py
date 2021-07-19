from time import sleep
from busio import UART


def sleep_ms(ms):
    sleep(ms//1000)


with open("phone.number") as file:
    OWNER_NUMBER = file.readline()

print(OWNER_NUMBER)

SEND_DELAY_MS = 50

class sms:
    def __init__(self, rx, tx):
        self._uart = UART(baudrate=9600, rx=rx, tx=tx)

    def _send(self, cmd: str) -> str:
        self._uart.write(bytes((cmd+"\r\n").encode("ascii")))
        sleep_ms(SEND_DELAY_MS)
        resp = self._uart.read()
        if resp is None:
            return ""
        return resp.decode("ascii")

    # Report will send commands to the sim800l and collect the responses.
    # Then the responses will be sent via sms to the "owner"
    def report(self):
        rep = ""
        rep += self._send("at")
        rep += self._send("at+ccid")
        rep += self._send("at+creg?")
        rep += self._send("at+csq")  # signal strength
        rep += self._send("AT+COPS?")  # connected network
        rep += self._send("AT+CBC")  # lipo state
        self.send_msg(rep)

    def send_msg(self, msg: str):
        self._send("AT+CMGF=1")  # Text message mode
        self._send(f"AT+CMGS=\"{OWNER_NUMBER}\"")
        self._send(msg)
        self._uart.write(bytes(chr(26).encode("ascii")))

    def recv_msg(self) -> str:
        self._send("AT+CMGF=1")  # Text message mode
        self._send("AT+CNMI=1,2,0,0,0")  # Send text message over uart
        resp = ""
        while resp == "" or "+CMT" not in resp:
            resp = self._uart.read()
            if resp is None:
                resp = ""

        msg = resp.decode("ascii")
        words = msg.split("\r\n")
        return words[2]
