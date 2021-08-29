from time import sleep
from busio import UART


def sleep_ms(ms):
    sleep(ms//1000)


with open("phone.number") as file:
    NUMBERS = file.readlines()
    OWNER_NUMBER = NUMBERS[0]
print(OWNER_NUMBER)

SEND_DELAY_MS = 50
MAX_PKT_SIZE = 320

class sms:
    def __init__(self, rx, tx):
        self._uart = UART(baudrate=57600, rx=rx, tx=tx)
        self.pkt_size = 0

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

    def send_msg(self, msg: str, number=OWNER_NUMBER):
        self._send("AT+CMGF=1")  # Text message mode
        self._send(f"AT+CMGS=\"{number}\"")
        self._send(msg)
        self._uart.write(bytes(chr(26).encode("ascii")))

    def send_msg_all(self, msg: str):
        for num in NUMBERS:
            self.send_msg(msg, num)
            sleep_ms(2000)

    def recv_msg(self, indicator=None) -> str:
        self._send("AT+CMGF=1")  # Text message mode
        self._send("AT+CNMI=1,2,0,0,0")  # Send text message over uart
        resp = "\r\n\r\n"
        while "+CMT" not in resp:
            if indicator is not None:
                indicator.toggle()
            resp = self._uart.read()
            if resp is None:
                resp = "\r\n\r\n"
            else:
                indicator.on()
                resp = resp.decode("ascii")
                words = resp.split("\r\n")
                return words[2]

    def connect(self):
        self._send("AT+CFUN=1")
        self._send("AT+CSTT=\"wap.vodafone.co.uk\",\"wap\",\"wap\"")
        self._send("AT+CIICR")
        sleep_ms(200)
        self._send("AT+CIFSR")
        self._send("AT+CIPSTART=\"UDP\",35.229.97.111,8080")
        sleep_ms(2000)

    #Returns the number of milliseconds that need to be waited
    #This will have some gaps though, especially when sending a new packet
    def send_pkt(self, pkt: bytes) -> int:
        if self.pkt_size == 0:
            self._uart.write(bytes((f"AT+CIPSEND={MAX_PKT_SIZE}\r\na").encode("ascii")))
            return 4
        
        self._uart.write(pkt)
        self.pkt_size += len(pkt)
        if self.pkt_size >= MAX_PKT_SIZE:
            self._uart.write(bytes("\r\n".encode("ascii")))
            return 3
        return 1

    def disconnect(self):
        self._send("AT+CIPCLOSE")
        self._send("AT+CIPSHUT")


#AT+CFUN=1
#AT+CPIN?
#AT+CSTT="wap.vodafone.co.uk","wap","wap"
#lAT+CIICR
#AT+CIFSR
#lAT+CIPSTART="TCP","exploreembedded.com",80
#lAT+CIPSEND=63
#lGET exploreembedded.com/wiki/images/1/15/Hello.txt HTTP/1.0
