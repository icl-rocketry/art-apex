from external import UART
from external import sleep_ms

OWNER_NUMBER = "0"
SEND_DELAY_MS = 50

class sms:
    def __init__(self, rx, tx):
        self._uart = UART(baudrate=9600, rx=rx, tx=tx)

        # Perform self check
        if not self._check():
            ...  # TODO replace this with a buzz or something

        self._send("AT+CMGF=1")  # Text message mode
        self._report()

        # TODO indicate sim setup complete

    def _send(self, cmd: str) -> str:
        self._uart.write(bytes((cmd+"\r\n").encode("ascii")))
        sleep_ms(SEND_DELAY_MS)
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
        self._uart.write(bytes(chr(26)))
